#ifndef CPUS_H
#define CPUS_H

#include <QWidget>
#include "QTimer"
#include "QFile"
#include "QTextStream"
#include"QDebug"
#include"QRegularExpression"
namespace Ui {
class Cpus;
}

class Cpus : public QWidget
{
    Q_OBJECT

public:
    explicit Cpus(QWidget *parent = 0);
    ~Cpus();
    void getcpus();
    QStringList getresult(QStringList&,QStringList&);
    QString colculate(QString& one,QString& two);
    void start();
private slots:
    void timeout();
private:
    Ui::Cpus *ui;
    QTimer *timer;
    QFile file;
    QStringList list;
    QStringList list2;
    QRegularExpression reg;
    QRegularExpressionMatchIterator matchs;
    bool isone;
};

#endif // CPUS_H
