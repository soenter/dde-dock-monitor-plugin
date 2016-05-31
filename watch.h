#ifndef WATCH_H
#define WATCH_H

#include <QWidget>
#include<QProcess>
#include<QTimer>
#include<QRegularExpression>
#include<QDebug>
#include<QMouseEvent>
#include<QDesktopWidget>
#include<QSystemTrayIcon>
#include<QMenu>
namespace Ui {
class watch;
}

class watch : public QWidget
{
    Q_OBJECT

public:
    explicit watch(bool memshowed = true,bool netshowed = true,QWidget *parent = 0);
    ~watch();
    void setmemshow(bool);
    void setnetshow(bool);
private slots:
    void timeout();
    void openmoniter();
private:
//    void mousePressEvent(QMouseEvent* event);
//    void mouseMoveEvent(QMouseEvent * event);
//    void mouseReleaseEvent(QMouseEvent * event);
//    void contextMenuEvent(QContextMenuEvent *);
//    void creattrayicon();

    Ui::watch *ui;
    QProcess *pro;
    QTimer *timer;
    QRegularExpression reg;
    qint64 oup,odown;
//    QPoint oldpos;
    bool memshow;
    bool netshow;
    bool firstrun;
//    bool mousepressed;
//    QSystemTrayIcon *trayIcon;//托盘图标
//    QAction *tuichu;
//    QAction *moniter;

};

#endif // WATCH_H
