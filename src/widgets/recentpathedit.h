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
#ifndef RECENTPATHEDIT_H
#define RECENTPATHEDIT_H

#include <QWidget>
#include <QIcon>
#include <QString>

class QToolButton;
class QCompleter;
class QComboBox;
class QHBoxLayout;

//! \brief A widget for entering a path via line editor or dialog.
class RecentPathEdit : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QIcon buttonIcon READ buttonIcon WRITE setButtonIcon)
    Q_PROPERTY(QString buttonText READ buttonText WRITE setButtonText)
    Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath)
    Q_PROPERTY(Qt::ToolButtonStyle toolButtonStyle
               READ toolButtonStyle WRITE setToolButtonStyle)

public:
    explicit RecentPathEdit(QWidget *parent = nullptr);

    const QIcon buttonIcon() const;
    const QString buttonText() const;
    const QString currentPath() const;
    Qt::ToolButtonStyle toolButtonStyle() const;
    const QString dialogTitle() const;

    QStringList recentPaths() const;
    int recentPathCount();
    int maxRecentPaths();

    void setRecentPaths(const QStringList &files);
    void setMaxRecentPaths(const int count);

    void setButtonIcon(const QIcon &icon);
    void setButtonText(const QString &text);
    void setToolButtonStyle(const Qt::ToolButtonStyle style);
    void setDialogTitle(const QString &title);

public slots:
    void setCurrentPath(const QString &path);
    void commitCurrentPath();

protected:
    QToolButton *button;
    QComboBox *editor;
    QHBoxLayout *layout;

protected slots:
    void _button_clicked();

private:
    QString dlgTitle;
};

#endif //RECENTPATHEDIT_H
