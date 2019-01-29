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
#ifndef PRESETMANAGERDIALOG_H
#define PRESETMANAGERDIALOG_H

#ifdef USE_PRESETS

#include <QDialog>
#include <QSettings>

class PresetManagerUi;
class QListWidgetItem;
struct PresetData;

class PresetManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PresetManagerDialog(QSettings *settings, QWidget *window = 0);
    ~PresetManagerDialog();

protected:
    void connectSignals();

protected slots:
    void _copy_clicked();
    void _rename_clicked();
    void _delete_clicked();
    void _save_and_close();

private:
    QSettings *_settings;
    PresetManagerUi *ui;
    QMap<QListWidgetItem *, PresetData *> data;
};

#endif //USE_PRESETS

#endif // PRESETMANAGERDIALOG_H
