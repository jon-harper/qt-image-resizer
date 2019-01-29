#ifndef PATHLINEEDIT_H
#define PATHLINEEDIT_H

#include <QWidget>
#include <QToolButton>
#include <QIcon>
#include <QString>

class QCompleter;
class QLineEdit;
class QDir;
class QHBoxLayout;
class QFileSystemModel;

//! \brief A widget for entering a path via line editor or dialog.
class PathLineEdit : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QIcon buttonIcon READ buttonIcon WRITE setButtonIcon)
    Q_PROPERTY(QString buttonText READ buttonText WRITE setButtonText)
    Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath)
    Q_PROPERTY(Qt::ToolButtonStyle toolButtonStyle
               READ toolButtonStyle WRITE setToolButtonStyle)

public:
    explicit PathLineEdit(QWidget *parent = 0);

    QIcon buttonIcon() const;
    QString buttonText() const;
    QString currentPath() const;
    Qt::ToolButtonStyle toolButtonStyle() const;
    QString dialogTitle() const;

    void setButtonIcon(const QIcon &icon);
    void setButtonText(const QString &text);
    void setToolButtonStyle(const Qt::ToolButtonStyle style);
    void setDialogTitle(const QString &title);

public slots:
    void setCurrentPath(const QString &path);

protected:
    QToolButton *button;
    QLineEdit *editor;
    QHBoxLayout *layout;
    QCompleter *completer;
    QFileSystemModel *model;

protected slots:
    void _button_clicked();

private:
    QString dlgTitle;
};

#endif //PATHLINEEDIT_H
