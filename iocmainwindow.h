// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef IOCMAINWINDOW_H
#define IOCMAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "iocsyntaxhighlighter.h"
#include "feed.h"
#include "report.h"

namespace Ui {
class IocMainWindow;
}

class IocMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IocMainWindow(QWidget *parent = 0);
    ~IocMainWindow();

private:
    Ui::IocMainWindow *ui;
    QStringList get_instances_list();
    void refresh();

    QString name;
    QString api_id;
    QString api_secret;
    QString server, server_url;
    QString org_key;
    QString auth;
    QByteArray IOCs;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    bool network_error;
    QList<Feed *> Feedlist;

private slots:
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);
    void slotFinished();
    void on_comboBox_currentIndexChanged(int index);
};

#endif // IOCMAINWINDOW_H
