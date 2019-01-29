#include "dialog.h"

#include <math.h>

//core
#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QSize>
#include <QThread>
#include <QDirIterator>
#include <QFileInfo>

//misc gui
#include <QIcon>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QStandardPaths>

#ifdef Q_OS_WIN
#include <QWinTaskbarProgress>
#include <QWinTaskbarButton>
#endif

#include "util.h"
#include "dialog_ui.h"
#include "resizeworker.h"

/*! Constructor for Dialog.
  \param parent The dialog's parent window.
  */
Dialog::Dialog(QWidget *parent) : QDialog(parent)
{
    setupUi();
    retranslateUi();
    readSettings();
    createConnections();

    nameFilters_ << "*.jpg" << "*.jpeg";
    filters_ = QFlags<QDir::Filter>(QDir::DirsFirst | QDir::Files | QDir::Dirs | QDir::Drives);
    running_ = false;

    fileCount_ = {};

    resize(sizeHint());
#ifdef USE_PRESETS
    readPresets();
#endif
}

Dialog::~Dialog()
{

}

//! Instantiates and configures widgets and layouts.
void Dialog::setupUi()
{
    setWindowFlags(Qt::Dialog
                   | Qt::CustomizeWindowHint
                   | Qt::WindowTitleHint
                   | Qt::WindowMinimizeButtonHint
                   | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/images/resize.png"));

    ui_.reset(new ResizeDialogUi);

    //set the master layout
    setLayout(ui_->layout);

    //add the layouts for the top half of the dialog
    ui_->layout->addLayout(ui_->topLayout);
    ui_->filesGroupBox->setLayout(ui_->filesLayout);
    ui_->resizeGroupBox->setLayout(ui_->resizeLayout);
    ui_->advancedBox->setLayout(ui_->advancedLayout);

    //the rest of the initialization for these widgets is deferred, so we just add them
    ui_->filesLayout->addRow(ui_->srcLabel, ui_->srcEdit);
    ui_->filesLayout->addRow(ui_->destLabel, ui_->destEdit);
    ui_->filesLayout->addWidget(ui_->recurseBox);
    ui_->filesLayout->addWidget(ui_->overwriteBox);

    ui_->resizeLayout->addRow(ui_->resizeLabel, ui_->resizeWidget);
    ui_->resizeLayout->addRow(ui_->skipConditionLabel, ui_->skipConditionBox);
    ui_->resizeLayout->addWidget(ui_->copySkippedBox);

    //Advanced Groupbox
    ui_->blurBox->setDecimals(4);
    ui_->blurBox->setMinimum(0.0005);
    ui_->blurBox->setMaximum(2.0);
    ui_->blurBox->setSingleStep(0.0005);
    ui_->blurBox->setValue(1.0);

    ui_->advancedLayout->addRow(ui_->blurLabel, ui_->blurBox);

    //add the group boxes and configure topLayout
    ui_->topLayout->addWidget(ui_->filesGroupBox, 0, 0, 1, 3);
    ui_->topLayout->addWidget(ui_->resizeGroupBox, 1, 0);
    ui_->topLayout->addWidget(ui_->advancedBox, 1, 1);
    ui_->topLayout->setColumnStretch(2, 1);
    ui_->topLayout->setHorizontalSpacing(10);

    //set the label buddies for the top half area
    ui_->srcLabel->setBuddy(ui_->srcEdit);
    ui_->destLabel->setBuddy(ui_->destEdit);
    ui_->skipConditionLabel->setBuddy(ui_->skipConditionBox);
    ui_->resizeLabel->setBuddy(ui_->resizeWidget);
    ui_->blurLabel->setBuddy(ui_->blurBox);

    //Init the lower half of the dialog
    ui_->progressBar->setFormat("  %v/%m (%p%)");
    ui_->progressBar->setVisible(false);

    ui_->debugWidget->setVisible(false);

    //dialog Buttons
    QIcon ico;
    ico.addFile(":/images/start.png", QSize(), QIcon::Normal, QIcon::Off);
    ico.addFile(":/images/stop.png", QSize(), QIcon::Normal, QIcon::On);
    ui_->startButton->setIcon(ico);
    ui_->startButton->setCheckable(true);

    ui_->debugButton->setCheckable(true);

    //lower half layout additions
    ui_->buttonLayout->addWidget(ui_->startButton);
    ui_->buttonLayout->addWidget(ui_->debugButton);
    ui_->buttonLayout->addStretch(1);
    ui_->buttonLayout->addWidget(ui_->closeButton);

    ui_->layout->addWidget(ui_->progressBar);
    ui_->layout->addWidget(ui_->debugWidget, 1);
    ui_->layout->addLayout(ui_->buttonLayout);

    populateComboBoxes();

#ifdef Q_OS_WIN
    (void)winId(); //forces Qt to create an HWND and QWindow wrapper so that windowHandle() works
    taskbarButton_ = new QWinTaskbarButton(this);
    taskbarButton_->setWindow(windowHandle());
    taskbarButton_->setOverlayIcon(QIcon(":/images/start.png"));
    taskbarProgress_ = taskbarButton_->progress();
    Q_ASSERT(taskbarProgress_);
#endif

#ifdef USE_PRESETS
    ui_->presetGroupBox = new QGroupBox("Presets");
    QHBoxLayout *presetLayout = new QHBoxLayout(ui_->presetGroupBox);
    ui_->presetComboBox = new QComboBox;
    ui_->presetManageBtn = new QPushButton("&Manage...");
    ui_->presetManageBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    presetLayout->addWidget(ui_->presetComboBox);
    presetLayout->addWidget(ui_->presetManageBtn);
    presetLayout->addStretch(1);

    ui_->layout->insertWidget(0, ui_->presetGroupBox);
#endif
}

//! Utility function called by setupUi()
void Dialog::populateComboBoxes()
{
    ui_->skipConditionBox->addItem("", static_cast<int>(Global::SkipNone));
    ui_->skipConditionBox->addItem("", static_cast<int>(Global::SkipSmaller));
    ui_->skipConditionBox->addItem("", static_cast<int>(Global::SkipLarger));
}

//! For translation support.
void Dialog::retranslateUi()
{
    setWindowTitle(tr("Image Resizer"));

    ui_->filesGroupBox->setTitle(tr("Files"));
    ui_->resizeGroupBox->setTitle(tr("Resize"));
    ui_->advancedBox->setTitle(tr("Advanced"));

    ui_->srcLabel->setText(tr("&Source"));
    ui_->destLabel->setText(tr("&Destination"));
    ui_->resizeLabel->setText(tr("&Resize"));
    ui_->resizeLabel->setToolTip(tr("<p>Sets the method and size for resizing images. Aspect ratio will be preserved.</p><p><span style=\" font-weight:600;\">Ratio</span>: Scale as a percentage of the original size<br/><span style=\"font-weight:600;\">Height, Width</span>: Scales the image based on a fixed size<br/><span style=\" font-weight:600;\">Smaller, Larger</span>: Same as height or width, except chooses the smaller or larger edge of each image processed."));
    ui_->blurLabel->setText(tr("&Blur"));
    ui_->skipConditionLabel->setText(tr("S&kip File"));

    ui_->skipConditionBox->setItemText(0, tr("None"));
    ui_->skipConditionBox->setItemText(1, tr("If smaller than fixed size"));
    ui_->skipConditionBox->setItemText(2, tr("If larger than fixed size"));

    ui_->overwriteBox->setText(tr("&Overwrite existing destination files"));
    ui_->recurseBox->setText(tr("Include files in subfolders"));
    ui_->copySkippedBox->setText(tr("Copy skipped files"));

    ui_->resizeWidget->retranslateUi();

    ui_->startButton->setText(tr("Start"));
    ui_->startButton->setToolTip(tr("Start"));

    ui_->debugButton->setText(tr("De&bug"));
    ui_->closeButton->setText(tr("&Close"));
}

//! Called during initialization to connect signals and slots.
void Dialog::createConnections()
{
    connect(ui_->closeButton, &QPushButton::clicked, this, &QDialog::close);
    connect(ui_->startButton, &QPushButton::clicked, this, &Dialog::startButtonClicked);
    connect(ui_->debugButton, &QPushButton::clicked, this, &Dialog::debugClicked);
    connect(ui_->resizeWidget, &ResizeSettingsWidget::currentModeChanged,
            this, &Dialog::resizeModeChanged);
#ifdef USE_PRESETS
    //connect(ui_->presetWidget, SIGNAL(manageButtonClicked()), this, SLOT(managePresets()));
    connect(ui_->presetManageBtn, SIGNAL(clicked()), this, SLOT(managePresets()));
#endif
}

void Dialog::setWidgetsEnabled(bool enable)
{
#ifdef USE_PRESETS
    ui_->presetGroupBox->setEnabled(enable);
    ui_->presetGroupBox->update();
#endif
    ui_->filesGroupBox->setEnabled(enable);
    ui_->filesGroupBox->update();
    ui_->resizeGroupBox->setEnabled(enable);
    ui_->resizeGroupBox->update();
    ui_->advancedBox->setEnabled(enable);
    ui_->advancedBox->update();
    ui_->closeButton->setEnabled(enable);
}

/*! Internal slot to enable or disable the Exceptions group Box->
 * The exceptions group box is only enabled when the ScaleMode is \em not Ratio, so it is necessary
 * to disable or enable it when the resize mode changes.
 * \param mode New scale mode.
 * \sa ResizeSettingsWidget, Global::ScaleMode
 */
void Dialog::resizeModeChanged(Global::ScaleMode mode)
{
    bool ratio = (mode == Global::Ratio);
    ui_->skipConditionBox->setEnabled(!ratio);
    ui_->skipConditionLabel->setEnabled(!ratio);
    ui_->copySkippedBox->setEnabled(!ratio);
}

//! Internal slot called by startButton when clicked to start or stop processing.
void Dialog::startButtonClicked(bool clicked)
{
    clicked ? startProcessing() : stopProcessing();
}

/*! \brief Preps and starts the resize threads.
 *  The number of threads created is determined by calling QThread::idealThreadCount().
 */
void Dialog::startProcessing()
{
    QDir source(ui_->srcEdit->currentPath());
    QDir target(ui_->destEdit->currentPath());

    //using the clue bat
    if (running_) {
        qDebug() << "Called startProcessing() while running";
        ui_->startButton->setChecked(false);
        return;
    }

    //using the clue bat again
    if (!source.exists()) {
        qDebug() << "Source path does not exist.";
        ui_->startButton->setChecked(false);
        return;
    }

    //make sure we can access the destination directory
    if (!target.exists()) {
        target.mkpath(target.absolutePath());
        if (!target.exists()) {
            qDebug() << "Cannot create target path.";
            ui_->startButton->setChecked(false);
            return;
        }
    }

    //check if we're going to be overwriting files
    if (source == target) {
        if (QMessageBox::question(this, tr("Continue?"),
                              tr("Source and target directory are the same. Original files will be overwritten! Continue anyway?"),
                              QMessageBox::Yes | QMessageBox::Cancel,
                              QMessageBox::Cancel)
                == QMessageBox::Cancel)
            return;
    }

    //get the file list; ensure it's not empty
    auto flags = ui_->recurseBox->isChecked()
            ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags;
    fileList_ = Util::fileList(source, nameFilters_, filters_, flags);

    if (fileList_.length() == 0) {
        QMessageBox::information(this, tr("No files"),
                                 tr("No valid files found to process."),
                                 QMessageBox::Ok, QMessageBox::Ok);
        ui_->startButton->setChecked(false);
        return;
    }

    //we're going with the paths currently in the editors, so commit them to the history
    ui_->srcEdit->commitCurrentPath();
    ui_->destEdit->commitCurrentPath();

    //reset the counting variables
    fileCount_ = {};
    fileCount_.total = fileList_.length();
    errorStrings_.clear();

    //create thread settings data
    ScaleSettings settings;
    settings.overwrite = ui_->overwriteBox->isChecked();
    settings.value = ui_->resizeWidget->currentValue();
    settings.scale_mode = ui_->resizeWidget->currentMode();
    settings.skip_mode = Global::SkipMode(ui_->skipConditionBox->itemData(ui_->skipConditionBox->currentIndex()).toInt());
    settings.copy_skipped = ui_->copySkippedBox->isChecked();
    settings.blur = ui_->blurBox->value();

    //disable anything we shouldn't be using while resizing images
    setWidgetsEnabled(false);

    //setup the progress bar
    ui_->progressBar->setRange(0, fileList_.length());
    ui_->progressBar->setValue(0);
    ui_->progressBar->setVisible(true);

#ifdef Q_OS_WIN
    //setup the taskbar overlay and progress bar
    taskbarButton_->setOverlayIcon(QIcon(":/images/start.png"));
    taskbarProgress_->setRange(0, fileList_.length());
    taskbarProgress_->show();
    taskbarProgress_->reset();
#endif

    ui_->startButton->setText("&Stop");

    running_ = true;

    //setup and start the threads
    auto thread_count = QThread::idealThreadCount();
    QThread::Priority priority = QThread::NormalPriority;
    QList<FileDataList> file_data;
    int idx, t;
    QThread *rt = nullptr;
    ResizeWorker *rw = nullptr;

    //idealThreadCount() may return -1
    if (thread_count < 1)
        thread_count = 1;

    threads_.reserve(thread_count);
    workers_.reserve(thread_count);

    //create empty FileDataLists to populate
    for (t = 0; t < thread_count; ++t)
        file_data << FileDataList();

    //chop up the file list for each thread
    for (idx = 0, t = 0; idx < fileList_.length(); ++idx, ++t) {
        if (t == thread_count)
            t = 0;
        file_data[t] << FileData(fileList_[idx], idx);
    }

    //create the threads
    for (t = 0; t < thread_count; ++t) {
        rt = new QThread;
        threads_.append(rt);
        rw = ResizeWorker::create(source.path(), target.path(), file_data[t], settings, rt);
        connect(rw, &ResizeWorker::progress, this, &Dialog::setProgress/*, Qt::QueuedConnection*/);
        threads_.append(rt);
        workers_.append(rw);
        rt->start(priority);
    }

    //and finally, start/restart the timer
    timer_.isValid() ? (void)timer_.restart() : timer_.start();

}

/*! Utility function to display dialog with completion summary informatiun.
 * This function is only called by stopProcessing().
 */
void Dialog::showFinishedDialog(qint64 elapsed)
{
    QMessageBox box(this);
    box.setWindowTitle("Image Resizer");
    box.setStandardButtons(QMessageBox::Ok);

    if (fileCount_.finished == fileCount_.total) {
        box.setIcon(QMessageBox::Information);
        box.setText("Operation completed sucessfully");
    } else {
        box.setIcon(QMessageBox::Warning);
        box.setText("Operation aborted");
    }

    auto msg = QString("Elapsed:\t\t%6\nProcessed:\t%1/%2\nCompleted:\t%3\nSkipped:\t%4\nFailed:\t\t%5");
    msg = msg.arg(
              QString::number(fileCount_.finished),
              QString::number(fileCount_.total),
              QString::number(fileCount_.success),
              QString::number(fileCount_.skipped),
              QString::number(fileCount_.error),
              Util::millisecondsToString(elapsed));
    box.setInformativeText(msg);
    box.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    QString txt;
    foreach(QString err_str, errorStrings_)
        txt.append(err_str).append('\n');
    txt.chop(1);
    box.setDetailedText(txt);

    box.exec();
}

/*! Slot called when processing finishes, is aborted, or the dialog is closed.
 * \sa startProcessing(), closeEvent(), showFinishedDialog()
 */
void Dialog::stopProcessing()
{
    for(auto rw : workers_) {
        if (rw->isRunning())
            rw->stop();
    }

    //...wait for them to finish
    for(auto rt : threads_) {
        if (!rt->isFinished()) {
            rt->quit();
            rt->wait();
        }
    }
    for(auto rw : workers_) {
        disconnect(rw, &ResizeWorker::progress, this, &Dialog::setProgress);
    }
    for (auto rt : threads_) {
        rt->setParent(nullptr);
        rt->deleteLater();
    }

    threads_.clear();
    workers_.clear();

    if (running_)
        running_ = false;

#ifdef Q_OS_WIN
    taskbarProgress_->stop();
    taskbarProgress_->hide();
    taskbarButton_->clearOverlayIcon();
#endif

    showFinishedDialog(timer_.elapsed());

    //clear the data
    fileCount_ = {};
    timer_.invalidate();
    fileList_.clear();

    //Re-enable widgets
    setWidgetsEnabled(true);

    ui_->closeButton->setEnabled(true);
    ui_->startButton->setChecked(false);
    ui_->startButton->setText("&Start");
    ui_->progressBar->setVisible(false);
    resize(minimumSizeHint());
}

/*! Slot for ResizeWorkers to submit progress information.
 * \param reason Cause of the signal (success, error, etc.).
 * \param idx Index of the file in the internal file list.
 * \param msg Optional message used when an error occurs.
 */
void Dialog::setProgress(ResizeWorker::Progress reason, int idx, QString msg)
{
    if (!running_) {
        qDebug() << __FUNCTION__ << "Called while not running";
        return;
    }

    QString txt;
    switch(reason) {
    case ResizeWorker::Progress::Success:
        ++fileCount_.success;
        txt = fileList_.at(idx);
        txt.append(QStringLiteral(": <span style=\" color:#00f;\">success</span>"));
        break;
    case ResizeWorker::Progress::Skipped:
        ++fileCount_.skipped;
        txt = fileList_.at(idx);
        txt.append(QStringLiteral(": <span style=\" color:#077;\">skipped</span>"));
        break;
    case ResizeWorker::Progress::ReadError:
    case ResizeWorker::Progress::WriteError:
    case ResizeWorker::Progress::NotFound:
    case ResizeWorker::Progress::UnknownError:
        ++fileCount_.error;
        txt = fileList_.at(idx);
        txt.append(": <span style=\" color:#f00;\">").append(msg).append("</span>");
        errorStrings_.append(QString(fileList_.at(idx)).append(": ").append(msg));
        qDebug() << fileList_.at(idx) << ": " << msg;
        break;
    }
    ui_->debugWidget->append(txt);
    ++fileCount_.finished;

    ui_->progressBar->setValue(fileCount_.finished);
#ifdef Q_OS_WIN
    taskbarProgress_->setValue(fileCount_.finished);
#endif

    if (fileCount_.finished == fileCount_.total)
        stopProcessing();

}

/*! \reimp
 * Ensures threads are stopped and settings saved.
 * \sa readSettings()
 */
void Dialog::closeEvent(QCloseEvent *ev)
{
    if (running_)
        stopProcessing();
    writeSettings();
    QDialog::closeEvent(ev);
}


//! Commits the current settings.
void Dialog::writeSettings()
{
    QSettings settings(settingsFilePath("settings.ini"), QSettings::IniFormat, this);
    settings.setValue("geometry", saveGeometry());

    QStringList files;

    ui_->srcEdit->commitCurrentPath();
    files = ui_->srcEdit->recentPaths();
    for (int i = 0; i < files.count(); ++i) {
        settings.setValue(QString("src%1").arg(i), fromNative(files.at(i)));
    }

    ui_->destEdit->commitCurrentPath();
    files = ui_->destEdit->recentPaths();
    for (int i = 0; i < files.count(); ++i) {
        settings.setValue(QString("dest%1").arg(i), fromNative(files.at(i)));
    }

    settings.setValue("resize_type", static_cast<int>(ui_->resizeWidget->currentMode()));
    settings.setValue("overwrite", ui_->overwriteBox->isChecked() ? 1 : 0);
    settings.setValue("recurse", ui_->recurseBox->isChecked() ? 1 : 0);

    settings.setValue("ratio_value", ui_->resizeWidget->value(Global::Ratio));
    settings.setValue("smaller_value", ui_->resizeWidget->value(Global::Smaller));
    settings.setValue("larger_value", ui_->resizeWidget->value(Global::Larger));
    settings.setValue("width_value", ui_->resizeWidget->value(Global::Width));
    settings.setValue("height_value", ui_->resizeWidget->value(Global::Height));

    settings.setValue("skip_mode", ui_->skipConditionBox->currentIndex());
    settings.setValue("skip_copy", ui_->copySkippedBox->isChecked() ? 1 : 0);

    settings.setValue("debug_visible", ui_->debugButton->isChecked());
}

/*! Loads settings from previous sessions.
 * \sa closeEvent()
 */
void Dialog::readSettings()
{
    QSettings settings(settingsFilePath("settings.ini"), QSettings::IniFormat, this);
    QByteArray geom = settings.value("geometry").toByteArray();

    //Source and Destination Paths
    QStringList src_files;
    QStringList dest_files;
    QString path;

    for (int i = 0; i < 5; ++i) {
        if (settings.contains(QString("src%1").arg(i))) {
            path = settings.value(QString("src%1").arg(i)).toString();
            src_files.append(toNative(path));
        }
    }

    for (int i = 0; i < 5; ++i) {
        if (settings.contains(QString("dest%1").arg(i))) {
            path = settings.value(QString("dest%1").arg(i)).toString();
            dest_files.append(toNative(path));
        }
    }

    ui_->srcEdit->setRecentPaths(src_files);
    if (src_files.count())
        ui_->srcEdit->setCurrentPath(src_files.at(0));

    ui_->destEdit->setRecentPaths(dest_files);
    if (dest_files.count())
        ui_->destEdit->setCurrentPath(dest_files.at(0));

    bool overwrite = settings.value("overwrite", false).toBool();
    bool recurse = settings.value("recurse", false).toBool();

    int resize_type = settings.value("resize_type", 0).toInt();
    double ratio = settings.value("ratio_value", 1.0).toDouble() * 100.0;
    double smaller = settings.value("smaller_value", 1000.0).toDouble();
    double larger = settings.value("larger_value", 1000.0).toDouble();
    double width = settings.value("width_value", 1000.0).toDouble();
    double height = settings.value("height_value", 1000.0).toDouble();

    bool skip_copy = settings.value("skip_copy", true).toBool();
    int skip_idx = settings.value("skip_mode", 0).toInt();

    if (settings.value("debug_visible", false).toBool()) {
        ui_->debugButton->setChecked(true);
        ui_->debugWidget->setVisible(true);
    }

    if (!geom.isNull())
        restoreGeometry(geom);

    ui_->overwriteBox->setChecked(overwrite);
    ui_->recurseBox->setChecked(recurse);

    ui_->resizeWidget->setCurrentMode(Global::ScaleMode(resize_type));
    ui_->resizeWidget->setValue(Global::Ratio, ratio);
    ui_->resizeWidget->setValue(Global::Smaller, smaller);
    ui_->resizeWidget->setValue(Global::Larger, larger);
    ui_->resizeWidget->setValue(Global::Width, width);
    ui_->resizeWidget->setValue(Global::Height, height);

    ui_->skipConditionBox->setCurrentIndex(skip_idx);
    ui_->copySkippedBox->setChecked(skip_copy);

    resizeModeChanged(ui_->resizeWidget->currentMode());
}

QString Dialog::settingsFilePath(QString filename) const
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).append('/').append(filename);
}

void Dialog::debugClicked(bool checked)
{
    ui_->debugWidget->setVisible(checked);
    resize(minimumSizeHint());
}

#ifdef USE_PRESETS
void Dialog::readPresets()
{
    QSettings settings(settingsFilePath("presets.ini"), QSettings::IniFormat, this);
    QStringList presetKeys = settings.childGroups();
    QString presetName;
    foreach(QString preset, presetKeys) {
        settings.beginGroup(preset);
        presetName = settings.value("name", "").toString();
        ui_->presetComboBox->addItem(presetName, preset);
        //qDebug() << QString("%1: %2").arg(preset).arg(presetName);
        settings.endGroup();
    }
    ui_->presetComboBox->addItem("(New...)");
}

void Dialog::managePresets()
{
    QSettings settings(settingsFilePath("presets.ini"), QSettings::IniFormat, this);
    PresetManagerDialog dlg(&settings, this);
    int ret = dlg.exec();
    if (ret == QDialog::Accepted)
        ;
}
#endif //USE_PRESETS
