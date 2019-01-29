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
#ifndef DEBUGBOX_H
#define DEBUGBOX_H

#include <QTextEdit>

//! \brief Output for debug information.
class DebugBox : public QTextEdit
{
    Q_OBJECT
public:
    DebugBox(const QString &text, QWidget *parent = nullptr);
    DebugBox(QWidget *parent = nullptr);
    ~DebugBox();
    
public slots:
    void postMessage(QtMsgType type, const char *msg);
};

#endif // DEBUGBOX_H
