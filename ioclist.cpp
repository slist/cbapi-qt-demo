// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "ioclist.h"
#include "ui_ioclist.h"

#include <QTextDocumentWriter>
#include <QFileDialog>
#include <qmessagebox.h>
#include "iocsyntaxhighlighter.h"

IocList::IocList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IocList)
{
    ui->setupUi(this);
    auto hl = new IocSyntaxHighlighter(ui->textEdit->document());

    if (hl == nullptr) {
        qDebug("Enable to create Syntax highlighter");
    }
}

IocList::~IocList()
{
    delete ui;
}

void IocList::setBigList(QStringList l)
{
    for (int i = 0; i < l.size(); ++i)
    {
        ui->textEdit->append(l.at(i));
    }
}

void IocList::on_pushButton_find_clicked()
{
    QString searchString = ui->lineEdit_searchString->text();
    QTextDocument *document = ui->textEdit->document();

    bool found = false;

    // undo previous change (if any)
    document->undo();

    if (searchString.isEmpty()) {
        QMessageBox::information(this, tr("Empty Search Field"),
                                 tr("The search field is empty. "
                                    "Please enter a word and click Find."));
    } else {
        QTextCursor highlightCursor(document);
        QTextCursor cursor(document);

        cursor.beginEditBlock();

        QTextCharFormat plainFormat(highlightCursor.charFormat());
        QTextCharFormat colorFormat = plainFormat;
        colorFormat.setForeground(Qt::red);
        colorFormat.setBackground(Qt::yellow);

        while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
            highlightCursor = document->find(searchString, highlightCursor,
                                             QTextDocument::FindWholeWords);

            if (!highlightCursor.isNull()) {
                found = true;
                highlightCursor.movePosition(QTextCursor::WordRight,
                                             QTextCursor::KeepAnchor);
                highlightCursor.mergeCharFormat(colorFormat);
            }
        }

        cursor.endEditBlock();
        if (found == false) {
            QMessageBox::information(this, tr("Word Not Found"),
                                     tr("Sorry, the word cannot be found."));
        }
    }
}

void IocList::on_pushButton_txt_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save IOC feed"), "", tr("Text file (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    if (!fileName.contains(".")) {
        fileName += ".txt";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    file.write(ui->textEdit->document()->toPlainText().toLatin1());
    file.close();
}

void IocList::on_pushButton_odf_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save IOC feed"), "", tr("ODF Text Document (*.odt);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QTextDocumentWriter m_write;
        m_write.setFileName(fileName);
        m_write.setFormat("odf");
        m_write.write(ui->textEdit->document());
}

void IocList::on_pushButton_ok_clicked()
{
    close();
}
