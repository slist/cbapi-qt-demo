// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QSettings>

#include "instance.h"
#include "instances.h"
#include "ui_instances.h"

Instances::Instances(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instances)
{
    ui->setupUi(this);

    QSettings settings;
    int size = settings.beginReadArray("instances");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        Instance *inst = new Instance(this);
        inst->set_name(settings.value("name").toString());
        inst->set_api_id(settings.value("api_id").toString());
        inst->set_api_secret(settings.value("api_secret").toString());
        inst->set_server(settings.value("server").toString());

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

Instances::~Instances()
{
    delete ui;
}

void Instances::on_pushButton_add_clicked()
{
    Instance *i = new Instance(this);

    // Add instance to the instance's List widget
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(i->minimumSizeHint());
    ui->listWidget->setItemWidget(item,i);
    ui->listWidget->scrollToBottom();
};

void Instances::on_buttonBox_accepted()
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
            settings.setValue("name", inst->get_name());
            settings.setValue("api_id", inst->get_api_id());
            settings.setValue("api_secret", inst->get_api_secret());
            settings.setValue("server", inst->get_server_display_name());
        }
    }
    settings.endArray();
}

void Instances::on_buttonBox_rejected()
{
}
