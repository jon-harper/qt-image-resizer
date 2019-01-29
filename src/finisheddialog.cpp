#include "finisheddialog.h"

#include "widgets/separatorwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

#include <QStyle>

FinishedDialog::FinishedDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
}

void FinishedDialog::setupUi()
{
    layout = new QVBoxLayout(this);

    QHBoxLayout *midLayout = new QHBoxLayout;

    iconLabel = new QLabel;
    iconLabel->setPixmap(style()->standardPixmap(QStyle::SP_MessageBoxInformation));

    textLabel = new QLabel;
    textLabel->setText("Elapsed:\nProcessed:\nCompleted:\nSkipped:\nFailed:");
    infoLabel = new QLabel;

    midLayout->addWidget(iconLabel);
    midLayout->addWidget(textLabel);
    midLayout->addWidget(infoLabel);

    layout->addLayout(midLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    okButton = new QPushButton("OK");
    detailButton = new QPushButton("Details");
    detailButton->setCheckable(true);
    buttonLayout->addWidget(detailButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(okButton);

    detailLayout = new QVBoxLayout;
    SeparatorWidget *sep = new SeparatorWidget;

    textEdit = new QTextEdit;
    textEdit->setReadOnly(true);
    textEdit->setAcceptRichText(true);
    textEdit->setFrameShape(QFrame::StyledPanel);
    textEdit->setFrameShadow(QFrame::Sunken);
    detailLayout->addWidget(sep);
    detailLayout->addWidget(textEdit, 1);
}

QString FinishedDialog::text() const
{
    return textLabel->text();
}
void FinishedDialog::setText(const QString &text)
{
    textLabel->setText(text);
}

QString FinishedDialog::informativeText() const
{
    return infoLabel->text();
}
void FinishedDialog::setInformativeText(const QString &text)
{
    infoLabel->setText(text);
}

QString FinishedDialog::detailedText() const
{
    return textEdit->toPlainText();
}
void FinishedDialog::setDetailedText(const QString &text)
{
    textEdit->setText(text);
}
