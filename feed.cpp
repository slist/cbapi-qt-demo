// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "feed.h"
#include "ui_feed.h"

#include <QPixmap>
#include <QNetworkRequest>
#include <QTreeView>
#include <QFileDialog>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

#include "qjsonmodel.h"

#include <QDebug>

Feed::Feed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Feed),
    manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
}

Feed::~Feed()
{
    delete ui;
    manager->deleteLater();
}

void Feed::setName(const QString & n) {
    ui->label_name->setText(n);
    name = n;
}

void Feed::setDescription(const QString & desc)
{
    ui->label_icon->setToolTip(desc);
}

void Feed::setPixmap(const QPixmap & pix)
{
    ui->label_icon->setPixmap(pix);
}

void Feed::setAlerts(bool b)
{
    if (b) {
        ui->label_alerts->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
        ui->label_alerts->setText("Alerts\nON");
    } else {
        ui->label_alerts->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
        ui->label_alerts->setText("Alerts\nOFF");
    }
}

void Feed::setLastUpdate(const QDateTime & dateTime)
{
    QString s;
    s = tr("Last updated:") + "\n" + dateTime.toString("ddd MMMM d yyyy");
    ui->label_last_updated->setText(s);

    QDateTime current;
    current = QDateTime::currentDateTime();

    if (dateTime.daysTo(current) < 60) {
        ui->label_new->setPixmap(QPixmap(":/img/new.png").scaledToHeight(40));
    } else {
        ui->label_new->setPixmap(QPixmap(""));
    }
}

void Feed::fetch_reports()
{
    QString url;

    if (isFeed) {
        ui->pushButton_expand->setVisible(false);
        // qDebug() << "Fetch reports for Feed : " << name;
        url = server_url + "/threathunter/feedmgr/v2/orgs/" + org_key + "/feeds/" + feed_id + "/reports";
    } else {
        // qDebug() << "Fetch reports for Watchlist : " << name;

        ui->pushButton_download->setDisabled(true);
        ui->pushButton_json_view->setDisabled(true);

        foreach(const QString & id, report_ids) {
            auto *r = new ReportObject(this);
            r->id = id;
            r->server_url = server_url;
            r->auth = auth;
            r->org_key = org_key;
            r->fetch_report();
            reportList << r;
            emit new_report(r);
        }
    }

    if (feed_id.isEmpty()) {
        return; // TODO
    }

    // Get Feed reports from feed_id

    QNetworkRequest request=QNetworkRequest(QUrl(url));
    request.setRawHeader("X-Auth-Token", auth.toLocal8Bit());

    reply = manager->get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinished()));
}

void Feed::slotError(QNetworkReply::NetworkError)
{
    qDebug() << "Network Error";
    qDebug() << "Error CODE : " << reply->error();
    qDebug() << reply->errorString();
    QMessageBox::critical(this, QString("Network Error: %0").arg(reply->error()), reply->errorString(), QMessageBox::Ok);
}

void Feed::slotSslErrors(QList<QSslError>)
{
    qDebug() << "SSL Error";
}

void Feed::slotFinished()
{
    // Convert received data from Latin1 to UTF8
    // In Latin1, 'é' character is \xE9

    QByteArray reply_latin1 = reply->readAll();
    QString s;
    s = s.fromLatin1(reply_latin1);
    jsonReports = s.toUtf8();
    IOC_count = 0;

    QJsonParseError *e = new QJsonParseError();

    QJsonDocument json_doc = QJsonDocument::fromJson(jsonReports, e);
    if (json_doc.isNull()) {
        qDebug("Failed parse JSON");
        reply->deleteLater();
        return;
    }
    if (!json_doc.isObject()) {
        qDebug("JSON is not an object.");
        reply->deleteLater();
        return;
    }

    QJsonObject jsonObject = json_doc.object();
    QJsonArray jsonArray = jsonObject["results"].toArray();

    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject(); // obj = 1 report
        QDateTime timestamp;
        timestamp.setSecsSinceEpoch(obj["timestamp"].toInt());

        QJsonArray report_array = obj["iocs_v2"].toArray()[0].toObject()["values"].toArray();
        int report_array_size= report_array.size();
        if (report_array_size < 1000) {
            for (int i=0; i < report_array_size; i++) {
                QString report_ioc = obj["iocs_v2"].toArray()[0].toObject()["values"].toArray()[i].toString();
                reports << report_ioc;
            }
        }
        IOC_count += report_array_size;
    }
    setReports(IOC_count);
    emit downloadFinished(name);
    reply->deleteLater();
}

void Feed::setReports(int n) {
    if (n > 0) {
        ui->label_reports->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
    } else {
        ui->label_reports->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
    }
    QString s = QString("IOCs:\n%1").arg(n);
    ui->label_reports->setText(s);
}

void Feed::on_pushButton_json_view_clicked()
{
    QTreeView * view   = new QTreeView;
    QJsonModel * model = new QJsonModel;

    view->setModel(model);
    model->loadJson(jsonReports);
    view->resize(1024,768);
    view->setColumnWidth(0,400);
    view->expandAll();
    view->setWindowTitle("Watchlist Reports for Feed : " + name);
    view->setWindowIcon(QIcon(":/img/cb.jpeg"));
    view->show();
}

void Feed::on_pushButton_download_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save IOC feed"), "", tr("JSON feed (*.json);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    if (!fileName.contains(".")) {
        fileName += ".json";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }
    file.write(jsonReports);
    file.close();
}

void Feed::on_pushButton_expand_clicked()
{
    if (ui->pushButton_expand->text() == "v")
    {
        ui->pushButton_expand->setText("^");
        emit expand(this);
    } else {
        ui->pushButton_expand->setText("v");
        emit contract(this);
    }
}
