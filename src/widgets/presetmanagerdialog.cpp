#include "presetmanagerdialog.h"

#ifdef USE_PRESETS

#include <QObject>

#include <QListWidget>
#include <QListWidgetItem>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSizePolicy>

#include <QSettings>
#include <QtDebug>

struct PresetData
{
    PresetData() {}
    PresetData(const PresetData *other)
        : name(other->name),
          sourceName(other->sourceName) {}
    QString name;
    QString sourceName;
};

class PresetManagerUi
{
public:
    explicit PresetManagerUi(PresetManagerDialog *parent);

    QVBoxLayout *v_layout;
    QHBoxLayout *h_layout;
    QPushButton *copy_btn;
    QPushButton *rename_btn;
    QPushButton *delete_btn;
    QListWidget *list_widget;
    QDialogButtonBox *btn_box;
};

PresetManagerUi::PresetManagerUi(PresetManagerDialog *parent)
{
    v_layout = new QVBoxLayout(parent);

    h_layout = new QHBoxLayout;
    h_layout->setContentsMargins(0,0,0,0);

    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    copy_btn = new QPushButton("Co&py");
    copy_btn->setSizePolicy(policy);
    rename_btn = new QPushButton("&Rename");
    rename_btn->setSizePolicy(policy);
    delete_btn = new QPushButton("&Delete");
    delete_btn->setSizePolicy(policy);
    h_layout->addWidget(copy_btn);
    h_layout->addWidget(rename_btn);
    h_layout->addWidget(delete_btn);
    h_layout->addStretch(1);

    list_widget = new QListWidget;

    btn_box = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Discard,
                                   Qt::Horizontal);

    v_layout->addLayout(h_layout);
    v_layout->addWidget(list_widget);
    v_layout->addWidget(btn_box);
}


PresetManagerDialog::PresetManagerDialog(QSettings *settings, QWidget *window) :
    QDialog(window), _settings(settings)
{
    setWindowTitle("Manage Presets");
    ui = new PresetManagerUi(this);
    connectSignals();

    QStringList presetKeys = settings->childGroups();
    QString presetName;
    QListWidgetItem *item;
    PresetData *itemdata;

    foreach(QString preset, presetKeys) {
        settings->beginGroup(preset);
        presetName = settings->value("name", "").toString();
        item = new QListWidgetItem(presetName, ui->list_widget);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        itemdata = new PresetData;
        itemdata->name = presetName;
        itemdata->sourceName = presetName;

        data.insert(item, itemdata);

        qDebug() << QString("%1: %2").arg(preset).arg(presetName);
        settings->endGroup();
    }
}

PresetManagerDialog::~PresetManagerDialog()
{
    QList<QListWidgetItem *> keys = data.keys();
    QList<PresetData *> values = data.values();
    data.clear();

    foreach(QListWidgetItem *i, keys) {
        if (!i->listWidget())
            delete i;
    }

    qDeleteAll(values);

    delete ui;
}

void PresetManagerDialog::connectSignals()
{
    connect(ui->copy_btn, SIGNAL(clicked()), this, SLOT(_copy_clicked()));
    connect(ui->rename_btn, SIGNAL(clicked()), this, SLOT(_rename_clicked()));
    connect(ui->delete_btn, SIGNAL(clicked()), this, SLOT(_delete_clicked()));
    QPushButton *btn = ui->btn_box->button(QDialogButtonBox::Save);
    connect(btn, SIGNAL(clicked()), this, SLOT(_save_and_close()));
    btn = ui->btn_box->button(QDialogButtonBox::Discard);
    connect(btn, SIGNAL(clicked()), this, SLOT(reject()));
}

void PresetManagerDialog::_copy_clicked()
{
    QListWidgetItem *cur_item = ui->list_widget->currentItem();
    PresetData *cur_data = data[cur_item];
    if (!cur_item || !cur_data)
        return;
    QListWidgetItem *new_item = cur_item->clone();
    PresetData *new_data = new PresetData(cur_data);
    data.insert(new_item, new_data);

    QString txt = new_item->text();
    txt.append(" (copy)");
    new_item->setText(txt);
    new_data->name = txt;
    //take from source name in case this is a copy of a copy
    new_data->sourceName = cur_data->sourceName;

    ui->list_widget->addItem(new_item);
    ui->list_widget->editItem(new_item);
}

void PresetManagerDialog::_rename_clicked()
{
    QListWidgetItem *item = ui->list_widget->currentItem();
    if (item)
        ui->list_widget->editItem(item);
}

void PresetManagerDialog::_delete_clicked()
{
    QListWidgetItem *item = ui->list_widget->currentItem();
    if (!item)
        return;
    ui->list_widget->takeItem(ui->list_widget->row(item));
}

void PresetManagerDialog::_save_and_close()
{
    PresetData *d;
    foreach(QListWidgetItem *i, data.keys()) {
        d = data[i];
        if (!i->listWidget()) {
            qDebug() << QString("Deleted: %1 (%2)").arg(i->text(), d->name);
        } else if (d->name != d->sourceName) {
            qDebug() << QString("Copied: %1 (%2)").arg(i->text(), d->sourceName);
        } else if (i->text() != d->name) {
            qDebug() << QString("Renamed: %1 (%2)").arg(i->text(), d->name);
        }
    }

    accept();
}
#endif //USE_PRESETS
