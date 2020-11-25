// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "banned.h"
#include "ui_banned.h"
#include "droparea.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QDropEvent>
#include <QCryptographicHash>

Banned::Banned(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Banned)
{
    ui->setupUi(this);

    auto dropArea = new DropArea(ui->widget_droparea);

    ui->widget_droparea->parentWidget()->layout()->replaceWidget(ui->widget_droparea, dropArea);
    ui->widget_droparea = dropArea;

    QStringList labels;
    labels << tr("Format") << tr("Content");
    QTableWidget * formatsTable = ui->tableWidget;
    formatsTable->setColumnCount(2);
    formatsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    formatsTable->setHorizontalHeaderLabels(labels);
    formatsTable->horizontalHeader()->setStretchLastSection(true);

    formatsTable->setVisible(false); // enable for debug only

    connect(dropArea, &DropArea::changed, this, &Banned::updateFormatsTable);
    connect(dropArea, &DropArea::dropped, this, &Banned::updateBannedText);
}

Banned::~Banned()
{
    delete ui;
}

void Banned::updateFormatsTable(const QMimeData *mimeData)
{
    QTableWidget * formatsTable = ui->tableWidget;

    formatsTable->setRowCount(0);
    if (!mimeData)
        return;

    for (const QString &format : mimeData->formats()) {
        QTableWidgetItem *formatItem = new QTableWidgetItem(format);
        formatItem->setFlags(Qt::ItemIsEnabled);
        formatItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);

        QString text;
        if (format == QLatin1String("text/plain")) {
            text = mimeData->text().simplified();
        } else if (format == QLatin1String("text/html")) {
            text = mimeData->html().simplified();
        } else if (format == QLatin1String("text/uri-list")) {
            QList<QUrl> urlList = mimeData->urls();
            for (int i = 0; i < urlList.size() && i < 32; ++i)
                text.append(urlList.at(i).toString() + QLatin1Char(' '));
        } else {
            QByteArray data = mimeData->data(format);
            for (int i = 0; i < data.size() && i < 32; ++i)
                text.append(QStringLiteral("%1 ").arg(uchar(data[i]), 2, 16, QLatin1Char('0')).toUpper());
        }

        int row = formatsTable->rowCount();
        formatsTable->insertRow(row);
        formatsTable->setItem(row, 0, new QTableWidgetItem(format));
        formatsTable->setItem(row, 1, new QTableWidgetItem(text));
    }

    formatsTable->resizeColumnToContents(0);
}

void Banned::updateBannedText(const QMimeData *mimeData)
{
    if (!mimeData)
        return;

    for (const QString &format : mimeData->formats()) {
        if (format == QLatin1String("text/uri-list")) {
            QList<QUrl> urlList = mimeData->urls();
            for (int i = 0; i < urlList.size() && i < 32; ++i) {
                addFile(urlList.at(i).toString());
            }
        }
    }
}

void Banned::addFile(const QString & fileName)
{
    bool notascii = false;
    QString f;

    if (!fileName.startsWith("file://"))
    {
        qDebug() << "Not a file: " << fileName;
        return;
    }

    if (QSysInfo::prettyProductName().contains("Windows", Qt::CaseInsensitive))
    {
        f = fileName.mid(sizeof("file:///") - 1); // On Windows file path starts with c: and not /
    } else {
       f = fileName.mid(sizeof("file://") - 1); // Keep leading / on Unix
    }

    QFile file(f);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can Not open file: " << f;
        return;
    }

    while ((!file.atEnd()) && (notascii == false)) {
        QByteArray line = file.readLine();
        QString s(line);
        s = s.simplified(); // remove whitespace and CR/LF...
        if (s.startsWith("/") || s.startsWith("#"))
            continue;
        if (!s.isSimpleText())
        {
            qDebug() << "Not ASCII ? " << s;
            notascii = true;
            continue;
        }
        if (s.size() == 64) {
            ui->textEdit_hashlist->append("BLACK_LIST,SHA256," + s + "," + f);
        }
    }

    if (notascii == true) {
        QCryptographicHash crypto(QCryptographicHash::Sha256);
        QFile file2(f);
        file2.open(QFile::ReadOnly);
        while(!file2.atEnd()){
            crypto.addData(file2.read(8192));
        }
        QByteArray hash = crypto.result();
        ui->textEdit_hashlist->append("BLACK_LIST,SHA256," + hash.toHex() + "," + f);
    }
}

void Banned::on_pushButton_save_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save list of hashes"), "", tr("CSV file (*.csv);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    if (!fileName.contains(".")) {
        fileName += ".csv";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    file.write(ui->textEdit_hashlist->document()->toPlainText().toLatin1());
    file.close();
}
