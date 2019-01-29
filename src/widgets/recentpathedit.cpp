#include "recentpathedit.h"

#include <QDir>

#include <QFileDialog>
#include <QComboBox>
#include <QToolButton>
#include <QHBoxLayout>
#include <QDebug>

//! \brief .
RecentPathEdit::RecentPathEdit(QWidget *parent)
    : QWidget(parent)
{
    //model = new QFileSystemModel(this);
    //model->setRootPath(model->myComputer().toString());
    //model->setFilter(QDir::Drives | QDir::AllDirs | QDir::NoDotAndDotDot);

    //completer = new QCompleter(this);
    //completer->setModel(model);

    //editor = new QLineEdit(this);
    editor = new QComboBox(this);
    editor->setEditable(true);
    editor->setInsertPolicy(QComboBox::InsertAtTop);
    editor->setDuplicatesEnabled(false);
    //editor->setCompleter(completer);
    editor->setMaxVisibleItems(5);

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
const QIcon RecentPathEdit::buttonIcon() const
{
    return button->icon();
}

//! \brief .
const QString RecentPathEdit::buttonText() const
{
    return button->text();
}

//! \brief .
const QString RecentPathEdit::currentPath() const
{
    return QDir::toNativeSeparators(editor->currentText());
}

//! \brief .
const QString RecentPathEdit::dialogTitle() const
{
    return dlgTitle;
}

//! \brief .
Qt::ToolButtonStyle RecentPathEdit::toolButtonStyle() const
{
    return button->toolButtonStyle();
}

//! \brief .
void RecentPathEdit::setButtonIcon(const QIcon &icon)
{
    button->setIcon(icon);
}

//! \brief .
void RecentPathEdit::setButtonText(const QString &text)
{
    button->setText(text);
}

//! \brief .
void RecentPathEdit::setToolButtonStyle(const Qt::ToolButtonStyle style)
{
    button->setToolButtonStyle(style);
}

//! \brief .
void RecentPathEdit::setCurrentPath(const QString &path)
{
    if (QDir(path) == QDir(editor->currentText()))
        return;
    editor->setEditText(QDir::toNativeSeparators(path));
}

//! \brief .
void RecentPathEdit::setDialogTitle(const QString &title)
{
    dlgTitle = title;
}

//! \brief .
void RecentPathEdit::_button_clicked()
{
    QString path;
    path = QFileDialog::getExistingDirectory(this, dlgTitle, editor->currentText());
    if (!path.isEmpty())
        editor->setEditText(QDir::toNativeSeparators(path));
}
/*! \var RecentPathEdit::button
 *  \brief .
 */

/*! \var RecentPathEdit::editor
 *  \brief .
 */

/*! \var RecentPathEdit::layout
 *  \brief .
 */

/*! \var RecentPathEdit::completer
 *  \brief .
 */
void RecentPathEdit::setRecentPaths(const QStringList &files)
{
    editor->clear();
    editor->addItems(files);
}

void RecentPathEdit::setMaxRecentPaths(const int count)
{
    editor->setMaxVisibleItems(count);
}

QStringList RecentPathEdit::recentPaths() const
{
    QStringList files;
    for(int i = 0; i < editor->count(); ++i) {
        files.append(editor->itemText(i));
    }
    return files;
}

int RecentPathEdit::recentPathCount()
{
    return editor->count();
}

int RecentPathEdit::maxRecentPaths()
{
    return editor->maxVisibleItems();
}

void RecentPathEdit::commitCurrentPath()
{
    int idx = editor->findText(editor->currentText());
    if (idx == -1) {
        if (editor->count() == 5)
            editor->removeItem(4);
        editor->insertItem(0, editor->currentText());
        editor->setCurrentIndex(0);
    } else if (idx != 0) {
        QString text = editor->currentText();
        editor->removeItem(idx);
        editor->insertItem(0, text);
        editor->setCurrentIndex(0);
    }
}
