// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QtDebug>
#include <QSettings>

#include "instance.h"
#include "instanceedr.h"
#include "instanceappcontrol.h"
#include "instances.h"
#include "ui_instances.h"

Instances::Instances(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instances)
{
    ui->setupUi(this);
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
}

Instances::~Instances()
{
    delete ui;
}

void Instances::on_pushButton_add_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

    switch(instanceType) {
    case Instances::NgavInstanceType:
    {
        auto *i = new Instance(this);
        i->setInstanceType(Instances::NgavInstanceType);
        item->setSizeHint(i->minimumSizeHint());
        ui->listWidget->setItemWidget(item,i);
    }
        break;

    case Instances::EedrInstanceType:
    {
        auto *i = new Instance(this);
        i->setInstanceType(Instances::EedrInstanceType);
        item->setSizeHint(i->minimumSizeHint());
        ui->listWidget->setItemWidget(item,i);
    }
        break;

    case Instances::EdrInstanceType:
    {
        auto *i = new InstanceEdr(this);
        item->setSizeHint(i->minimumSizeHint());
        ui->listWidget->setItemWidget(item,i);
    }
        break;

    case Instances::AppcontrolInstanceType:
    {
        auto *i = new InstanceAppcontrol(this);
        item->setSizeHint(i->minimumSizeHint());
        ui->listWidget->setItemWidget(item,i);
    }
        break;

    }
    ui->listWidget->scrollToBottom();
};

void Instances::on_save()
{
    switch (instanceType) {
    case NgavInstanceType:
    {
        QSettings settings;
        Instance *inst;
        int count = ui->listWidget->count();
        int count_valid = 0;

        settings.beginWriteArray("instances");

        for (int i=0; i<count;i++) {
            inst = qobject_cast<Instance *>(ui->listWidget->itemWidget(ui->listWidget->item(i)));

            if ((inst) && (inst->isValid())) {
                settings.setArrayIndex(count_valid++);
                qDebug() << "Saved Endpoint Standard instance: " << inst->get_name();
                settings.setValue("name", inst->get_name());
                settings.setValue("api_id", inst->get_api_id());
                settings.setValue("api_secret", inst->get_api_secret());
                settings.setValue("org_key", inst->get_org_key());
                settings.setValue("server", inst->get_server_display_name());
            }
        }
        settings.endArray();
    }
        break;
    case EedrInstanceType:
    {
        QSettings settings;
        Instance *inst;
        int count = ui->listWidget->count();
        int count_valid = 0;

        settings.beginWriteArray("instances_eedr");

        for (int i=0; i<count;i++) {
            inst = qobject_cast<Instance *>(ui->listWidget->itemWidget(ui->listWidget->item(i)));

            if ((inst) && (inst->isValid())) {
                settings.setArrayIndex(count_valid++);
                settings.setValue("name", inst->get_name());
                settings.setValue("api_id", inst->get_api_id());
                settings.setValue("api_secret", inst->get_api_secret());
                settings.setValue("org_key", inst->get_org_key());
                settings.setValue("server", inst->get_server_display_name());
            }
        }
        settings.endArray();
    }
        break;
    case EdrInstanceType:
    {
        QSettings settings;
        InstanceEdr *inst;
        int count = ui->listWidget->count();
        int count_valid = 0;

        settings.beginWriteArray("instances_edr");

        for (int i=0; i<count;i++) {
            inst = qobject_cast<InstanceEdr *>(ui->listWidget->itemWidget(ui->listWidget->item(i)));

            if ((inst) && (inst->isValid())) {
                settings.setArrayIndex(count_valid++);
                settings.setValue("name", inst->get_name());
                settings.setValue("api", inst->get_api());
                settings.setValue("url", inst->get_url());
            }
        }
        settings.endArray();
    }
        break;

    case AppcontrolInstanceType:
    {
        QSettings settings;
        InstanceAppcontrol *inst;
        int count = ui->listWidget->count();
        int count_valid = 0;

        settings.beginWriteArray("instances_appcontrol");

        for (int i=0; i<count;i++) {
            inst = qobject_cast<InstanceAppcontrol *>(ui->listWidget->itemWidget(ui->listWidget->item(i)));

            if ((inst) && (inst->isValid())) {
                settings.setArrayIndex(count_valid++);
                settings.setValue("name", inst->get_name());
                settings.setValue("api", inst->get_api());
                settings.setValue("url", inst->get_url());
            }
        }
        settings.endArray();
    }
        break;

    }
}

