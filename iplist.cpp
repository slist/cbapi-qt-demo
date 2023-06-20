// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "iplist.h"
#include "ui_iplist.h"

#include <QTimer>
#include <QSettings>
#include <QMessageBox>

#include "iplistadd.h"

IpList::IpList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IpList),
    lookup_in_progress(false)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    updateServerList();
    updateComboBox();
    on_comboBox_region_currentIndexChanged(0);
}

IpList::~IpList()
{
    delete ui;
}

void IpList::on_pushButton_lookup_clicked()
{
    if (!lookup_in_progress)
    {
        lookup_in_progress = true;
        on_comboBox_region_currentIndexChanged(0);
        connect(timer, SIGNAL(timeout()), this, SLOT(lookUp()));
        timer->start(30*1000);
        ui->pushButton_lookup->setText(tr("Lookup in loop every 30s in progress...")); // QT internal cache is updated every 30s.
        ui->comboBox_region->setEnabled(false);
        lookUp(); // Don't wait 30s for first lookup
    } else {
        lookup_in_progress = false;
        timer->stop();
        ui->pushButton_lookup->setText(tr("Lookup"));
        ui->comboBox_region->setEnabled(true);
    }
}

void IpList::lookUp()
{
    QString host = ui->comboBox_region->currentText();
    QHostInfo::lookupHost(host, this, SLOT(lookedUp(QHostInfo)));
}


void IpList::lookedUp(const QHostInfo &host)
{
    bool newIP = false;

    if (host.error() != QHostInfo::NoError) {
        qDebug() << "Lookup failed:" << host.errorString();
        QMessageBox::critical(this, tr("Lookup failed"), host.errorString(), QMessageBox::Ok);
        return;
    }

    const auto addresses = host.addresses();
    for (const QHostAddress &address : addresses) {
        QString s = address.toString();
        if (!ipList.contains(s)) {
            newIP = true;
            ipList.append(s);
            ipList_new.append(s);
        }
    }

    if (newIP) {
        int count = 0;
        ipList.sort();
        ui->plainTextEdit->clear();

        for (const QString &s : ipList) {
            ui->plainTextEdit->appendPlainText(s);
            count++;
        }
        ui->plainTextEdit->appendPlainText(QString("%0 IP addresses").arg(count));
        ui->plainTextEdit->appendPlainText("\n------- ");
        ui->plainTextEdit->appendPlainText(tr("New addresses"));
        ui->plainTextEdit->appendPlainText(" -------- \n");

        count = 0;
        ipList_new.sort();

        for (const QString &s : ipList_new) {
            ui->plainTextEdit->appendPlainText(s);
            count++;
        }
        ui->plainTextEdit->appendPlainText(QString("%0 new IP addresses").arg(count));
    }

    QSettings settings;
    settings.setValue(ui->comboBox_region->currentText() + "_iplist", ipList);
}

void IpList::on_comboBox_region_currentIndexChanged(int i)
{
    int count = 0;
    QSettings settings;

    ui->plainTextEdit->clear();
    ipList_new.clear();
    ipList = settings.value(ui->comboBox_region->currentText() + "_iplist").toStringList();

    for (const QString &s : ipList) {
        ui->plainTextEdit->appendPlainText(s);
        count++;
    }
    ui->plainTextEdit->appendPlainText(QString("%0 IP addresses").arg(count));
}

void IpList::on_pushButton_add_clicked()
{
    auto d = new ipListAdd(this);
    int res = d->exec();
    if (res == QDialog::Accepted)
    {
        addServer(d->getServer());
        updateComboBox();
        ui->comboBox_region->setCurrentText(d->getServer());
    }
}

void IpList::addServer(const QString &newServer)
{
    QSettings settings;
    ipList_servers << newServer;
    ipList_servers.removeDuplicates();
    ipList_servers.sort();
    settings.setValue("ipList_servers", ipList_servers);
}

void IpList::updateServerList()
{
    ipList_servers.clear();
    ipList_servers << "api-prod06.conferdeploy.net"
                   << "api-prodnrt.conferdeploy.net"
                   << "api-prodsyd.conferdeploy.net"
                   << "api.confer.net"
                   << "api5.conferdeploy.net"
                   << "content.carbonblack.io"
                   << "crl.godaddy.com"
                   << "dashboard.confer.net"
                   << "defense-eu.conferdeploy.net"
                   << "defense-prodnrt.conferdeploy.net"
                   << "defense-prodsyd.conferdeploy.net"
                   << "defense.conferdeploy.net"
                   << "dev-prod05.conferdeploy.net"
                   << "dev-prod06.conferdeploy.net"
                   << "dev-prodnrt.conferdeploy.net"
                   << "dev-prodsyd.conferdeploy.net"
                   << "dev5.conferdeploy.net"
                   << "devices.confer.net"
                   << "ocsp.godaddy.com"
                   << "updates2.cdc.carbonblack.io";
    QSettings settings;
    ipList_servers << settings.value("ipList_servers").toStringList();
    ipList_servers.removeDuplicates();
    ipList_servers.sort();
}

void IpList::updateComboBox()
{
    ui->comboBox_region->clear();
    for (const QString &server : ipList_servers) {
        ui->comboBox_region->addItem(server);
    }
}
