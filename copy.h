// Copyright 2020 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef COPY_H
#define COPY_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class Copy;
}

class Copy : public QDialog
{
    Q_OBJECT

public:
    explicit Copy(QWidget *parent = nullptr);
    ~Copy();

private:
    Ui::Copy *ui;
    void send(void);
    int nb_policy_to_copy;

public:
    void set_inst_list(QStringList);
    void add_pol(const QString &, const QString &);

private slots:
    void on_pushButton_copy_clicked();
    void copy_finished(QNetworkReply *);
    void on_pushButton_cancel_clicked();
};

#endif // COPY_H
