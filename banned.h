// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef BANNED_H
#define BANNED_H

#include <QWidget>
#include <QMimeData>

namespace Ui {
class Banned;
}

class Banned : public QWidget
{
    Q_OBJECT

public:
    explicit Banned(QWidget *parent = nullptr);
    ~Banned();

private:
    Ui::Banned *ui;
    void addFile(const QString & fileName);


public slots:
    void updateFormatsTable(const QMimeData *mimeData);
    void updateBannedText(const QMimeData *mimeData);

private slots:
    void on_pushButton_save_clicked();
};

#endif // BANNED_H
