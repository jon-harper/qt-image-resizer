#include "presetwidget.h"

#ifdef USE_PRESETS

#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSizePolicy>

#include "presetmanagerdialog.h"
#include "qdummysettings.h"

class PresetWidgetUi
{
public:
    explicit PresetWidgetUi(QWidget *parent = 0)
    {
        layout = new QHBoxLayout(parent);
        layout->setContentsMargins(0,0,0,0);

        comboBox = new QComboBox();
        saveButton = new QPushButton("Sa&ve");
        saveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        manageButton = new QPushButton("&Manage...");
        manageButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        layout->addWidget(comboBox);
        layout->addWidget(saveButton);
        layout->addWidget(manageButton);
        layout->addStretch(1);
    }

    QHBoxLayout *layout;
    QPushButton *saveButton;
    QPushButton *manageButton;
    QComboBox *comboBox;
};

PresetWidget::PresetWidget(QWidget *parent) :
    QWidget(parent), ui(0)
{
    init();
}

PresetWidget::PresetWidget(const QStringList &items, QWidget *parent) :
    QWidget(parent), ui(0)
{
    init();
    addPresets(items);
}

PresetWidget::~PresetWidget()
{
    delete ui;
}

void PresetWidget::addPresets(const QStringList &psItems)
{
    foreach(QString ps, psItems)
        addPreset(ps);
}

void PresetWidget::addPreset(const QString &ps)
{
    insertPreset(-1, ps);
}

void PresetWidget::insertPreset(const int idx, const QString &ps)
{
    if (idx < -1)
        return;
    else if (idx == -1)
        ui->comboBox->addItem(ps);
    else
        ui->comboBox->insertItem(idx, ps);
}

void PresetWidget::removePreset(const QString &ps)
{
    int idx = ui->comboBox->findText(ps);
    if (idx != -1)
        removePreset(idx);
}

void PresetWidget::removePreset(const int idx)
{
    if (idx >= 0 && idx < ui->comboBox->count())
        ui->comboBox->removeItem(idx);
}

int PresetWidget::currentIndex() const
{
    return ui->comboBox->currentIndex();
}

void PresetWidget::setCurrentIndex(const int idx)
{
    if (idx >= 0)
        ui->comboBox->setCurrentIndex(idx);
}

QString PresetWidget::currentPreset() const
{
    return ui->comboBox->currentText();
}

void PresetWidget::setCurrentPreset(QString ps)
{
    int idx = ui->comboBox->findText(ps);
    setCurrentIndex(idx);
}

QStringList PresetWidget::presets() const
{
    QStringList ret;
    for(int i = 0; i < ui->comboBox->count(); ++i)
        ret.append(ui->comboBox->itemText(i));
    return ret;
}

int PresetWidget::count() const
{
    return ui->comboBox->count();
}

void PresetWidget::_combobox_index_changed()
{
    emit currentIndexChanged(ui->comboBox->currentIndex());
    emit currentPresetChanged(ui->comboBox->currentText());
}

void PresetWidget::init()
{
    ui = new PresetWidgetUi(this);

    connect(ui->saveButton, SIGNAL(clicked()), this, SIGNAL(saveButtonClicked()));
    connect(ui->manageButton, SIGNAL(clicked()), this, SIGNAL(manageButtonClicked()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(_combobox_index_changed()));
}

void PresetWidget::manage(QSettings *sets)
{
    QWidget *w = this->window();
    PresetManagerDialog dlg(sets, w);
    int ret = dlg.exec();
}

#endif //USE_PRESETS
