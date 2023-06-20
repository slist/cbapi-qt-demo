// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "iocmainwindow.h"
#include "ui_iocmainwindow.h"

#include <QSettings>
#include <QStandardPaths>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QListWidgetItem>

#include <QTreeView>
#include "qjsonmodel.h"
#include "feed.h"

#include <QDebug>

IocMainWindow::IocMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IocMainWindow),
    manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    QStringList inst_list = get_instances_list();

    for (int inst=0; inst<inst_list.size(); inst++) {
        ui->comboBox_left->addItem(QIcon(":/img/cloud.png"),inst_list[inst]);
    }
}

IocMainWindow::~IocMainWindow()
{
    delete ui;
}

QStringList IocMainWindow::get_instances_list()
{
    QStringList inst_list;
    QSettings settings;

    int size = settings.beginReadArray("instances");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        if (!settings.value("org_key").toString().isEmpty()) {
            inst_list << settings.value("name").toString();
        }
    }
    settings.endArray();
    return inst_list;
}

void IocMainWindow::slotError(QNetworkReply::NetworkError)
{
    qDebug() << "Network Error";
    qDebug() << "Error CODE : " << reply->error();
    qDebug() << reply->errorString();
}

void IocMainWindow::slotSslErrors(QList<QSslError>)
{
    qDebug() << "SSL Error";
}

void IocMainWindow::slotFinished()
{
    // Convert received data from Latin1 to UTF8
    // In Latin1, 'é' character is \xE9

    QByteArray reply_latin1 = reply->readAll();
    reply->deleteLater();
    QString s;
    s = s.fromLatin1(reply_latin1);
    QByteArray IocFeeds = s.toUtf8();

    QJsonParseError *e = new QJsonParseError();

    QJsonDocument json_doc = QJsonDocument::fromJson(IocFeeds, e);
    if (json_doc.isNull()) {
        qDebug("Failed parse JSON");
        return;
    }
    if (!json_doc.isObject()) {
        qDebug("JSON is not an object.");
        return;
    }
}

void IocMainWindow::on_comboBox_currentIndexChanged(int i)
{
    qDebug() << "TODO, change index" << i;
    // refresh(); TODO !
}
