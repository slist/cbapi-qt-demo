// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueRef>
#include <QSettings>
#include <QStandardPaths>
#include <QMessageBox>

#include "policy.h"
#include "ui_policy.h"

Policy::Policy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Policy),
    id(-1),
    manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
}

Policy::~Policy()
{
    delete ui;
}

void Policy::download_policy()
{
    if (id == -1) {
        qDebug() << "Policy id not set";
        return;
    }

    QString auth = api_secret + "/" + api_id;

    // Remove text after SPACE in server name
    int i = server.indexOf(" ");
    if (i > 0) {
        server.truncate(i);
    }

    //GET {cbc-hostname}/policyservice/v1/orgs/{org_key}/policies/{policy_id}

    QString url("https://api-" + server + ".conferdeploy.net/policyservice/v1/orgs/" + org_key + "/policies/" + QString::number(id));
    qDebug() << "Download policy: " << policy_name << url;

    QNetworkRequest request=QNetworkRequest(QUrl(url));
    request.setRawHeader("X-Auth-Token", auth.toLocal8Bit());
    reply = manager->get(request);

#if QT_VERSION_MAJOR == 5
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
#endif
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinished()));
}


void Policy::slotError(QNetworkReply::NetworkError)
{
    qDebug() << "Network Error";
    QMessageBox::critical(this, QString("Network Error: %0").arg(reply->error()), reply->errorString(), QMessageBox::Ok);
}

void Policy::slotSslErrors(QList<QSslError>)
{
    qDebug() << "SSL Error";
}


void Policy::slotFinished()
{
    QByteArray policy;

    // Convert received data from Latin1 to UTF8
    // In Latin1, 'é' character is \xE9

    QByteArray policy_latin1 = reply->readAll();
    reply->deleteLater();
    QString s;
    s = s.fromLatin1(policy_latin1);
    policy = s.toUtf8();

    QJsonParseError *e = new QJsonParseError();

    QJsonDocument json_doc = QJsonDocument::fromJson(policy, e);
    if (json_doc.isNull()) {
        qDebug("Failed parse JSON");
        return;
    }
    if (!json_doc.isObject()) {
        qDebug("JSON is not an object.");
        return;
    }

    QJsonObject jsonObject = json_doc.object();

    if (jsonObject["success"].toBool(true) == false)
    {
        qDebug() << "Instance \"" + instance_name
                    + "\", Policy \"" + policy_name
                    + "\", Error message: " + jsonObject["message"].toString();
        return;
    }

    // save policy in a file
    QString qP;
    qP += QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    qP += "/" + instance_name + "_" + policy_name + ".json";
    QFile qF(qP);

    if (qF.open(QFile::WriteOnly | QIODevice::Text)) {
        QTextStream out(&qF);
        out << json_doc.toJson();
        qF.close();
    } else {
        qWarning() << "Can't save policy in " << qP;
    }
    qDebug() << "Saved policy in file: " << qP;

    // Count rules
    QJsonArray rulesArray = jsonObject["rules"].toArray();
    setRulesNumber(rulesArray.count());

    // Check Sensor Settings
    QJsonArray jsonArray = jsonObject["sensor_settings"].toArray();
    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();

        if (obj["name"].toString() == QString("CB_LIVE_RESPONSE")) {
            QString val_go_live = obj["value"].toString();
            if (val_go_live == QString("true"))
            {
                setGoLive(true);
            } else {
                setGoLive(false);
            }

        } else if (obj["name"].toString() == QString("SHOW_UI")) {
            QString val_go_live = obj["value"].toString();
            if (val_go_live == QString("true"))
            {
                setUi(true);
            } else {
                setUi(false);
            }
        }
    }
}

void Policy::setName(const QString n)
{
    policy_name = n;
    ui->label_name->setText(n);
}

void Policy::set_instance(const QString & n, const QString & id, const QString & secret, const QString & o, const QString & s)
{
    instance_name = n;
    api_id = id;
    api_secret = secret;
    org_key = o;
    server = s;
}

void Policy::setDescription(const QString d)
{
    description = d;
    ui->label_name->setToolTip(d);
}

void Policy::setUi(bool b)
{
    if (b) {
        ui->label_ui->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
        ui->label_ui->setText("UI\nON");
    } else {
        ui->label_ui->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
        ui->label_ui->setText("UI\nOFF");
    }
}

void Policy::setGoLive(bool b)
{
    ui->label_go_live->setAlignment(Qt::AlignCenter);
    ui->label_go_live->setMargin(2);

    if (b) {
        ui->label_go_live->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
        ui->label_go_live->setText(" Go Live \nON");
    } else {
        ui->label_go_live->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
        ui->label_go_live->setText(" Go Live \nOFF");
    }
}

void Policy::setRulesNumber(int n)
{
    if (n > 0) {
        ui->label_rules->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
    } else {
        ui->label_rules->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
    }
    QString s = QString("Rules:\n%1").arg(n);
    ui->label_rules->setText(s);
}

void Policy::setId(int i)
{
    id =i;
}

void Policy::setNumDevices(int n)
{
    num_devices = n;
    if (n > 0) {
        ui->label_num_devices->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #2AA62A;");
    } else {
        ui->label_num_devices->setStyleSheet("border: 2px solid gray;border-radius: 5px;background-color: #FD7979;");
    }
    QString s = QString("Devices:\n%1").arg(n);
    ui->label_num_devices->setText(s);
}

void Policy::on_checkBox_stateChanged(int arg1)
{
    switch (arg1) {
    case Qt::Unchecked:
        emit pol_deselected(policy_name);
        break;
    case Qt::Checked:
        emit pol_selected(policy_name);
        break;
    }
}

void Policy::unselect()
{
    ui->checkBox->setChecked(false);
}
