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
#ifndef SEPARATORWIDGET_H
#define SEPARATORWIDGET_H

#include <QFrame>

class SeparatorWidget : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(int thickness READ thickness WRITE setThickness)
    Q_PROPERTY(Qt::Orientation direction READ direction WRITE setDirection)
public:
    explicit SeparatorWidget(Qt::Orientation orient = Qt::Horizontal, QWidget *parent = nullptr);

    int thickness() const;
    Qt::Orientation direction() const;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void setThickness(int size);
    void setDirection(Qt::Orientation dir);

private:
    int m_thickness;
    Qt::Orientation m_direction;
};

#endif // SEPARATORWIDGET_H
