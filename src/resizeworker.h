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
#ifndef RESIZEWORKER_H
#define RESIZEWORKER_H

#include <QObject>
#include <QScopedPointer>

#include "global.h"

class QThread;

/*! \brief Performs the actual processing of each image to be scaled.
 *
 *  Each instance of a ResizeWorker is constructed and returned from `createInThread`.
 *  A simple call to start() starts the processing. The thread is \em not re-usable and must be
 *  destroyed once it finishes.
 *
 *  Aside from inherited signals, ResizeWorker has a progress signal to indicated it has
 *  finished with a file, successfully or otherwise. The signal's first parameter is an
 *  error code for the receiver.
 */
class ResizeWorker : public QObject
{
    Q_OBJECT

public:
    enum Progress {
        Success = 0,
        Skipped,
        ReadError,
        WriteError,
        NotFound,
        UnknownError
    };
    Q_ENUM(Progress)

    static ResizeWorker *create(const QString &src_path,
                                const QString &dest_path,
                                const FileDataList &data,
                                const ScaleSettings &settings,
                                QThread *parent);
    bool isRunning() const noexcept;

public Q_SLOTS:
    void start();
    void stop() noexcept;

Q_SIGNALS:
    void started();
    void progress(Progress type, int index, QString logString);
    void finished();

protected:
    using QObject::QObject;
    void run();
    void processPendingEvents();

private:
    struct Private;
    QScopedPointer<Private> d;
};
#endif // RESIZEWORKER_H
