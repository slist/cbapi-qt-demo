// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "edr.h"
#include "ui_edr.h"

#include <QSettings>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueRef>
#include <QTreeView>
#include <QFileDialog>
#include <QMessageBox>

#include "qjsonmodel.h"
#include "eraser.h"

Edr::Edr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Edr),
    manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    ui->table->resizeColumnsToContents();
    ui->table->horizontalHeader()->setStretchLastSection(true);
    ui->textEdit->setVisible(false);

    QStringList inst_list_edr = get_instances_list_edr();
    for (int inst=0; inst<inst_list_edr.size(); inst++) {
        ui->comboBox_server->addItem(QIcon(":/img/onprem.png"),inst_list_edr[inst]);
    }

    ui->pushButton_delete->setVisible(false); // TODO
}

QStringList Edr::get_instances_list_edr()
{
    QStringList list;
    QSettings settings;

    int size = settings.beginReadArray("instances_edr");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        list << settings.value("name").toString();
    }
    settings.endArray();
    return list;
}

Edr::~Edr()
{
    delete ui;
}

void Edr::on_pushButton_ping_clicked()
{
    QString server = server_url;

    ui->checkBox_show_logs->setCheckState(Qt::Checked);

    ui->textEdit->append("--------------------------------------------------------------------------------------------------------------------");
    ui->textEdit->append("PING server");
    ui->textEdit->append(server);

    server = server.mid(sizeof("https://")-1); // Remove https://
    ui->textEdit->append(server);

    server=server.split(":").first();
    ui->textEdit->append(server);

    QStringList parameters;
#if defined(WIN32)
    parameters << "-n" << "1";
#else
    parameters << "-c 1";
#endif

    parameters << server;

    int exitCode = QProcess::execute("ping", parameters);
    if (exitCode==0) {
        ui->textEdit->append("Server alive\n");
    } else {
        ui->textEdit->append("Server dead\n");
    }
}

void Edr::on_pushButton_sensor_list_clicked()
{
    ui->textEdit->append("--------------------------------------------------------------------------------------------------------------------");
    ui->textEdit->append("Get Sensor List");

    // Example:
    // curl -k -H X-Auth-Token:e1638bbf0c868c1d437068498a22d5e7e47c6b7f https://192.168.230.2:443/api/v1/sensor

    QString server = server_url;
    QString api = server_api;
    QString url(server + "/api/v1/sensor");
    ui->textEdit->append(url);

    QNetworkRequest request=QNetworkRequest(QUrl(url));
    request.setRawHeader("X-Auth-Token", api.toLocal8Bit());

    reply_error = false;
    reply = manager->get(request);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinished()));
}

void Edr::slotError(QNetworkReply::NetworkError)
{
    reply_error = true;
    ui->textEdit->append("-- BEGIN -- Network error");
    ui->textEdit->append(reply->errorString());
    ui->textEdit->append("-- END -- Network error");
    ui->checkBox_show_logs->setCheckState(Qt::Checked);
    QMessageBox::critical(this, QString("Network Error: %0").arg(reply->error()), reply->errorString(), QMessageBox::Ok);
}

void Edr::slotSslErrors(QList<QSslError> le)
{
    // reply_error = true; We have always an error the first connection if certificate is self signed
    ui->textEdit->append("-- BEGIN -- SSL error");

    for (const QSslError &e : le) {
        ui->textEdit->append(e.errorString());
    }
    reply->ignoreSslErrors();
    ui->textEdit->append("-- END -- SSL error");
    ui->checkBox_show_logs->setCheckState(Qt::Checked);
}

