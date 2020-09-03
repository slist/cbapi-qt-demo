// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef DIFFTOOL_H
#define DIFFTOOL_H

#include <QDialog>

namespace Ui {
class DiffTool;
}

class DiffTool : public QDialog
{
    Q_OBJECT

public:
    explicit DiffTool(QWidget *parent = 0);
    ~DiffTool();

private slots:
    void on_buttonBox_accepted();
    void on_pushButton_clicked();

    void on_lineEdit_tool_textChanged(const QString &arg1);

private:
    Ui::DiffTool *ui;
    QString os;
};

#endif // DIFFTOOL_H
