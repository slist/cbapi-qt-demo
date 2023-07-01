// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>

#include "copy.h"
#include "ui_copy.h"

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

void Copy::send(void)
{
    QString dest_api_id, dest_api_secret, dest_server, dest_org_key;
    QString dest_instance_name(ui->comboBox_instances->currentText());

    qDebug() << "nb_policy_to_copy:" << nb_policy_to_copy;
    qDebug() << "dest_name : " << dest_instance_name;

    // Retrieve API ID/SECRET and Org_key
    QSettings settings;
    int size = settings.beginReadArray("instances");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        if (settings.value("name").toString() == dest_instance_name) {
            dest_api_id = settings.value("api_id").toString();
            dest_api_secret = settings.value("api_secret").toString();
            dest_server = settings.value("server").toString();
            dest_org_key = settings.value("org_key").toString();
            break;
        }
    }
    settings.endArray();

    QString auth = dest_api_secret + "/" + dest_api_id;

    // Remove text after SPACE in server name, for example : "prod06 (Europe)"
    int i = dest_server.indexOf(" ");
    if (i > 0) {
        dest_server.truncate(i);
    }

    QString url("https://api-" + dest_server + ".conferdeploy.net/policyservice/v1/orgs/" + dest_org_key + "/policies");

    qDebug() << "Dest URL:" << url;

    for (int row=0; row < ui->tableWidget_selection->rowCount(); row++) {
        QString src_inst = ui->tableWidget_selection->item(row, 0)->text();
        QString policy = ui->tableWidget_selection->item(row, 1)->text(); // policy is src and dest

        QString policy_filename;
        policy_filename += QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        policy_filename += QString("/%1_%2.json").arg(src_inst).arg(policy);

        qDebug() << "Copy policy: " << policy_filename;

        QNetworkAccessManager * manager = new QNetworkAccessManager(this);
        QNetworkRequest * request = new QNetworkRequest(QUrl(url));
        request->setRawHeader("X-Auth-Token", auth.toLocal8Bit());
        request->setRawHeader("Content-Type", "application/json");

        QFile policy_file(policy_filename);
        if (!policy_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            manager->deleteLater();
            return;
        }
        QByteArray * policy_data = new QByteArray(policy_file.readAll());
        policy_file.close();

        /* Before sending policy to the dest:
         * - remove policy ID
         * - set dest org_key in policy
         * - remove rule_configs (API not ready : https://developer.carbonblack.com/reference/carbon-black-cloud/platform/latest/policy-service/#policy )
         */

        QJsonParseError error;
        QJsonDocument json_doc = QJsonDocument::fromJson(*policy_data, &error);
        if (json_doc.isNull()) {
            qDebug() << tr("Failed parse JSON");
            manager->deleteLater();
            return;
        }
        if (!json_doc.isObject()) {
            qDebug() << tr("JSON is not an object.");
            manager->deleteLater();
            return;
        }
        QJsonObject root = json_doc.object();
        root.remove("id");
        root.insert("org_key", QJsonValue(dest_org_key));

        if (ui->checkBox_ignore_rule_configs->isChecked()) {
            root.remove("rule_configs");
        }

        json_doc = QJsonDocument(root);
        *policy_data = json_doc.toJson();

        connect(manager,SIGNAL(finished(QNetworkReply*)),manager,SLOT(deleteLater()));
        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(copy_finished(QNetworkReply*)));
        manager->post(*request, *policy_data);
    }
}

void Copy::on_pushButton_copy_clicked()
{
    send();
}

void Copy::copy_finished(QNetworkReply* reply)
{
    nb_policy_to_copy--;
    qDebug() << "nb_policy_to_copy:" << nb_policy_to_copy;

    QString res(reply->readAll());

    if (res.startsWith("{\"error_code")) {
        qDebug() << res;
        emit log("Error during copy:" + res);
    }

    if (nb_policy_to_copy == 0) {
        this->deleteLater();
    }
}

void Copy::on_pushButton_cancel_clicked()
{
    this->deleteLater();
}
