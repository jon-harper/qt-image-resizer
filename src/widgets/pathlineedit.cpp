#include "pathlineedit.h"

#include <QDir>

#include <QFileDialog>
#include <QLineEdit>
#include <QFileSystemModel>
#include <QCompleter>
#include <QHBoxLayout>

//! \brief .
PathLineEdit::PathLineEdit(QWidget *parent)
    : QWidget(parent)
{
    model = new QFileSystemModel(this);
    model->setRootPath(model->myComputer().toString());
    model->setFilter(QDir::Drives | QDir::AllDirs | QDir::NoDotAndDotDot);

    completer = new QCompleter(this);
    completer->setModel(model);

    editor = new QLineEdit(this);
    editor->setCompleter(completer);

    button = new QToolButton(this);
    button->setText(QString::fromUtf8("..."));
    button->setAutoRaise(false);
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->addWidget(editor);
    layout->addWidget(button);

    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(editor);

    connect(button, SIGNAL(clicked()), this, SLOT(_button_clicked()));
}

//! \brief .
QIcon PathLineEdit::buttonIcon() const
{
    return button->icon();
}

//! \brief .
QString PathLineEdit::buttonText() const
{
    return button->text();
}

//! \brief .
QString PathLineEdit::currentPath() const
{
    return QDir::toNativeSeparators(editor->text());
}

//! \brief .
QString PathLineEdit::dialogTitle() const
{
    return dlgTitle;
}

//! \brief .
Qt::ToolButtonStyle PathLineEdit::toolButtonStyle() const
{
    return button->toolButtonStyle();
}

//! \brief .
void PathLineEdit::setButtonIcon(const QIcon &icon)
{
    button->setIcon(icon);
}

//! \brief .
void PathLineEdit::setButtonText(const QString &text)
{
    button->setText(text);
}

//! \brief .
void PathLineEdit::setToolButtonStyle(const Qt::ToolButtonStyle style)
{
    button->setToolButtonStyle(style);
}

//! \brief .
void PathLineEdit::setCurrentPath(const QString &path)
{
    if (QDir(path) == QDir(editor->text()))
        return;
    editor->setText(QDir::toNativeSeparators(path));
}

//! \brief .
void PathLineEdit::setDialogTitle(const QString &title)
{
    dlgTitle = title;
}

//! \brief .
void PathLineEdit::_button_clicked()
{
    QString path;
    path = QFileDialog::getExistingDirectory(this, dlgTitle, editor->text());
    if (!path.isEmpty())
        editor->setText(QDir::toNativeSeparators(path));
}
/*! \var PathLineEdit::button
 *  \brief .
 */

/*! \var PathLineEdit::editor
 *  \brief .
 */

/*! \var PathLineEdit::layout
 *  \brief .
 */

/*! \var PathLineEdit::completer
 *  \brief .
 */
