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
#ifndef FINISHEDDIALOG_H
#define FINISHEDDIALOG_H

#include <QDialog>

class QGridLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;
class QTextEdit;

class FinishedDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString detailedText READ detailedText WRITE setDetailedText)
    Q_PROPERTY(QString informativeText READ informativeText WRITE setInformativeText)

public:
    explicit FinishedDialog(QWidget *parent = nullptr);

    QString text() const;
    void setText(const QString &text);

    QString informativeText() const;
    void setInformativeText(const QString &text);

    QString detailedText() const;
    void setDetailedText(const QString &text);

protected:
    void setupUi();

private:
    QVBoxLayout *layout;
    QVBoxLayout *detailLayout;
    QLabel *iconLabel;
    QLabel *textLabel;
    QLabel *infoLabel;
    QPushButton *okButton;
    QPushButton *detailButton;
    QTextEdit *textEdit;
};

#endif // FINISHEDDIALOG_H