void Instances::setInstanceType(InstanceType type)
{
    instanceType = type;

    switch (type) {
    case NgavInstanceType:
    {
        QSettings settings;
        int size = settings.beginReadArray("instances");
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);

            Instance *inst = new Instance(this);
            inst->setInstanceType(type);
            qDebug() << "Loaded Endpoint Standard instance: " << settings.value("name").toString();
            inst->set_name(settings.value("name").toString());
            inst->set_api_id(settings.value("api_id").toString());
            inst->set_api_secret(settings.value("api_secret").toString());
            inst->set_server(settings.value("server").toString());
            inst->set_org_key(settings.value("org_key").toString());

            if (inst->isValid()) {            // Add instance to the instance's List widget
                QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
                item->setSizeHint(inst->minimumSizeHint());
                ui->listWidget->setItemWidget(item,inst);
            } else {
                delete inst;
            }
        }
        settings.endArray();
        if (ui->listWidget->count() == 0) {
            on_pushButton_add_clicked();
        }
    }
        break;
    case EedrInstanceType:
    {
        QSettings settings;
        int size = settings.beginReadArray("instances_eedr");
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);

            Instance *inst = new Instance(this);
            inst->setInstanceType(type);
            inst->set_name(settings.value("name").toString());
            inst->set_api_id(settings.value("api_id").toString());
            inst->set_api_secret(settings.value("api_secret").toString());
            inst->set_server(settings.value("server").toString());
            inst->set_org_key(settings.value("org_key").toString());

            if (inst->isValid()) {            // Add instance to the instance's List widget
                QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
                item->setSizeHint(inst->minimumSizeHint());
                ui->listWidget->setItemWidget(item,inst);
            } else {
                delete inst;
            }
        }
        settings.endArray();

        if (ui->listWidget->count() == 0) {
            on_pushButton_add_clicked();
        }
    }
        break;
    case EdrInstanceType:
    {
        QSettings settings;
        int size = settings.beginReadArray("instances_edr");
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);


            auto *inst = new InstanceEdr(this);
            inst->set_name(settings.value("name").toString());
            inst->set_api(settings.value("api").toString());
            inst->set_url(settings.value("url").toString());

            if (inst->isValid()) {            // Add instance to the instance's List widget
                QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
                item->setSizeHint(inst->minimumSizeHint());
                ui->listWidget->setItemWidget(item,inst);
            } else {
                delete inst;
            }
        }
        settings.endArray();

        if (ui->listWidget->count() == 0) {
            on_pushButton_add_clicked();
        }
    }
        break;
    case AppcontrolInstanceType:
    {
        QSettings settings;
        int size = settings.beginReadArray("instances_appcontrol");
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);


            auto *inst = new InstanceAppcontrol(this);
            inst->set_name(settings.value("name").toString());
            inst->set_api(settings.value("api").toString());
            inst->set_url(settings.value("url").toString());

            if (inst->isValid()) {            // Add instance to the instance's List widget
                QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
                item->setSizeHint(inst->minimumSizeHint());
                ui->listWidget->setItemWidget(item,inst);
            } else {
                delete inst;
            }
        }
        settings.endArray();

        if (ui->listWidget->count() == 0) {
            on_pushButton_add_clicked();
        }
    }
        break;
    default:
        qDebug("Unknown Instance type");
        break;
    }
}
