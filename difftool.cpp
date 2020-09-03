// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>

#include "difftool.h"
#include "ui_difftool.h"

DiffTool::DiffTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiffTool)
{
    ui->setupUi(this);

    QSettings settings;
    os = QSysInfo::prettyProductName();

    ui->plainTextEdit->setReadOnly(true);

    ui->lineEdit_tool->setProperty("mandatoryField", true);
    ui->lineEdit_tool->setText(settings.value("diff_tool","").toString());

    if (ui->lineEdit_tool->text().isEmpty()) {
        QString program;

        if (QFile("/usr/bin/meld").exists()) {
            program = "/usr/bin/meld";
        } else if (QFile("/usr/bin/kompare").exists()) {
            program = "/usr/bin/kompare";
        } else if (QFile("/usr/bin/gvimdiff").exists()) {
            program = "/usr/bin/gvimdiff";
        }
        ui->lineEdit_tool->setText(program);
    }

    if (os.contains("Ubuntu", Qt::CaseInsensitive)) {
        ui->plainTextEdit->appendPlainText(tr("Please install a graphical diff tool.\n"));
        ui->plainTextEdit->appendHtml("<pre>sudo apt-get install meld     --&gt; provides /usr/bin/meld</pre>");
        ui->plainTextEdit->appendHtml("<pre>sudo apt-get install kompare  --&gt; provides /usr/bin/kompare</pre>");
        ui->plainTextEdit->appendHtml("<pre>sudo apt-get install vim-gtk  --&gt; provides /usr/bin/gvimdiff</pre>");
        ui->plainTextEdit->appendHtml("<pre>sudo apt-get install kdiff3   --&gt; provides /usr/bin/kdiff3</pre>");
    } else if (os.contains("Centos", Qt::CaseInsensitive)) {
        ui->plainTextEdit->appendPlainText(tr("Please install a graphical diff tool.\n"));
        ui->plainTextEdit->appendPlainText(tr("For example gvimdiff : /usr/bin/gvimdiff\n"));
        ui->plainTextEdit->appendPlainText("\n");
        ui->plainTextEdit->appendPlainText("sudo yum install vim-X11  --> provides /usr/bin/gvimdiff\n");
    } else if (os.contains("Windows", Qt::CaseInsensitive)) {
        ui->plainTextEdit->appendPlainText(tr("Please install a graphical diff tool.\n"));
        ui->plainTextEdit->appendPlainText(tr("For example kdiff3, WinMerge...\n\n"));
        ui->plainTextEdit->appendPlainText(tr("C:\\Program Files\\KDiff3\\kdiff3.exe\n"));
        ui->plainTextEdit->appendPlainText(tr("C:\\Program Files (x86)\\WinMerge\\WinMergeU.exe\n"));
    } else {
        ui->plainTextEdit->appendPlainText(tr("Select a graphical diff tool"));
        ui->plainTextEdit->appendPlainText(tr("For example gvimdiff"));
    }
}

DiffTool::~DiffTool()
{
    delete ui;
}

void DiffTool::on_buttonBox_accepted()
{
    QSettings settings;
    settings.setValue("diff_tool", ui->lineEdit_tool->text());
}

void DiffTool::on_pushButton_clicked()
{
    QString f;
    if (os.contains("windows", Qt::CaseInsensitive)) {
        f = QFileDialog::getOpenFileName(this, tr("Choose File"),  QDir::homePath(), "*.exe");
    } else {
        f = QFileDialog::getOpenFileName(this, tr("Choose File"),  QDir::homePath(), "*");
    }
    if (f.isNull())
        return;

    ui->lineEdit_tool->setText(f);
}

void DiffTool::on_lineEdit_tool_textChanged(const QString &arg1)
{
    if (QFile(arg1).exists()) {
        ui->lineEdit_tool->setStyleSheet("QLineEdit { border: 1px solid green }");
    } else {
        ui->lineEdit_tool->setStyleSheet("QLineEdit { border: 2px solid red }");
    }
}
