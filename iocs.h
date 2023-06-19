// Copyright 2020-2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef IOCS_H
#define IOCS_H

#include <QWidget>

namespace Ui {
class Iocs;
}

class Iocs : public QWidget
{
    Q_OBJECT

public:
    explicit Iocs(QWidget *parent = nullptr);
    ~Iocs();

private:
    Ui::Iocs *ui;
};

#endif // IOCS_H
