#include "separatorwidget.h"

SeparatorWidget::SeparatorWidget(Qt::Orientation orient, QWidget *parent) :
    QFrame(parent), m_thickness(2), m_direction(orient)
{
}

QSize SeparatorWidget::sizeHint() const
{
    return minimumSizeHint();
}

QSize SeparatorWidget::minimumSizeHint() const
{    
    return QSize(m_thickness, m_thickness);
}

void SeparatorWidget::setThickness(int size)
{
    if (size < 2)
        return;
    m_thickness = size;
    updateGeometry();
}

void SeparatorWidget::setDirection(Qt::Orientation dir)
{
    if (dir == m_direction)
        return;
    m_direction = dir;
    if (dir == Qt::Horizontal)
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    else
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    updateGeometry();
}

int SeparatorWidget::thickness() const
{
    return m_thickness;
}
Qt::Orientation SeparatorWidget::direction() const
{
    return m_direction;
}
