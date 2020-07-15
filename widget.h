#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QDebug>

#include <QKeyEvent>

#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <QWebEngineView>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void loadImage(QByteArray baTmp);
    void connectSlots();

public slots:

private slots:
    void on_pushButton_conn_clicked();
    void on_pushButton_disconn_clicked();

private:
    Ui::Widget *ui;
    QWebEngineView *imgView;
    QNetworkAccessManager *manager;
    QTcpSocket *camSocket;
    QUrl url;

    struct {
        int Velocity;
        int TurnDegree;
    }carInfo;

    void keyPressEvent(QKeyEvent *event);

};
#endif // WIDGET_H
