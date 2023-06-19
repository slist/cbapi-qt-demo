// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef IPLIST_H
#define IPLIST_H

#include <QWidget>
#include <QHostInfo>
#include <QTimer>

namespace Ui {
class IpList;
}

class IpList : public QWidget
{
    Q_OBJECT

public:
    explicit IpList(QWidget *parent = nullptr);
    ~IpList();

private slots:
    void on_pushButton_lookup_clicked();
    void lookUp();
    void lookedUp(const QHostInfo &host);
    void on_comboBox_region_currentIndexChanged(int index);
    void on_pushButton_add_clicked();

private:
    void addServer(const QString &);
    void updateServerList();
    void updateComboBox();
    Ui::IpList *ui;
    QStringList ipList;
    QStringList ipList_new;
    QStringList ipList_servers;
    QTimer *timer;
    bool lookup_in_progress;
};

#endif // IPLIST_H
