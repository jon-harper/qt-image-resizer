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
#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QObject>
#include <QVector>

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#include <Windows.h>
#if WINVER >= 0x0600 && !defined(WIN_OS_EXTENSIONS)
#define WIN_OS_EXTENSIONS
//#define WIN_OS_CUSTOM_EXTENSIONS
#endif
#endif

using FileData = QPair<QString, int>;
using FileDataList = QVector<FileData>;

//! Global defines common enums and gives them the benefit of being members of a QObject.
class Global : public QObject
{
    Q_OBJECT
    using QObject::QObject;
public:
    //! The method of calculating the final size of each image.
    enum ScaleMode {
        Ratio = 0,  //! Scale to a percentage of original size
        Smaller,    //! Set the smaller edge to a fixed size
        Larger,     //! Set the larger edge to a fixed size
        Width,      //! Set the width to a fixed size
        Height      //! Set the height to a fixed size
    };
    Q_ENUM(ScaleMode)

    //! Determines under what conditions images are skipped. Does not work with ScaleMode::Ratio
    enum SkipMode {
        SkipNone,   //! Do not skip images
        SkipLarger, //! Skip images larger than the scaled size
        SkipSmaller //! Skip images smaller than the scaled size
    };
    Q_ENUM(SkipMode)
};

//! \brief POD structure to contain data passed to ResizeThread during construction.
struct ScaleSettings
{
    double value = 0.0;
    double blur = 1.0;
    Global::ScaleMode scale_mode = Global::Ratio;
    Global::SkipMode skip_mode = Global::SkipNone;
    bool copy_skipped = true;
    bool overwrite = false;
};

#endif // GLOBAL_H
