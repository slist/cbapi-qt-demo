// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "ioc.h"
#include "ui_ioc.h"

#include <QSettings>
#include <QStandardPaths>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QListWidgetItem>
#include <QMessageBox>

#include <QTreeView>
#include "qjsonmodel.h"
#include "feed.h"

#include "ioclist.h"

#include <QDebug>

Ioc::Ioc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ioc),
    manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    QStringList inst_list_eedr = get_instances_list("instances_eedr");
    for (int inst=0; inst<inst_list_eedr.size(); inst++) {
        ui->comboBox_server->addItem(QIcon(":/img/cloud.png"),inst_list_eedr[inst]);
    }
    /* TODO
    QStringList inst_list_edr = get_instances_list("instances_edr");
    for (int inst=0; inst<inst_list_edr.size(); inst++) {
        ui->comboBox_server->addItem(QIcon(":/img/onprem.png"),inst_list_edr[inst]);
    }

    ui->comboBox_server->addItem(QIcon(":/img/json.png"), tr("JSON file"));
    */
}

Ioc::~Ioc()
{
    delete ui;
    manager->deleteLater();
}

QStringList Ioc::get_instances_list(const QString & inst_type)
{
    QStringList list;
    QSettings settings;

    int size = settings.beginReadArray(inst_type);
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        list << settings.value("name").toString();
    }
    settings.endArray();
    return list;
}

void Ioc::refresh()
{
    // Reset all ...
    ui->listWidget_feeds->clear();
    Feedlist.clear();
    TheBigList_IOC.clear();
    TheBigList_IP.clear();
    jsonReports.clear();
    watchlist_count=0;
    feed_count=0;

    if (ui->comboBox_server->count() == 0)
    {
        return; // No instance
    }
    int currentIndex = ui->comboBox_server->currentIndex();
    name = ui->comboBox_server->itemText(currentIndex);

    bool found = false;

    QSettings settings;
    int size = settings.beginReadArray("instances_eedr");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        if (name == settings.value("name").toString())
        {
            found = true;
            api_id = settings.value("api_id").toString();
            api_secret = settings.value("api_secret").toString();
            server = settings.value("server").toString();
            org_key = settings.value("org_key").toString();
            break;
        }
    }
    settings.endArray();

    if (!found) {
        qDebug("No instance selected"); // OR "JSON File" selected : TODO
        return;
    }

    auth = api_secret + "/" + api_id;

    // Remove text after SPACE in server name
    int i = server.indexOf(" ");
    if (i > 0) {
        server.truncate(i);
    }

    if (org_key == "") {
        qDebug("Need an ORG_KEY for the instance");
        return;
    }

    if (server == "prod05") {
        server_url = "https://defense-prod05.conferdeploy.net";
    } else if (server == "prod06") {
        server_url = "https://defense-eu.conferdeploy.net";
    } else if (server == "prod02") {
        server_url = "https://defense.conferdeploy.net";
    } else if (server == "prod01") {
        server_url = "https://dashboard.confer.net";
    } else if (server == "prodnrt") {
        server_url = "https://defense-prodnrt.conferdeploy.net";
    } else if (server == "prodsyd") {
        server_url = "https://defense-prodsyd.conferdeploy.net";
    } else if (server == "eap01") {
        server_url = "https://defense-eap01.conferdeploy.net";
    }

    QString url(server_url + "/threathunter/watchlistmgr/v3/orgs/" + org_key + "/watchlists");

    QNetworkRequest request=QNetworkRequest(QUrl(url));
    request.setRawHeader("X-Auth-Token", auth.toLocal8Bit());

    watchlist_count = 0;
    feed_count = 0;

    reply = manager->get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinished()));
}

void Ioc::slotError(QNetworkReply::NetworkError)
{
    qDebug() << "Network Error, code : " << reply->error();
    qDebug() << reply->errorString();
    QMessageBox::critical(this, QString("Network Error: %0").arg(reply->error()), reply->errorString(), QMessageBox::Ok);
}

void Ioc::slotSslErrors(QList<QSslError> le)
{
    QString msg;
    qDebug() << "SSL Error";
    for (const QSslError &e : le) {
        msg += e.errorString();
    }
    QMessageBox::critical(this, tr("SSL Error"), msg, QMessageBox::Ok);
}

