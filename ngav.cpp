// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include "ngav.h"
#include "ui_ngav.h"

#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QProcess>
#include <QComboBox>
#include <QStandardPaths>

#include "difftool.h"
#include "copy.h"
#include "policies.h"

const int max_inst_display = 2;

Ngav::Ngav(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ngav)
{
    ui->setupUi(this);
    ui->checkBox_show_logs->setVisible(false);
    start();
}

Ngav::~Ngav()
{
    delete ui;
}

void Ngav::on_pushButton_connect_clicked()
{
    // clear policy in selected policy table
    ui->tableWidget_selection->clearContents();
    ui->tableWidget_selection->setRowCount(0);
    updateButtons();
    emit refresh();
}

void Ngav::on_checkBox_show_logs_stateChanged(int checked)
{
    if (checked) {
        ui->logTextEdit->setVisible(true);
    } else {
        ui->logTextEdit->setVisible(false);
    }
}

void Ngav::log(const QString & text)
{
    ui->checkBox_show_logs->setVisible(true);
    ui->checkBox_show_logs->setChecked(true);
    ui->logTextEdit->append(text);
}

void Ngav::on_pushButton_compare_clicked()
{
    QSettings settings;

    QString program(settings.value("diff_tool","").toString());

    if (QFile(program).exists() == false) {
        ui->checkBox_show_logs->setVisible(true);
        ui->checkBox_show_logs->setChecked(true);
        ui->logTextEdit->append(tr("Please set a diff tool"));
        return;
    }

    QProcess *myProcess = new QProcess(this);
    QStringList arguments;
    for (int row=0; row < ui->tableWidget_selection->rowCount(); row++)
    {
        QString f;
        f += QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        f += QString("/%1_%2.json").arg(ui->tableWidget_selection->item(row, 0)->text()).arg(ui->tableWidget_selection->item(row, 1)->text());
        arguments << f;
    }
    myProcess->start(program, arguments);
}

QStringList Ngav::get_instances_list()
{
    QStringList list;

    QSettings settings;
    int size = settings.beginReadArray("instances");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        list << settings.value("name").toString();
    }
    settings.endArray();
    return list;
}

void Ngav::add_pol(const QString & a, const QString & b)
{
    int row = ui->tableWidget_selection->rowCount() + 1;
    ui->tableWidget_selection->setRowCount(row--);

    QTableWidgetItem *newItem_inst= new QTableWidgetItem(a);
    QTableWidgetItem *newItem_pol = new QTableWidgetItem(b);
    ui->tableWidget_selection->setItem(row, 0, newItem_inst);
    ui->tableWidget_selection->setItem(row, 1, newItem_pol);

    updateButtons();
    ui->tableWidget_selection->resizeColumnToContents(0);
}

void Ngav::del_pol(const QString & a, const QString & b)
{
    for (int i=0;i < ui->tableWidget_selection->rowCount(); i++) {
        if ((a == ui->tableWidget_selection->item(i,0)->text()) && (b == ui->tableWidget_selection->item(i,1)->text())) {
            ui->tableWidget_selection->removeRow(i);
            updateButtons();
            return;
        }
    }
    log("Can't delete, not found" + a + " -- " + b);
    updateButtons();
}

void Ngav::updateButtons()
{
    ui->pushButton_unselect->setEnabled(true);
    ui->pushButton_copy->setEnabled(true);

    int rows = ui->tableWidget_selection->rowCount();
    switch (rows) {
    case 0:
        ui->pushButton_compare->setEnabled(false);
        ui->pushButton_copy->setEnabled(false);
        ui->pushButton_unselect->setEnabled(false);
        break;
    case 1:
        ui->pushButton_compare->setEnabled(false);
        break;
    case 2:
    case 3:
        ui->pushButton_compare->setEnabled(true);

        break;
    default:
        ui->pushButton_compare->setEnabled(false);
    }
}

void Ngav::on_pushButton_unselect_clicked()
{
    emit unselect_all();
    ui->tableWidget_selection->clearContents();
    ui->tableWidget_selection->setRowCount(0);
}

void Ngav::start()
{
    ui->logTextEdit->setVisible(false);
    ui->tableWidget_selection->horizontalHeader()->setStretchLastSection(true);
    ui->pushButton_compare->setToolTip(tr("Max 3 files"));

    updateButtons();

    inst_list = get_instances_list();

    int n = qMin(inst_list.size(), max_inst_display);

    // Create n instances !
    for (int i=0; i<n; i++)
    {
        QVBoxLayout *layout = new QVBoxLayout();
        QComboBox * qcb = new QComboBox(this);
        Policies  * pol = new Policies(this);

        Widgets.append(qcb);
        Widgets.append(pol);

        for (int inst=0; inst<inst_list.size(); inst++) {
            qcb->addItem(inst_list[inst]);
        }
        qcb->setCurrentIndex(i);
        layout->addWidget(qcb);
        layout->addWidget(pol);
        ui->horizontalLayout_policies->addLayout(layout);

        // connect combobox to policies...
        connect(qcb, SIGNAL(currentTextChanged(const QString &)), pol, SLOT(set_instance_name(const QString &)));

        // connect policies to Ngav
        connect(pol, SIGNAL(pol_selected(const QString &, const QString &)), this, SLOT(add_pol(const QString &, const QString &)));
        connect(pol, SIGNAL(pol_deselected(const QString &, const QString &)), this, SLOT(del_pol(const QString &, const QString &)));
        connect(pol, SIGNAL(log(const QString &)), this, SLOT(log(const QString &)));

        // connect Ngav to policies
        connect(this, SIGNAL(unselect_all()), pol, SLOT(unselect_all()));
        connect(this, SIGNAL(refresh()), pol, SLOT(refresh()));

        pol->set_instance_name(inst_list[i]);
    }
}

void Ngav::clear()
{
    foreach(QWidget * child, Widgets)
    {
        delete child;
    }
    Widgets.clear();
}

void Ngav::on_pushButton_copy_clicked()
{
    //int res;
    Copy *copy_window = new Copy(this);

    connect (copy_window, SIGNAL(log(const QString &)), this, SLOT(log(const QString &)));
    copy_window->setModal(true);
    copy_window->set_inst_list(inst_list);

    QStringList arguments;
    for (int row=0; row < ui->tableWidget_selection->rowCount(); row++)
    {
        copy_window->add_pol(ui->tableWidget_selection->item(row, 0)->text(), ui->tableWidget_selection->item(row, 1)->text());
    }

    /* res = */ copy_window->exec();
    on_pushButton_connect_clicked();
}

void Ngav::on_pushButton_difftool_clicked()
{
    DiffTool *d = new DiffTool(this);
    d->setModal(true);
    d->show();
}
