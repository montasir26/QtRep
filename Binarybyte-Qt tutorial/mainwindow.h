#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSql>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QDebug>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void onnameclicked();
public:
    static int x;
    static QString pro_user;
    QSqlDatabase mydb;
    void connClose(){

        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connOpen(){
        mydb=QSqlDatabase::addDatabase("QSQLITE");
       mydb.setDatabaseName("C:/Users/Asus/Desktop/tutorial/tutorial/tutorial/db/test.db");

       if(!mydb.open()){

           qDebug()<<("File not opened");
           return false;
       }else {
           qDebug()<<("File opened");
           return true;

       }
    }

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_upload_pic_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
