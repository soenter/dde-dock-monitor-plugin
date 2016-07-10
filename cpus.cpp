#include "cpus.h"
#include "ui_cpus.h"

Cpus::Cpus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cpus)
{
    ui->setupUi(this);
    ui->label->setText("CPU10 : 100.0%   CPU99 : 100.0%");
    ui->label->adjustSize();
    adjustSize();
    isone = true;
    timer = new QTimer;
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    getcpus();
}

Cpus::~Cpus()
{
    delete ui;
}

void Cpus::getcpus()
{
    QStringList* l;
    if(isone)
    {
        l=&list;
        isone = false;
    }
    else {
        l = &list2;
        isone = true;
    }
    l->clear();

    file.setFileName("/proc/stat");
    QTextStream in;
    in.setDevice(&file);
   if( file.open(QIODevice::ReadOnly))
   {
       QString data = in.readAll();
       //qDebug()<<data;
       reg.setPattern("cpu\\d.+");
       matchs = reg.globalMatch(data);
       while(matchs.hasNext())
       {
           l->append(matchs.next().captured());
       }
      // qDebug()<<(*l);
       if(list.isEmpty()||list2.isEmpty()) {
           file.close();
           return;
       }
       else {
           QStringList result=getresult(list,list2);
           QString text;
           for(int i=0;i<result.count();i++)
           {
                text +="CPU"+QString().setNum(i)+" : "+result.at(i);
                if(i%2==0) text +="%   ";//0开始
                else text +="%\n";
           }
           if((result.count()-1)%2!=0) text=text.remove(text.length()-1,1);//假如是换行符结尾的去掉换行
           ui->label->setText(text);

       }
   }
   else {
      // qDebug()<<"file open error!";
   }
   file.close();
}

void Cpus::timeout()
{
    timer->stop();
    getcpus();
    if(!this->isHidden()) timer->start();//不显示cpu占用时就停止
}

QStringList  Cpus::getresult(QStringList& l1,QStringList&l2){
    QStringList result;
    QString one,two;
    for(int i=0;i<l1.count();i++)
    {
        one=l1.at(i);
        two=l2.at(i);
        QString res = colculate(one,two);
        if(!res.isEmpty()) {
            //qDebug()<<"cpu"<<i<<":"<<res;
            result.append(res);
        }
    }
    return result;
}

QString  Cpus::colculate(QString& one,QString& two){
    int result;
    QStringList list;
    int sum1=0;
    int sum2=0;
    int idle1=0;
    int idle2=0;
    QList<int> intlist;
    list = one.split(QRegularExpression(" +"));
    for(int i=0;i<list.count();i++)
    {
        QString d=list.at(i);
        int dd=d.toInt();
        if(i==4) idle1=dd;
        intlist.append(dd);
        sum1+=dd;
    }
    list = two.split(QRegularExpression(" +"));
    for(int i=0;i<list.count();i++)
    {
        QString d=list.at(i);
        int dd=d.toInt();
        if(i==4) idle2=dd;
        intlist.append(dd);
        sum2+=dd;
    }
    result =1000 - 1000*(idle2-idle1)/(sum2-sum1);
    QString r;
    r=QString().setNum(result/10)+"."+QString().setNum(result%10);
    return r;
}

void Cpus::start()
{
    timer->start();
}
