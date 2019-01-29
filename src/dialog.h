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
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QElapsedTimer>
#include <QDir>
#include <QScopedPointer>

#include "global.h"
#include "resizeworker.h"

class ResizeDialogUi;
class QCloseEvent;
class QThread;
class QWinTaskbarButton;
class QWinTaskbarProgress;

//! Provides the main dialog for the application.
class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog() override;
    void retranslateUi();

public Q_SLOTS:
    void setProgress(ResizeWorker::Progress reason, int idx, QString msg);

#ifdef USE_PRESETS
    void managePresets();
#endif //USE_PRESETS

protected:
    void closeEvent(QCloseEvent *ev) override;

    void setupUi();
    void createConnections();
    void populateComboBoxes();

    void readSettings();
    void writeSettings();
#ifdef USE_PRESETS
    void readPresets();
    //void writePresets();
#endif //USE_PRESETS
    void startProcessing();
    void showFinishedDialog(qint64 elapsed);
    void setWidgetsEnabled(bool enable);

    QString settingsFilePath(QString filename) const;

    //! Inline shorthand for QDir::toNativeSeparators()
    inline auto toNative(const QString &path)
        { return QDir::toNativeSeparators(path); }
    //! Inline shorthand for QDir::fromNativeSeparators()
    inline auto fromNative(const QString &path)
        { return QDir::fromNativeSeparators(path); }

protected Q_SLOTS:
    void startButtonClicked(bool clicked);
    void stopProcessing();
    void debugClicked(bool checked);
    void resizeModeChanged(Global::ScaleMode mode);

private:
    struct FileCount
    {
        int total = 0;
        int finished = 0;
        int success = 0;
        int skipped = 0;
        int error = 0;
    };

    bool running_;
    QVector<QThread *> threads_;
    QVector<ResizeWorker *> workers_;
    QStringList fileList_;
    QStringList errorStrings_;
    QElapsedTimer timer_;

#ifdef Q_OS_WIN
    QWinTaskbarButton *taskbarButton_;
    QWinTaskbarProgress *taskbarProgress_;
#endif

    FileCount fileCount_;

    QDir::Filters filters_;
    QStringList nameFilters_;

    QScopedPointer<ResizeDialogUi> ui_;
};

#endif // DIALOG_H
