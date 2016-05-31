#include "watch.h"
#include "ui_watch.h"

/*
 * 内存占用颜色在timeout()函数里改
 * 网速的文字颜色，背景颜色等在watch.ui文件的up和down控件里修改stylesheet就可以
 * 当然也可以直接用代码修改，参考内存的写法，控件分别是ui->up和ui->down
 *
 *
*/

watch::watch(bool memshowed ,bool netshowed ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::watch)
{
    ui->setupUi(this);
    setLayout(ui->horizontalLayout);

    memshow=memshowed;
    netshow=netshowed;
    if((netshow^memshow)||netshow){
        ui->mem->setVisible(memshow);
        ui->up->setVisible(netshow);
        ui->down->setVisible(netshow);
    }
    firstrun=true;
    oup=0;
    odown=0;
    pro = new QProcess(this);
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(false);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    timer->start();
}

watch::~watch()
{
    delete ui;
}

void watch::setmemshow(bool show)
{
    memshow=show;
    ui->mem->setVisible(show);
}

void watch::setnetshow(bool show)
{
    netshow=show;
    ui->up->setVisible(show);
    ui->down->setVisible(show);

}
void watch::timeout()
{
    QRegularExpressionMatchIterator matchs;
    QString result,tmp;
    qint64 up,down,t;
    up=0;
    down=0;
    if(netshow)
    {
        pro->start("ip -s link");
        if(pro->waitForFinished(1000))
        {
            result=pro->readAllStandardOutput();
          //  qDebug()<<result;
            if(!result.isEmpty())
            {
                result.remove(QRegularExpression("link/(?!loopback|ether)[\\S\\s]+?collsns[\\S\\s]+?(\\d+ +){6}"));
                reg.setPattern("collsns[\\s\\S]+?\\d+");
                matchs=reg.globalMatch(result);
                while(matchs.hasNext())
                {
                    tmp=matchs.next().captured(0);
                    tmp.remove(QRegularExpression("[\\s\\S]+ "));
                    t=tmp.toInt();
                    up+=t;
                }
               // qDebug()<<"got up-data"<<up<<"oup"<<oup;
                QString text;
                t=up-oup;
                t=t/1024;
                if(t>999)
                {
                    t=t*100/1024;
                    float f=t*1.0/100;
                    text=""+QString().setNum(f)+"m";

                }
                else text+=""+QString().setNum(t)+"k";
                if(oup!=0) ui->up->setText(text);
                oup=up;

                reg.setPattern("mcast[\\s\\S]+?\\d+");
                matchs=reg.globalMatch(result);
                while(matchs.hasNext())
                {
                    tmp=matchs.next().captured(0);
                    tmp.remove(QRegularExpression("[\\s\\S]+ "));
                    t=tmp.toLongLong();
                    down+=t;
                   // qDebug()<<"got down"<<tmp;
                }
               // qDebug()<<"got down-data"<<down<<"odown"<<odown;
                t=down-odown;
                t=t/1024;
                if(t>999)
                {
                    t=t*100/1024;
                    float f=t*1.0/100;
                    text=""+QString().setNum(f)+"m";

                }
                else text=""+QString().setNum(t)+"k";

                if(odown!=0) ui->down->setText(text);
                odown=down;
            }
        }

    }
    else {
        oup=0;
        odown=0;
    }
    if(!memshow){
        firstrun=true;
        return;
       //不显示内存就直接退出了
    }
     pro->start("free -m");
    if(pro->waitForFinished(1000))
    {
        result=pro->readAllStandardOutput();
      //  qDebug()<<result;
        if(!result.isEmpty())
        {
            qint64 total,used;
            int per=0;
            total=0;
            used=0;
            reg.setPattern("\\d+.+");
            //reg.setPattern("Mem:.+");
            tmp=reg.match(result).captured(0);
            qDebug()<<tmp;
            QStringList list=tmp.split(QRegularExpression(" +"));
            result=list.at(0);//total
            tmp=list.at(5);//available
            total=result.toLongLong();
            used=tmp.toLongLong();
            per=used*1000/total;
            per=1000-per;
            int m=per%10;
            per/=10;
            if(m>=5) per+=1;//四舍五入
            int omem=ui->mem->value();

            //============================【内存占用颜色在这改】================================
            if(per>=65&&(omem>=80||omem<65||firstrun))//内存占用较多时的颜色！默认橙色
            {
                QString style;
                style="QProgressBar {\
                       background-color:#D26900;\
                       }\
                       QProgressBar::chunk {\
                        background-color: #FF9224;\
                       }";
                ui->mem->setStyleSheet(style);
            }
            else if(per<65&&(omem>=65||firstrun))//内存降回正常状态时的颜色！默认蓝色
            {
                QString style;
                style="QProgressBar {\
                        background-color:rgb(46, 141, 175) ;\
                        }\
                        QProgressBar::chunk {\
                         background-color:  rgb(74, 183, 255);\
                        }";
                ui->mem->setStyleSheet(style);

            }
            else if(per>=80&&(omem<80||firstrun))//内存大时的颜色！默认红色
            {
                QString style;
                style="QProgressBar {\
                       background-color:#AE0000;\
                       }\
                       QProgressBar::chunk {\
                        background-color: #FF0000;\
                       }";
                ui->mem->setStyleSheet(style);

            }
            ui->mem->setValue(per);
            firstrun = false;
           // qDebug()<<total<<used<<used*1.0/total;
        }
    }

}
