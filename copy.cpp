// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "copy.h"
#include "ui_copy.h"

#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>

Copy::Copy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Copy),
    nb_policy_to_copy(0)
{
    ui->setupUi(this);
    ui->tableWidget_selection->horizontalHeader()->setStretchLastSection(true);
}

Copy::~Copy()
{
    delete ui;
}

void Copy::set_inst_list(QStringList list)
{
    for (int i = 0; i < list.size(); ++i) {
        ui->comboBox_instances->addItem(list[i]);
    }
}

void Copy::add_pol(const QString & a, const QString & b)
{
    int row = ui->tableWidget_selection->rowCount() + 1;
    QTableWidgetItem *newItem_inst= new QTableWidgetItem(a);
    QTableWidgetItem *newItem_pol = new QTableWidgetItem(b);

    ui->tableWidget_selection->setRowCount(row--);
    ui->tableWidget_selection->setItem(row, 0, newItem_inst);
    ui->tableWidget_selection->setItem(row, 1, newItem_pol);
    ui->tableWidget_selection->resizeColumnToContents(0);
    nb_policy_to_copy++;
}

/*
 * See https://developer.carbonblack.com/reference/carbon-black-cloud/cb-defense/latest/rest-api/
 *
 * $ curl -X POST -H X-Auth-Token:ABCD/1234 -H Content-Type:application/json \
 *         https://api-url.conferdeploy.net/integrationServices/v3/policy -d @policy.txt
 *
 */

void Copy::send(void)
{
    QString dest_name, dest_api_id, dest_api_secret, dest_server;
    // Get dest info
    dest_name = ui->comboBox_instances->currentText();
    //qDebug() << "dest_name : " << dest_name;

    // Retrieve API ID and API SECRET
    QSettings settings;
    int size = settings.beginReadArray("instances");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        if (settings.value("name").toString() == dest_name) {
            dest_api_id = settings.value("api_id").toString();
            dest_api_secret = settings.value("api_secret").toString();
            dest_server = settings.value("server").toString();
        }
    }
    settings.endArray();

    QString auth = dest_api_secret + "/" + dest_api_id;

    // Remove text after SPACE in server name
    int i = dest_server.indexOf(" ");
    if (i > 0) {
        dest_server.truncate(i);
    }

    QString url("https://api-" + dest_server + ".conferdeploy.net/integrationServices/v3/policy");

    for (int row=0; row < ui->tableWidget_selection->rowCount(); row++) {
        QString src_inst = ui->tableWidget_selection->item(row, 0)->text();
        QString policy = ui->tableWidget_selection->item(row, 1)->text(); // policy is src and dest

        QString f;
        f += QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        f += QString("/%1_%2.txt").arg(src_inst).arg(policy);

        QNetworkAccessManager * manager = new QNetworkAccessManager(this);
        QNetworkRequest * request = new QNetworkRequest(QUrl(url));
        request->setRawHeader("X-Auth-Token", auth.toLocal8Bit());
        request->setRawHeader("Content-Type", "application/json");

        QByteArray * data = new QByteArray("");

        data->append("{ \"policyInfo\": {                      \
                     \"description\": \"imported policy\",     \
                     \"name\": \"");
        data->append(policy.toUtf8());
        data->append("\", \"policy\": ");

        QFile file(f);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        // Remove first line
        QByteArray esc = file.readLine();
        if (esc.isEmpty()) {
            qDebug() << "First line should not be empty : " << f;
        }

        while (!file.atEnd()) {
            data->append(file.readLine());
        }

        data->append(", \"priorityLevel\": \"LOW\", \"version\": 2 }}");

        connect(manager,SIGNAL(finished(QNetworkReply*)),manager,SLOT(deleteLater()));
        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(copy_finished(QNetworkReply*)));
        manager->post(*request, *data);
    }
}

void Copy::on_pushButton_copy_clicked()
{
    send();
}

void Copy::copy_finished(QNetworkReply* reply)
{
    nb_policy_to_copy--;

    QString res(reply->readAll());

    if (!res.contains("Success")) {
        qDebug() << res;
    }

    if (nb_policy_to_copy == 0) {
        this->deleteLater();
    }
}

void Copy::on_pushButton_cancel_clicked()
{
     this->deleteLater();
}