void Edr::slotFinished()
{
    QByteArray sensors_latin1 = reply->readAll();
    reply->deleteLater();

    if (reply_error)
        return;

    QString s;
    s = s.fromLatin1(sensors_latin1);
    jsonReports = s.toUtf8();

    QJsonParseError *e = new QJsonParseError();

    QJsonDocument json_doc = QJsonDocument::fromJson(jsonReports, e);
    if (json_doc.isNull()) {
        ui->textEdit->append("Failed parse JSON");
        ui->textEdit->append(e->errorString());
        return;
    }

    if (!json_doc.isArray()) {
        ui->textEdit->append("JSON is not an array.");
        return;
    }

    QJsonArray jsonArray = json_doc.array();

    int sensor = 0;

    foreach (const QJsonValue & value, jsonArray) {
        QTableWidgetItem *item;

        QJsonObject obj = value.toObject();

        if ((ui->checkBox_show_uninstalled->isChecked()) || (obj["status"].toString() != "Uninstall Pending"))
        {
            ui->table->setRowCount(sensor+1);

            item = new QTableWidgetItem(QString::number(obj["id"].toInt()));
            ui->table->setItem(sensor, 0, item);

            item = new QTableWidgetItem(obj["computer_dns_name"].toString());
            ui->table->setItem(sensor, 1, item);

            item = new QTableWidgetItem(obj["registration_time"].toString());
            ui->table->setItem(sensor, 2, item);

            item = new QTableWidgetItem(obj["last_checkin_time"].toString());
            ui->table->setItem(sensor, 3, item);

            item = new QTableWidgetItem(obj["status"].toString());
            ui->table->setItem(sensor, 4, item);

            item = new QTableWidgetItem("");
            ui->table->setItem(sensor, 5, item);

            sensor++;
        }
    }
    ui->table->resizeColumnsToContents();
    ui->table->horizontalHeader()->setStretchLastSection(true);
    ui->textEdit->append(QString(tr("Found %1 sensors\n").arg(sensor)));
}

void Edr::on_checkBox_show_logs_stateChanged(int arg1)
{
    if (arg1) {
        ui->textEdit->setVisible(true);
    } else {
        ui->textEdit->setVisible(false);
    }
}

void Edr::on_pushButton_duplicate_clicked()
{
    int found = 0;
    int nb = ui->table->rowCount();

    for (int i=0; i<nb; i++)
    {
        QString s_name = ui->table->item(i,1)->text();
        for (int j=i+1; j<nb; j++)
        {
            if (s_name == ui->table->item(j,1)->text())
            {
                found++;
                QTableWidgetItem *item = new QTableWidgetItem("X");
                QString date1 = ui->table->item(i,3)->text();
                QString date2 = ui->table->item(j,3)->text();

                if (date1 < date2)
                {
                    ui->table->setItem(i, 5, item);
                }
                else
                {
                    ui->table->setItem(j, 5, item);
                }
            }
        }
    }
    ui->textEdit->append(QString(tr("Found %1 duplicate sensors\n").arg(found)));
}

void Edr::on_checkBox_show_uninstalled_clicked()
{
    on_pushButton_sensor_list_clicked();
}

void Edr::on_pushButton_delete_clicked()
{
    Eraser *w = new Eraser(this);
    w->setModal(true);

    w->setTable(ui->table);
    w->exec();
    on_pushButton_sensor_list_clicked();
}

void Edr::on_comboBox_server_currentIndexChanged(const QString &name)
{
    QStringList list;
    QSettings settings;

    int size = settings.beginReadArray("instances_edr");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        if (name == settings.value("name").toString())
        {
            server_name = name;
            server_url = settings.value("url").toString();
            server_api = settings.value("api").toString();
            break;
        }
    }
    settings.endArray();
}

void Edr::on_pushButton_json_view_clicked()
{
    QTreeView * view   = new QTreeView;
    QJsonModel * model = new QJsonModel;

    view->setModel(model);

    model->loadJson(jsonReports);
    view->resize(1024,768);
    view->setColumnWidth(0,400);
    view->expandAll();
    view->setWindowTitle(tr("Sensor list"));
    view->setWindowIcon(QIcon(":/img/cb.jpeg"));
    view->show();
}

void Edr::on_pushButton_download_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save sensor list"), "", tr("JSON file (*.json);;All Files (*)"));

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
