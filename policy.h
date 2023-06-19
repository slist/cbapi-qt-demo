// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef POLICY_H
#define POLICY_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class Policy;
}

class Policy : public QWidget
{
    Q_OBJECT

public:
    explicit Policy(QWidget *parent = 0);
    ~Policy();
    void setName(const QString policy_name);
    void setDescription(const QString description);
    void setUi(bool b);
    void setGoLive(bool b);
    void setRulesNumber(int);
    void setId(int);
    void setNumDevices(int);

    void set_instance(const QString & instance_name,
                      const QString & api_id,
                      const QString & api_secret,
                      const QString & org_key,
                      const QString & server);
    void download_policy();

private slots:
    void on_checkBox_stateChanged(int arg1);
    void unselect();
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);
    void slotFinished();

private:
    Ui::Policy *ui;
    QString policy_name;
    QString description;
    int id;
    int num_devices;

    QString instance_name;
    QString api_id;
    QString api_secret;
    QString org_key;
    QString server;

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    bool network_error;

signals:
    void pol_selected(const QString &);
    void pol_deselected(const QString &);
};

#endif // POLICY_H
