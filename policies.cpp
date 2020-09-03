// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueRef>
#include <QSettings>
#include <QStandardPaths>

#include "policy.h"
#include "policies.h"

Policies::Policies(QWidget *parent) :
    QListWidget (parent),
    manager(new QNetworkAccessManager(this))
{
}

void Policies::set_instance(const QString & n, const QString & id, const QString & secret, const QString & s)
{
    name = n;
    api_id = id;
    api_secret = secret;
    server = s;
    refresh();
}

void Policies::refresh()
{
    QSettings settings;
    int size = settings.beginReadArray("instances");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        if (name == settings.value("name").toString())
        {
            api_id = settings.value("api_id").toString();
            api_secret = settings.value("api_secret").toString();
            server = settings.value("server").toString();
            break;
        }
    }
    settings.endArray();

    clear();
    download_policies();
}

void Policies::download_policies()
{
    QString auth = api_secret + "/" + api_id;

    //emit log(name + ": Download policies");
    // Remove text after SPACE in server name
    int i = server.indexOf(" ");
    if (i > 0) {
        server.truncate(i);
    }
    QString url("https://api-" + server + ".conferdeploy.net/integrationServices/v3/policy");

    QNetworkRequest request=QNetworkRequest(QUrl(url));
    request.setRawHeader("X-Auth-Token", auth.toLocal8Bit());
    reply = manager->get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinished()));
}

void Policies::slotError(QNetworkReply::NetworkError)
{
    qDebug() << "Network Error";
    emit log(name + ": Network error");
}

void Policies::slotSslErrors(QList<QSslError>)
{
    qDebug() << "SSL Error";
    emit log(name + ": SSL error");
}

void Policies::slotFinished()
{
    // Convert received data from Latin1 to UTF8
    // In Latin1, 'é' character is \xE9

    QByteArray policies_latin1 = reply->readAll();
    reply->deleteLater();
    QString s;
    s = s.fromLatin1(policies_latin1);
    policies = s.toUtf8();

    // save reply in a file
    QString qP;
    qP += QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    qP += "/" + name + "_reply.txt";
    QFile qF(qP);

    if (qF.open(QFile::WriteOnly | QIODevice::Text)) {
        QTextStream out(&qF);
        out << policies;
        qF.close();
        // emit log ui->logTextEdit->append("Policy saved in " + qP);
    } else {
        // emit error ui->logTextEdit->append("Can't save policy in " + qP);
    }

    //emit log ui->logTextEdit->append("JSON parsing STARTING");

    QJsonParseError *e = new QJsonParseError();

    QJsonDocument json_doc = QJsonDocument::fromJson(policies, e);
    if (json_doc.isNull()) {
        emit log("Failed parse JSON");
        return;
    }
    if (!json_doc.isObject()) {
        emit log("JSON is not an object.");
        return;
    }

    QJsonObject jsonObject = json_doc.object();
    QJsonArray jsonArray = jsonObject["results"].toArray();

    int pol=0;
    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
        Policy *p = new Policy(this);
        p->setName(obj["name"].toString());
        p->setDescription(obj["description"].toString());

        connect(p,SIGNAL(pol_selected(const QString &)), this, SLOT(pol_selection(const QString &)));
        connect(p,SIGNAL(pol_deselected(const QString &)), this, SLOT(pol_deselection(const QString &)));
        connect(this, SIGNAL(unselect()), p, SLOT(unselect()));

        // save policy in a file
        QJsonDocument docPol;
        QJsonValue valPol = obj["policy"];
        QJsonObject objPol = valPol.toObject();
        docPol.setObject(objPol);
        QString qPol(docPol.toJson());
        QString qPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString("/%1_%2.txt").arg(name).arg(obj["name"].toString());
        QFile qFile(qPath);

        if (qFile.open(QFile::WriteOnly)) {
            QTextStream out(&qFile);
            out << QString("Policy name: " + obj["name"].toString() + "\n");
            out << qPol;
            qFile.close();
        } else {
            qWarning() << "Can't open file";        }

        // Add policy to the widget
        QListWidgetItem *item = new QListWidgetItem(this);
        item->setSizeHint(p->minimumSizeHint());
        this->setItemWidget(item,p);

        // Count rules
        QJsonArray rulesArray = objPol["rules"].toArray();
        p->setRulesNumber(rulesArray.count());

        // Check Sensor Settings
        QJsonArray jsonArray00 = objPol["sensorSettings"].toArray();
        foreach (const QJsonValue & value00, jsonArray00) {
            QJsonObject obj00 = value00.toObject();

            if (obj00["name"].toString() == QString("CB_LIVE_RESPONSE")) {
                QString val_go_live = obj00["value"].toString();
                if (val_go_live == QString("true"))
                {
                    p->setGoLive(true);
                } else {
                    p->setGoLive(false);
                }

            } else if (obj00["name"].toString() == QString("SHOW_UI")) {
                QString val_go_live = obj00["value"].toString();
                if (val_go_live == QString("true"))
                {
                    p->setUi(true);
                } else {
                    p->setUi(false);
                }
            } else if (obj00["name"].toString() == QString("LOGGING_LEVEL")) {
                QString val_go_live = obj00["value"].toString();
                if (val_go_live == QString("false"))
                {
                    p->setLog(false);
                } else {
                    p->setLog(true);
                }
            }
        }
        pol++;
    }
}

void Policies::set_instance_name(const QString &name)
{
    QSettings settings;
    int size = settings.beginReadArray("instances");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        if (name == settings.value("name").toString())
        {
            set_instance(name, settings.value("api_id").toString(), settings.value("api_secret").toString(), settings.value("server").toString());
            return;
        }
    }
    settings.endArray();
    qDebug() << "policy not found : " << name;
}

void Policies::pol_selection(const QString & pol)
{
    emit pol_selected(name, pol);
}

void Policies::pol_deselection(const QString & pol)
{
    emit pol_deselected(name, pol);
}

void Policies::unselect_all()
{
    emit unselect();
}
