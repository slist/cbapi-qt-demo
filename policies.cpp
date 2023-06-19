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
#include "policies.h"

Policies::Policies(QWidget *parent) :
    QListWidget (parent),
    manager(new QNetworkAccessManager(this))
{
}

void Policies::set_instance(const QString & n, const QString & id, const QString & secret, const QString & o, const QString & s)
{
    name = n;
    api_id = id;
    api_secret = secret;
    org_key = o;
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
            org_key = settings.value("org_key").toString();
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

    // Remove text after SPACE in server name
    int i = server.indexOf(" ");
    if (i > 0) {
        server.truncate(i);
    }

    QString url("https://api-" + server + ".conferdeploy.net/policyservice/v1/orgs/" + org_key + "/policies/summary");
    qDebug() << "Download policies for instance " << name << url;

    QNetworkRequest request=QNetworkRequest(QUrl(url));
    request.setRawHeader("X-Auth-Token", auth.toLocal8Bit());
    reply = manager->get(request);

#if QT_VERSION_MAJOR == 5
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
#endif
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinished()));
}

void Policies::slotError(QNetworkReply::NetworkError)
{
    qDebug() << "Network Error";
    emit log(name + ": Network error");
    QMessageBox::critical(this, QString("Network Error: %0").arg(reply->error()), reply->errorString(), QMessageBox::Ok);
}

void Policies::slotSslErrors(QList<QSslError>)
{
    qDebug() << "SSL Error";
    emit log(name + ": SSL error");
}

void Policies::slotFinished()
{
    QByteArray policies; // policies summary

    // Convert received data from Latin1 to UTF8
    // In Latin1, 'é' character is \xE9

    QByteArray policies_latin1 = reply->readAll();
    reply->deleteLater();
    QString s;
    s = s.fromLatin1(policies_latin1);
    policies = s.toUtf8();

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

    if (jsonObject["success"].toBool(true) == false)
    {
        qWarning() << ("Instance \"" + name + "\", Error message: " + jsonObject["message"].toString());
        emit log("Instance \"" + name + "\", Error message: " + jsonObject["message"].toString());
        return;
    }

    QJsonArray jsonArray = jsonObject["policies"].toArray();

    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
        Policy *p = new Policy(this);

        p->setName(obj["name"].toString());
        p->setDescription(obj["description"].toString());
        p->setId(obj["id"].toInt());
        p->setNumDevices(obj["num_devices"].toInt());

        p->set_instance(name,api_id,api_secret,org_key,server);
        connect(p,SIGNAL(pol_selected(const QString &)), this, SLOT(pol_selection(const QString &)));
        connect(p,SIGNAL(pol_deselected(const QString &)), this, SLOT(pol_deselection(const QString &)));
        connect(this, SIGNAL(unselect()), p, SLOT(unselect()));
        p->download_policy();

        // Add policy to the widget
        QListWidgetItem *item = new QListWidgetItem(this);
        item->setSizeHint(p->minimumSizeHint());
        this->setItemWidget(item,p);
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
            set_instance(name,
                         settings.value("api_id").toString(),
                         settings.value("api_secret").toString(),
                         settings.value("org_key").toString(),
                         settings.value("server").toString());
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
