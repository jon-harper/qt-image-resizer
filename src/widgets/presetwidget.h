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
#ifndef PRESETWIDGET_H
#define PRESETWIDGET_H

#ifdef USE_PRESETS

#include <QWidget>
#include <QSettings>

class PresetWidgetUi;

class PresetWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString currentPreset READ currentPreset WRITE setCurrentPreset NOTIFY currentPresetChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit PresetWidget(QWidget *parent = 0);
    PresetWidget(const QStringList &items, QWidget *parent = 0);
    ~PresetWidget();

    void addPreset(const QString &ps);
    void insertPreset(const int idx, const QString &ps);

    void addPresets(const QStringList &psItems);

    void removePreset(const QString &ps);
    void removePreset(const int idx);

    int currentIndex() const;
    QString currentPreset() const;

    QStringList presets() const;
    int count() const;

signals:
    void saveButtonClicked();
    void manageButtonClicked();
    void currentIndexChanged(int);
    void currentPresetChanged(QString);

public slots:
    void setCurrentIndex(const int idx);
    void setCurrentPreset(QString ps);

    void manage(QSettings *sets);

protected:
    PresetWidgetUi *ui;
    virtual void init();

protected slots:
    void _combobox_index_changed();
};

#endif //USE_PRESETS

#endif // PRESETWIDGET_H
