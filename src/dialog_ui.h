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
#ifndef DIALOG_UI_H
#define DIALOG_UI_H

//widgets
#include <QFrame>
#include <QTreeWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QProgressBar>

//layouts
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "widgets/resizesettingswidget.h"
#include "widgets/recentpathedit.h"
#include "widgets/debugbox.h"

#ifdef USE_PRESETS
//#include "widgets/presetwidget.h"
#include "widgets/presetmanagerdialog.h"
#endif

//! Stores the widgets for the dialog
class ResizeDialogUi
{
public:
    ResizeDialogUi()
        : layout(new QVBoxLayout)
        , topLayout(new QGridLayout)
        , filesLayout(new QFormLayout)
        , resizeLayout(new QFormLayout)
        , advancedLayout(new QFormLayout)
        , buttonLayout(new QHBoxLayout)
        , filesGroupBox(new QGroupBox)
        , srcEdit(new RecentPathEdit)
        , destEdit(new RecentPathEdit)
        , overwriteBox(new QCheckBox)
        , recurseBox(new QCheckBox)
        , resizeGroupBox(new QGroupBox)
        , resizeWidget(new ResizeSettingsWidget)
        , skipConditionBox(new QComboBox)
        , copySkippedBox(new QCheckBox)
        , advancedBox(new QGroupBox)
        , blurBox(new QDoubleSpinBox)
        , progressBar(new QProgressBar)
        , debugWidget(new DebugBox)
        , startButton(new QPushButton)
        , debugButton(new QPushButton)
        , closeButton(new QPushButton)
        , srcLabel(new QLabel)
        , destLabel(new QLabel)
        , resizeLabel(new QLabel)
        , blurLabel(new QLabel)
        , priorityLabel(new QLabel)
        , skipConditionLabel(new QLabel)
    {

    }

    QVBoxLayout *layout;
    QGridLayout *topLayout;
    QFormLayout *filesLayout;
    QFormLayout *resizeLayout;
    QFormLayout *advancedLayout;
    QHBoxLayout *buttonLayout;

#ifdef USE_PRESETS
    QGroupBox *presetGroupBox;
    QComboBox *presetComboBox;
    QPushButton *presetManageBtn;
#endif

    //widgets
    QGroupBox *filesGroupBox;
    RecentPathEdit *srcEdit;
    RecentPathEdit *destEdit;

    QCheckBox *overwriteBox;
    QCheckBox *recurseBox;

    QGroupBox *resizeGroupBox;
    ResizeSettingsWidget *resizeWidget;
    QComboBox *skipConditionBox;
    QCheckBox *copySkippedBox;

    QGroupBox *advancedBox;
    QDoubleSpinBox *blurBox;

    QProgressBar *progressBar;
    DebugBox *debugWidget;

    QPushButton *startButton;
    QPushButton *debugButton;
    QPushButton *closeButton;

    //Labels
    QLabel *srcLabel;
    QLabel *destLabel;
    QLabel *resizeLabel;
    QLabel *blurLabel;
    QLabel *priorityLabel;
    QLabel *skipConditionLabel;
};

#endif // DIALOG_UI_H
