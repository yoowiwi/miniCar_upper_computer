#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager;
    camSocket = new QTcpSocket;
    imgView = new QWebEngineView();
    ui->Layout_img->addWidget(imgView);
    imgView->setMinimumSize(ui->groupBox_img->size());

    connectSlots();
}

Widget::~Widget()
{
    delete imgView;
    delete manager;
    delete camSocket;
    delete ui;
}

void Widget::loadImage(QByteArray baTmp)
{
//    int lenTmp = QString("<IMG SRC='").length();
//    int indexTmp = baTmp.indexOf("<IMG SRC='");
//    int index2Tmp = baTmp.indexOf("' style='",indexTmp);
//    QUrl imgUrl(baTmp.mid(indexTmp + lenTmp,index2Tmp-indexTmp- lenTmp));
    QUrl imgUrl(url.toString() + ":81/stream");
    imgView->load(imgUrl);

}

void Widget::connectSlots()
{
    connect(ui->radioButton_off,&QRadioButton::clicked,this,[this](){
        manager->get(QNetworkRequest(QUrl(url.toString() + "/ledturn?signal=2")));
    });
    connect(ui->radioButton_left,&QRadioButton::clicked,this,[this](){
        manager->get(QNetworkRequest(QUrl(url.toString() + "/ledturn?signal=1")));
    });
    connect(ui->radioButton_right,&QRadioButton::clicked,this,[this](){
        manager->get(QNetworkRequest(QUrl(url.toString() + "/ledturn?signal=3")));
    });
    connect(ui->checkBox_led,&QCheckBox::clicked,this,[this](){
        if(ui->checkBox_led->isChecked())
        {
            manager->get(QNetworkRequest(QUrl(url.toString() + "/led?signal=1")));
        }else{
            manager->get(QNetworkRequest(QUrl(url.toString() + "/led?signal=2")));
        }
    });

    connect(manager,&QNetworkAccessManager::finished,this,[this](QNetworkReply *reply){
        if(reply->error() == reply->NoError)
        {
            loadImage(reply->readAll());
            ui->pushButton_conn->setEnabled(false);
            ui->comboBox_ip->setEnabled(false);
            ui->pushButton_disconn->setEnabled(true);
            ui->groupBox_Ctrl->setEnabled(true);
            ui->label_ipstatus->setText(ui->comboBox_ip->currentText());
            QMessageBox::information(this,"TIP","Connection Successful!");
        }else{
            QMessageBox::information(this,"TIP"," Connection Failed!\n Please Try Again");
        }
    },Qt::UniqueConnection);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        if(carInfo.Velocity < 0) carInfo.Velocity = 0;
        if((carInfo.Velocity+=5) >= 90)
        {
            carInfo.Velocity = 90;
        }
        break;

    case Qt::Key_Down:
        if(carInfo.Velocity > 0) carInfo.Velocity = 0;
        if((carInfo.Velocity-=5) <= -90)
        {
            carInfo.Velocity = -90;
        }
        break;

    case Qt::Key_Left:
        if((carInfo.TurnDegree-=5) <= -45)
        {
            carInfo.TurnDegree = -45;
        }
        break;

    case Qt::Key_Right:
        if((carInfo.TurnDegree+=5) >= 45)
        {
            carInfo.TurnDegree = 45;
        }
        break;

    case Qt::Key_W:
        ui->checkBox_led->toggle();
        emit ui->checkBox_led->clicked();
        break;

    case Qt::Key_A:
        ui->radioButton_left->setChecked(true);
        emit ui->radioButton_left->clicked();
        break;

    case Qt::Key_S:
        ui->radioButton_off->setChecked(true);
        emit ui->radioButton_off->clicked();
        break;

    case Qt::Key_D:
        ui->radioButton_right->setChecked(true);
        emit ui->radioButton_right->clicked();
        break;

    default:
        break;
    }
    ui->lcdNumber_TurnDegree->display(carInfo.TurnDegree);
    ui->lcdNumber_Velocity->display(carInfo.Velocity);

}


void Widget::on_pushButton_conn_clicked()
{
    url.clear();
    url ="http://" + ui->comboBox_ip->currentText();
    manager->get(QNetworkRequest(url));
}

void Widget::on_pushButton_disconn_clicked()
{
    imgView->load(QUrl(""));
    ui->pushButton_conn->setEnabled(true);
    ui->comboBox_ip->setEnabled(true);
    ui->groupBox_Ctrl->setEnabled(false);
    ui->pushButton_disconn->setEnabled(false);
    ui->label_ipstatus->setText("Unknown");
}
