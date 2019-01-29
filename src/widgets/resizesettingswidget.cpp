#include "resizesettingswidget.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QStackedWidget>
#include <QDoubleSpinBox>

//! \brief .
ResizeSettingsWidget::ResizeSettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi();
    retranslateUi();
}

//! \brief .
void ResizeSettingsWidget::setupUi()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    selectionBox = new QComboBox();
    selectionBox->addItem("", QVariant((int)Global::Ratio));
    selectionBox->addItem("", QVariant((int)Global::Smaller));
    selectionBox->addItem("", QVariant((int)Global::Larger));
    selectionBox->addItem("", QVariant((int)Global::Width));
    selectionBox->addItem("", QVariant((int)Global::Height));

    stack = new QStackedWidget();

    QDoubleSpinBox *ratioBox = new QDoubleSpinBox();
    ratioBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ratioBox->setRange(5.0, 200.0);
    ratioBox->setValue(100.0);
    ratioBox->setDecimals(0);

    QDoubleSpinBox *smallerBox = new QDoubleSpinBox();
    smallerBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    smallerBox->setRange(0.0, 32000.0);
    smallerBox->setValue(1000.0);
    smallerBox->setDecimals(0);

    QDoubleSpinBox *largerBox = new QDoubleSpinBox();
    largerBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    largerBox->setRange(0.0, 32000.0);
    largerBox->setValue(1000.0);
    largerBox->setDecimals(0);

    QDoubleSpinBox *widthBox = new QDoubleSpinBox();
    widthBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    widthBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    widthBox->setRange(0.0, 32000.0);
    widthBox->setValue(1000.0);
    widthBox->setDecimals(0);

    QDoubleSpinBox *heightBox = new QDoubleSpinBox();
    heightBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    heightBox->setRange(0.0, 32000.0);
    heightBox->setValue(1000.0);
    heightBox->setDecimals(0);

    stack->addWidget(ratioBox);
    stack->addWidget(smallerBox);
    stack->addWidget(largerBox);
    stack->addWidget(widthBox);
    stack->addWidget(heightBox);

    layout->addWidget(selectionBox);
    layout->addWidget(stack);

    connect(selectionBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(_mode_changed()));
}

//! \brief .
void ResizeSettingsWidget::retranslateUi()
{

    selectionBox->setToolTip(trUtf8("<p>Sets the method and size for resizing images. Aspect ratio will be preserved.</p><p><span style=\" font-weight:600;\">Ratio</span>: Scale as a percentage of the original size<br/><span style=\"font-weight:600;\">Height, Width</span>: Scales the image based on a fixed size<br/><span style=\" font-weight:600;\">Smaller, Larger</span>: Same as height or width, except chooses the smaller or larger edge of each image processed."));
    selectionBox->setItemText((int)Global::Ratio, tr("Ratio"));
    selectionBox->setItemText((int)Global::Smaller, tr("Smaller"));
    selectionBox->setItemText((int)Global::Larger, tr("Larger"));
    selectionBox->setItemText((int)Global::Width, tr("Width"));
    selectionBox->setItemText((int)Global::Height, tr("Height"));

    boxForMode(Global::Ratio)->setSuffix(tr(" %"));
    boxForMode(Global::Smaller)->setSuffix(tr(" px"));
    boxForMode(Global::Larger)->setSuffix(tr(" px"));
    boxForMode(Global::Width)->setSuffix(tr(" px"));
    boxForMode(Global::Height)->setSuffix(tr(" px"));
}

//! \brief .
QDoubleSpinBox *ResizeSettingsWidget::spinBox(Global::ScaleMode mode)
{
    return boxForMode(mode);
}

//! \brief .
Global::ScaleMode ResizeSettingsWidget::currentMode()
{
    return Global::ScaleMode(selectionBox->itemData(selectionBox->currentIndex()).toInt());
}

//! \brief .
double ResizeSettingsWidget::currentValue()
{
    QDoubleSpinBox *box;
    box = qobject_cast<QDoubleSpinBox*>(stack->currentWidget());
    Q_ASSERT(box); //be safe
    if (currentMode() == Global::Ratio)
        return box->value() / 100.0;
    else
        return box->value();
}

//! \brief .
double ResizeSettingsWidget::value(Global::ScaleMode mode)
{
    if (mode == Global::Ratio)
        return boxForMode(mode)->value() / 100.0;
    else
        return boxForMode(mode)->value();
}

//! \brief .
void ResizeSettingsWidget::setRange(Global::ScaleMode mode, double min, double max)
{
    boxForMode(mode)->setRange(min, max);
}

//! \brief .
void ResizeSettingsWidget::setCurrentValue(double value)
{
    QDoubleSpinBox *box = currentBox();
    box->setValue(value);
}

//! \brief .
void ResizeSettingsWidget::setCurrentMode(Global::ScaleMode mode)
{
    if (mode == currentMode())
        return;
    selectionBox->setCurrentIndex(int(mode));
    stack->setCurrentIndex(int(mode));
}

//! \brief .
void ResizeSettingsWidget::setValue(Global::ScaleMode mode, double value)
{
    boxForMode(mode)->setValue(value);
}

//! \brief .
QDoubleSpinBox *ResizeSettingsWidget::boxForMode(Global::ScaleMode mode)
{
    QDoubleSpinBox *box;
    box = qobject_cast<QDoubleSpinBox*>(stack->widget((int)mode));
    Q_ASSERT(box); //be safe
    return box;
}

//! \brief .
QDoubleSpinBox *ResizeSettingsWidget::currentBox()
{
    QDoubleSpinBox *box;
    box = qobject_cast<QDoubleSpinBox*>(stack->currentWidget());
    Q_ASSERT(box); //be safe
    return box;
}

//! \brief .
void ResizeSettingsWidget::_mode_changed()
{
    Global::ScaleMode mode = currentMode();
    stack->setCurrentIndex(int(mode));
    emit currentModeChanged(mode);
}


/*! \property ResizeSettingsWidget::currentMode
  * \brief This property holds the current ScaleMode.
  *
  */
