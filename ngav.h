#ifndef NGAV_H
#define NGAV_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class Ngav;
}

class Ngav : public QWidget
{
    Q_OBJECT

public:
    explicit Ngav(QWidget *parent = nullptr);
    ~Ngav();

private slots:
    void on_pushButton_connect_clicked();
    void on_pushButton_compare_clicked();
    void on_checkBox_stateChanged(int arg1);
    void on_pushButton_unselect_clicked();
    void on_pushButton_copy_clicked();
    void on_pushButton_difftool_clicked();

private:
    Ui::Ngav *ui;
    QStringList inst_list;
    QStringList get_instances_list();
    void updateButtons();
    void start();
    void clear();
    QList<QWidget *> Widgets;

public slots:
    void add_pol(const QString &, const QString &);
    void del_pol(const QString &, const QString &);
    void log(const QString &);

signals:
    void unselect_all();
    void refresh();
};

#endif // NGAV_H
