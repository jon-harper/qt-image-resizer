#include "util.h"

#include <cmath>

QStringList Util::fileList(const QDir &src, const QStringList &name_filters, QDir::Filters filters, QDirIterator::IteratorFlags flags)
{
    QDirIterator iter(src.absolutePath(), name_filters, filters, flags);
    QString next;
    QStringList filenames;

    while (iter.hasNext()) {
        next = iter.next();
        if (!iter.fileInfo().isDir())
            filenames.append(src.relativeFilePath(next));
        if (filenames.count() > 10000)
            break;
    }
    return filenames;
}

QString Util::elapsedSecondsToString(unsigned long time)
{
    QString ret;
    if (time >= 86400) { //day
        ret += QString("%1d ").arg(time / 86400);
        time %= 86400;
    }
    if (time >= 3600) { //hour
        ret += QString("%1h ").arg(time / 3600);
        time %= 3600;
    }
    if (time >= 60) { //minute
        ret += QString("%1m ").arg(time / 60);
        time %= 60;
    } else
        ret += QString("0m ");
    ret += QString("%1s").arg(time);
    return ret;
}

QString Util::readableBytes(qint64 bytes, bool si)
{
    Q_ASSERT(bytes >= 0);
    if (bytes < 1024)
        return QString("%1B").arg(bytes);

    // Static lookup table of byte-based SI units
    const char suffix[6] = {'k','M','G','T','P','E'};
    //return value
    QString ret;
    //exponent for pow(1024, exp) and suffix index
    int exp = int(log(double(bytes)) / log(1024.0));
    //value to divide bytes by
    qint64 divisor = static_cast<qint64>(pow(1024.0, exp));

    ret = QString("%3%1%2B").arg(suffix[exp-1]).arg(si ? "i" : "");

    ret = ret.arg(double(bytes)/double(divisor),0,'f',1);
    return ret;
}

QString Util::imageTypeToString(QImage::Format f)
{
    QString ret;
    switch(f) {
    case QImage::Format_Mono:
        ret = "Mono";
        break;
    case QImage::Format_MonoLSB:
        ret = "MonoLSB";
        break;
    case QImage::Format_Indexed8:
        ret = "Indexed8";
        break;
    case QImage::Format_RGB32:
        ret = "RGB32";
        break;
    case QImage::Format_ARGB32:
        ret = "ARGB32";
        break;
    case QImage::Format_ARGB32_Premultiplied:
        ret = "ARGB32_Premultiplied";
        break;
    case QImage::Format_RGB16:
        ret = "RGB16";
        break;
    case QImage::Format_ARGB8565_Premultiplied:
        ret = "ARGB8565_Premultiplied";
        break;
    case QImage::Format_RGB666:
        ret = "RGB666";
        break;
    case QImage::Format_ARGB6666_Premultiplied:
        ret = "ARGB6666_Premultiplied";
        break;
    case QImage::Format_RGB555:
        ret = "RGB555";
        break;
    case QImage::Format_ARGB8555_Premultiplied:
        ret = "ARGB8555_Premultiplied";
        break;
    case QImage::Format_RGB888:
        ret = "RGB888";
        break;
    case QImage::Format_RGB444:
        ret = "RGB444";
        break;
    case QImage::Format_ARGB4444_Premultiplied:
        ret = "ARGB4444_Premultiplied";
        break;
    case QImage::Format_Invalid:
    default:
        ret = "Invalid";
        break;
    }
    return ret;
}

QString Util::millisecondsToString(qint64 ms)
{
    QString ret;
    ms /= 1000; //convert to seconds
    if (ms >= 86400) { //day
        ret += QString("%1d ").arg(ms / 86400);
        ms %= 86400;
    }
    if (ms >= 3600) { //hour
        ret += QString("%1h ").arg(ms / 3600);
        ms %= 3600;
    }
    if (ms >= 60) { //minute
        ret += QString("%1m ").arg(ms / 60);
        ms %= 60;
    } else
        ret += QString("0m ");
    ret += QString("%1s").arg(ms);
    return ret;
}
