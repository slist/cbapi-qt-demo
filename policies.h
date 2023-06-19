// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef POLICIES_H
#define POLICIES_H

#include <QWidget>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Policies : public QListWidget
{
    Q_OBJECT

public:
    Policies(QWidget *parent = nullptr);
    ~Policies() {}
    void set_instance(const QString & name, const QString & id, const QString & secret, const QString & org_key, const QString & server);
    void download_policies();

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    bool network_error;

    QString name;
    QString api_id;
    QString api_secret;
    QString org_key;
    QString server;

private slots:
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);
    void slotFinished();

public slots:
    void set_instance_name(const QString &);
    void pol_selection(const QString &);
    void pol_deselection(const QString &);
    void unselect_all();
    void refresh();

signals:
    void pol_selected(const QString &, const QString &);
    void pol_deselected(const QString &, const QString &);
    void unselect();
    void log(const QString &);
};

#endif // POLICIES_H
