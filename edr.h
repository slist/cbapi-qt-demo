// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef EDR_H
#define EDR_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonValue>

namespace Ui {
class Edr;
}

class Edr : public QWidget
{
    Q_OBJECT

public:
    explicit Edr(QWidget *parent = nullptr);
    ~Edr();

private slots:
    void on_pushButton_ping_clicked();
    void on_pushButton_sensor_list_clicked();
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);
    void slotFinished();
    void on_checkBox_show_logs_stateChanged(int);
    void on_checkBox_show_uninstalled_clicked();
    void on_pushButton_duplicate_clicked();
    void on_pushButton_delete_clicked();
    void on_comboBox_server_currentIndexChanged(const QString &);
    void on_pushButton_json_view_clicked();
    void on_pushButton_download_clicked();

private:
    Ui::Edr *ui;
    QStringList get_instances_list_edr();

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    bool reply_error;
    QString server_name;
    QString server_url;
    QString server_api;
    QByteArray jsonReports;
};

#endif // EDR_H
