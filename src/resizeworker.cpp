#include "resizeworker.h"

#include <atomic>
#include <algorithm>

#include <QImage>
#include <QSize>
#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>

#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QAbstractEventDispatcher>

#include "filters.h"

struct ResizeWorker::Private
{
    std::atomic<bool> running;
    double value = 0.0;
    double blur = 0.0;
    Global::ScaleMode scaleMode = Global::Ratio;
    Global::SkipMode skipMode = Global::SkipNone;

    bool copySkipped = false;
    bool overwrite = false;

    QString src;
    QString dest;
    FileDataList files;
};

ResizeWorker *ResizeWorker::create(const QString &src_path, const QString &dest_path, const FileDataList &data, const ScaleSettings &settings, QThread *parent)
{
    Q_ASSERT(parent);
    auto ret = new ResizeWorker(nullptr);
    ret->d.reset(new Private);
    ret->d->src = src_path;
    ret->d->src.detach();
    ret->d->dest = dest_path;
    ret->d->dest.detach();
    ret->d->files = data;
    ret->d->files.detach();

    ret->d->value = settings.value;
    ret->d->blur = settings.blur;
    ret->d->scaleMode = settings.scale_mode;
    ret->d->skipMode = settings.skip_mode;
    ret->d->copySkipped = settings.copy_skipped;
    ret->d->overwrite = settings.overwrite;
    ret->d->running.store(false, std::memory_order_release);

    ret->moveToThread(parent);
    connect(parent, &QThread::started, ret, &ResizeWorker::start);
    connect(parent, &QThread::finished, ret, &ResizeWorker::finished);

    return ret;
}

bool ResizeWorker::isRunning() const noexcept
{
    return d->running.load(std::memory_order_acquire);
}

void ResizeWorker::start()
{
    if (isRunning())
        return;
    d->running.store(true, std::memory_order_release);

    //insert the separator now so we don't have to do it over and over later
    QString sep = QDir::toNativeSeparators("/");
    if (!d->src.endsWith(sep))
        d->src.append(sep);
    if (!d->dest.endsWith(sep))
        d->dest.append(sep);

    run();
}

void ResizeWorker::stop() noexcept
{
    d->running.store(false, std::memory_order_release);
}

void ResizeWorker::run()
{
    QImageReader reader;
    QImageWriter writer;

    QImage src_img;
    QImage dest_img;

    QSizeF src_sz;
    QSizeF dest_sz;

    QFileInfo fi;

    int quality = 0;

    for(FileData f : d->files) {
        processPendingEvents();
        if (!isRunning())
            break;
        //if overwrite mode is off and the file exists, skip it
        fi.setFile(d->dest + f.first);
        if (!d->overwrite && fi.exists()) {
            emit progress(Progress::Skipped, f.second, QStringLiteral("File already exists."));
            continue;
        } else if (!fi.dir().exists() && !fi.dir().mkpath(fi.path())){
            emit progress(Progress::WriteError, f.second, QStringLiteral("Unable to create destination path."));
        }

        //read in the image
        reader.setFileName(d->src + f.first);
        if (!reader.read(&src_img)) {
            emit progress(Progress::ReadError, f.second, reader.errorString());
            continue;
        }

        quality = reader.quality();

        //determine the new size
        src_sz = src_img.size();
        switch (d->scaleMode) {
        case Global::Ratio:
            dest_sz = QSizeF(src_sz.width() * d->value, src_sz.height() * d->value);
            break;
        case Global::Smaller:
            if (src_sz.width() < src_sz.height())
                dest_sz = QSizeF(d->value, d->value / src_sz.width() * src_sz.height());
            else
                dest_sz = QSizeF(d->value / src_sz.height() * src_sz.width(), d->value);
            break;
        case Global::Larger:
            if (src_sz.width() > src_sz.height())
                dest_sz = QSizeF(d->value, d->value / src_sz.width() * src_sz.height());
            else
                dest_sz = QSizeF(d->value / src_sz.height() * src_sz.width(), d->value);
            break;
        case Global::Width:
            dest_sz = QSizeF(d->value, d->value  / src_sz.width() * src_sz.height());
            break;
        case Global::Height:
            dest_sz = QSizeF(d->value / src_sz.height() * src_sz.width() , d->value);
            break;
        //default:
        }

        //determine if we need to skip this file
        //qt makes copying a file directly painful.
        if (d->scaleMode != Global::Ratio && d->skipMode != Global::SkipNone) {
            if ((d->skipMode == Global::SkipLarger && src_sz.width() > dest_sz.width())
                    || (d->skipMode == Global::SkipSmaller && src_sz.width() < dest_sz.width())) {
                if (d->copySkipped) {
                    QFile tmp(d->src + f.first);
                    if (QFile::exists(d->dest + f.first))
                        if (!QFile::remove(d->dest + f.first))
                            emit progress(Progress::UnknownError, f.second, QStringLiteral("Error overwriting destination file."));
                    if (!tmp.copy(d->dest + f.first))
                        emit progress(Progress::WriteError, f.second, tmp.errorString());
                    else
                        emit progress(Progress::Success, f.second, QStringLiteral("Copied without resizing."));
                } else
                    emit progress(Progress::Skipped, f.second, QStringLiteral(""));
                continue;
            }
        }

        //do the actual scale work
        dest_img = Filters::scale(&src_img, dest_sz.toSize(), d->blur);
        if (dest_img.isNull()) {
            emit progress(Progress::UnknownError, f.second, QStringLiteral("Error resizing image."));
            continue;
        }

        //process events before we commit to writing
        processPendingEvents();
        if (!isRunning())
            break;

        //write
        writer.setFileName(d->dest + f.first);
        writer.setQuality(quality);
        if (!writer.write(dest_img)) {
            emit progress(Progress::WriteError, f.second, QStringLiteral("Cannot write file."));
        } else {
            emit progress(Progress::Success, f.second, QStringLiteral(""));
        }
    }
}

void ResizeWorker::processPendingEvents()
{
    if (thread()->eventDispatcher()->hasPendingEvents())
        thread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);
}