void Ioc::slotFinished()
{
    QSettings settings;

    // Convert received data from Latin1 to UTF8
    // In Latin1, 'é' character is \xE9

    QByteArray reply_latin1 = reply->readAll();
    QString s;
    s = s.fromLatin1(reply_latin1);
    jsonReports = s.toUtf8();

    QJsonParseError *e = new QJsonParseError();

    QJsonDocument json_doc = QJsonDocument::fromJson(jsonReports, e);
    if (json_doc.isNull()) {
        qDebug("Failed parse JSON");
        reply->deleteLater();
        return;
    }
    if (!json_doc.isObject()) {
        qDebug("JSON is not an object.");
        reply->deleteLater();
        return;
    }

    QJsonObject jsonObject = json_doc.object();
    QJsonArray jsonArray = jsonObject["results"].toArray();

    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
        Feed * feed = new Feed(this); // Feed UI
        connect(feed, SIGNAL(expand(Feed *)), this, SLOT(expand_feed(Feed *)));
        connect(feed, SIGNAL(contract(Feed *)), this, SLOT(contract_feed(Feed *)));

        Feedlist << feed;
        feed->description = obj["description"].toString();
        feed->timestamp_create.setSecsSinceEpoch(obj["create_timestamp"].toInt());
        feed->timestamp_last_update.setSecsSinceEpoch(obj["last_update_timestamp"].toInt());
        feed->server_url = server_url;
        feed->auth = auth;
        feed->org_key = org_key;
        feed->setName(obj["name"].toString());
        feed->setDescription(obj["description"].toString());
        feed->setAlerts(obj["alerts_enabled"].toBool());
        feed->setLastUpdate(feed->timestamp_last_update);

        // Get feed_id to see if it is a watchlist or a feed !
        QJsonObject JsonObjectFeedId = obj["classifier"].toObject();
        feed->feed_id = JsonObjectFeedId["value"].toString();

        if (feed->feed_id.isEmpty()) {
            QImage img(":/img/watchlist.png");;
            if (settings.value("DarkMode").toBool()) {
                img.invertPixels();
            }
            feed->setPixmap(QPixmap::fromImage(img));

            feed->isFeed = false;
            watchlist_count++;
            // get reports number
            int nreports = 0;
            QJsonArray jsonReports = obj["report_ids"].toArray();
            foreach (const QJsonValue & jsonReport, jsonReports) {
                QString report = jsonReport.toString();
                feed->report_ids << report;
                nreports++;
            }
            feed->setReports(nreports);
        } else {
            QImage img(":/img/feed.png");;
            if (settings.value("DarkMode").toBool()) {
                img.invertPixels();
            }
            feed->setPixmap(QPixmap::fromImage(img));
            feed->isFeed = true;
            feed_count++;
        }
    }

    // Sort Watchlists and then Feeds by Name -> in Feedlist_ordered and then add Items in List...
    QList<Feed *> Feedlist_ordered;

    // Sort Watchlist
    for (int i = 0; i < watchlist_count; i++ ) {
        Feed *feed_smallest = NULL;
        int pos_smallest = 0;
        for (int j = 0; j< Feedlist.size(); j++) {
            Feed *f = Feedlist.at(j);
            if (f->isFeed == false) {
                if (feed_smallest == NULL) {
                    feed_smallest = f;
                    pos_smallest = j;
                } else if (f->name < feed_smallest->name) {
                    feed_smallest = f;
                    pos_smallest = j;
                }
            }
        }
        Feedlist_ordered << feed_smallest;
        Feedlist.removeAt(pos_smallest);
    }

    // Sort Feeds
    for (int i = 0; i < feed_count; i++ ) {
        Feed *feed_smallest = NULL;
        int pos_smallest = 0;
        for (int j = 0; j< Feedlist.size(); j++) {
            Feed *f = Feedlist.at(j);
            if (feed_smallest == NULL) {
                feed_smallest = f;
                pos_smallest = j;
            } else if (f->name < feed_smallest->name) {
                feed_smallest = f;
                pos_smallest = j;
            }
        }
        Feedlist_ordered << feed_smallest;
        Feedlist.removeAt(pos_smallest);
    }
    Feedlist = Feedlist_ordered;

    // Display Watchlists and Feeds
    display_watchlists_feeds();

    // Fetch reports
    for (int i = 0; i < Feedlist.size(); i++) {
        Feed *f = Feedlist.at(i);
        connect(f, SIGNAL(downloadFinished(const QString &)), this, SLOT(watchlistDownloadFinished(const QString &)));
        f->fetch_reports();
    }
    reply->deleteLater();
}

