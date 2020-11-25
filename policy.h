#ifndef POLICY_H
#define POLICY_H

#include <QWidget>

namespace Ui {
class Policy;
}

class Policy : public QWidget
{
    Q_OBJECT

public:
    explicit Policy(QWidget *parent = 0);
    ~Policy();
    void setName(const QString name);
    void setDescription(const QString description);
    void setUi(bool b);
    void setGoLive(bool b);
    void setLog(bool b);
    void setRulesNumber(int);

private slots:
    void on_checkBox_stateChanged(int arg1);
    void unselect();

private:
    Ui::Policy *ui;
    QString name;
    QString description;

signals:
    void pol_selected(const QString &);
    void pol_deselected(const QString &);
};

#endif // POLICY_H
