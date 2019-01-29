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
#ifndef UTIL_H
#define UTIL_H

#include <QImage>
#include <QDir>
#include <QDirIterator>

/*! \namespace Util
    \brief Contains miscellaneous utility functions.
  */
namespace Util {

    QStringList fileList(const QDir &src, const QStringList &name_filters, QDir::Filters filters, QDirIterator::IteratorFlags flags);
    QString elapsedSecondsToString(unsigned long time);
    QString millisecondsToString(qint64 ms);

    template<typename T>
    QString msToString(T ms);

    QString readableBytes(qint64 bytes, bool si = false);
    QString imageTypeToString(QImage::Format f);
}

template<typename T>
QString Util::msToString(T ms)
{
    QString ret;
    ms /= 1000; //convert to seconds
    if (ms >= 86400) { //day
        ret += QString("%1d ").arg(QString::number(ms / 86400));
        ms %= 86400;
    }
    if (ms >= 3600) { //hour
        ret += QString("%1h ").arg(QString::number(ms / 3600));
        ms %= 3600;
    }
    if (ms >= 60) { //minute
        ret += QString("%1m ").arg(QString::number(ms / 60));
        ms %= 60;
    } else
        ret += QString("0m ");
    ret += QString("%1s").arg(ms);
    return ret;
}

#endif // UTIL_H