void Ioc::on_comboBox_server_currentIndexChanged(int /* index */)
{
    if (default_server != -1) {
        refresh();
    }
}

// TODO : refactor this uggly code
void Ioc::watchlistDownloadFinished(const QString & fname)
{
    if ((fname == "Carbon Black TOR") || (fname == "Tor Exit Nodes") || (fname == "Carbon Black AlienVault"))
    {
        for (int i = 0; i < Feedlist.size(); ++i) {
            Feed *f = Feedlist.at(i);
            if (f->name == fname) {
                if (f->reports.size() > 0) { // Add Feed in the list only if it has reports
                    TheBigList_IP << "\n*****************************************************";
                    TheBigList_IP << fname;
                    TheBigList_IP << "*****************************************************\n";
                }
                for (int j = 0; j < f->reports.size(); ++j) {
                    TheBigList_IP << f->reports[j];
                }
            }
        }
        return;
    }

    for (int i = 0; i < Feedlist.size(); ++i) {
        Feed *f = Feedlist.at(i);
        if (f->name == fname) {
            if (f->reports.size() > 0) { // Add Feed in the list only if it has reports
                TheBigList_IOC << "\n*****************************************************";
                TheBigList_IOC << fname;
                TheBigList_IOC << "*****************************************************\n";
            }
            for (int j = 0; j < f->reports.size(); ++j) {
                TheBigList_IOC << f->reports[j];
            }
        }
    }
}

void Ioc::set_default_server(int index)
{
    default_server = index;
    ui->comboBox_server->setCurrentIndex(index);
    if (index == 0) {
        refresh();
    }
}

void Ioc::on_pushButton_list_ioc_clicked()
{
    auto w = new IocList(this);
    w->setBigList(TheBigList_IOC);
    w->exec();
}

void Ioc::on_pushButton_list_ip_clicked()
{
    auto w = new IocList(this);
    w->setBigList(TheBigList_IP);
    w->exec();
}

void Ioc::on_pushButton_json_view_clicked()
{
    QTreeView * view   = new QTreeView();
    QJsonModel * model = new QJsonModel(this);

    view->setModel(model);
    model->loadJson(jsonReports);
    view->resize(1024,768);
    view->setColumnWidth(0,400);
    view->expandAll();
    view->setWindowTitle(tr("Watchlists and Feeds list"));
    view->setWindowIcon(QIcon(":/img/cb.jpeg"));
    view->show();
}

void Ioc::expand_feed(Feed * f)
{
    int pos_feed = -1;

    for (int i = 0; i < ui->listWidget_feeds->count() && (pos_feed < 0); i++) {
        auto *feed = ui->listWidget_feeds->itemWidget(ui->listWidget_feeds->item(i));
        if (feed == f) {
            pos_feed = i;
        }
    }
    if (pos_feed<0) {
        qDebug() << "Feed to expand not found: " << f->name;
        return;
    }

    // Create reports from reportobjects of the feed and add them at pos pos_feed
    for (int i = 0; i < f->reportList.size(); i++) {
        auto *r = new Report(this);
        r->setReportObject(f->reportList[i]);
        auto *item = new QListWidgetItem();
        item->setSizeHint(r->minimumSizeHint());
        ui->listWidget_feeds->insertItem(pos_feed + 1 + i, item);
        ui->listWidget_feeds->setItemWidget(item, r);
    }
}

void Ioc::contract_feed(Feed * f)
{
    int pos_feed = -1;

    for (int i = 0; i < ui->listWidget_feeds->count() && (pos_feed < 0); i++) {
        auto *feed = ui->listWidget_feeds->itemWidget(ui->listWidget_feeds->item(i));
        if (feed == f) {
            pos_feed = i;
        }
    }
    if (pos_feed<0) {
        qDebug() << "Feed to expand not found: " << f->name;
        return;
    }

    for (int i = 0; i < f->reportList.size(); i++)
    {
        QListWidgetItem *item = ui->listWidget_feeds->takeItem(pos_feed+1);
        delete item;
    }
}

void Ioc::display_watchlists_feeds()
{
    for (int i = 0; i < Feedlist.size(); i++) {
        auto *feed = Feedlist.at(i);
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget_feeds);
        item->setSizeHint(feed->minimumSizeHint());
        ui->listWidget_feeds->setItemWidget(item, feed);
    }
}
