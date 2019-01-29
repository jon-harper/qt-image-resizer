/*  qt-image-resizer
 *  Copyright (C) 2016  Jonathan Harper
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef RESIZESETTINGSWIDGET_H
#define RESIZESETTINGSWIDGET_H

#include <QWidget>

#include "../global.h"

class QComboBox;
class QStackedWidget;
class QDoubleSpinBox;

/*! \brief Widget for selecting image scale mode and accompanying value.
 *
 *  Each ScaleMode is represented in a QComboBox and paired with a QDoubleSpinBox.
 *  When the user changes the combo box index, a QStackedWidget switches to the appropriate
 *  spin box.
 */
class ResizeSettingsWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(Global::ScaleMode currentMode READ currentMode WRITE setCurrentMode)
    Q_PROPERTY(double currentValue READ currentValue WRITE setCurrentValue)

signals:
    void currentModeChanged(Global::ScaleMode);

public:
    explicit ResizeSettingsWidget(QWidget *parent = 0);
    virtual void retranslateUi();

    QDoubleSpinBox *spinBox(Global::ScaleMode mode);

    Global::ScaleMode currentMode();
    double currentValue();
    double value(Global::ScaleMode mode);
    void setRange(Global::ScaleMode mode, double min, double max);

public slots:
    void setCurrentValue(double value);
    void setCurrentMode(Global::ScaleMode mode);
    void setValue(Global::ScaleMode mode, double value);

protected slots:
    void _mode_changed();

protected:
    QComboBox *selectionBox;
    QStackedWidget *stack;

    virtual void setupUi();
    QDoubleSpinBox *boxForMode(Global::ScaleMode mode);
    QDoubleSpinBox *currentBox();
};

#endif // RESIZESETTINGSWIDGET_H
