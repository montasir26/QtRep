#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QFrame>
#include <QSqlRecord>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>

//at first we have to change the object name of scroll area from scrollAreaWidgetContents to scrollContents...

QString MainWindow:: pro_user;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    connOpen();

    //Creating Modal...
    QSqlQueryModel *modal= new QSqlQueryModel();
    QSqlQueryModel *modal1= new QSqlQueryModel();
    QSqlQueryModel *modal2= new QSqlQueryModel();    //Creating Modal...

    //Creating query and assigning database to it...
    QSqlQuery *qry= new QSqlQuery(mydb);
    QSqlQuery *qry1= new QSqlQuery(mydb);
    QSqlQuery *qry3= new QSqlQuery(mydb);//Creating query and assigning database to it...

    //Preparing the queries with the necessary fetched data...
    qry->prepare("select name from users");
    qry1->prepare("select bg from users");
    qry3->prepare("select Address from users");    //Preparing the queries with the necessary fetched data...

    //Executing the queries...
    qry->exec();
    qry1->exec();
    qry3->exec();//Executing the queries...

    //Assigning the queries to the previuosly created modals...
    modal->setQuery(*qry);
    modal1->setQuery(*qry1);
    modal2->setQuery(*qry3);//Assigning the queries to the previuosly created modals...

    //Creating a grid layout...
    QGridLayout *lay=new QGridLayout(this);
    //Creating an array of button...
    QPushButton *name[100];
    //running a loop to add the desired components to the scroll area...
    for(int j=0;j<=modal->rowCount()-1;j++)
    {
        //Assigning the desired information from the previously prepared modals...
    QString str=modal->record(j).value(0).toString();//(name)
    QString str1=modal1->record(j).value(0).toString();//(blood group)
    QString str2=modal2->record(j).value(0).toString();//(address)

    //Assigning the desired information from the previously prepared modals...

    //Assigning the strings to widgets...
    name[j]=new QPushButton(str);
    QLabel *lab=new QLabel("Blood Group: "+str1+", Address: "+str2+"."); //Assigning the strings to widgets...
    lab->setStyleSheet("color:white");
    QFrame *line;

    //Creating horizontal line with desired properties...
    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background:white"); //Creating horizontal line with desired properties...

    //Setting up the pushbutton with the on-hover changes...
    name[j]->setObjectName("btnName_1");
    name[j]->setStyleSheet(
    "   QPushButton#btnName_1 {"
    "     background:transparent; Text-align:left;font-family:century gothic;font-size:18px; color:orange;"
    " }"
    " QPushButton#btnName_1:hover {"
    "     color: yellow;font-size:25px;"
    " }");                                  //Setting up the pushbutton with the on-hover changes...


    lab->setStyleSheet("background:transparent; Text-align:left;font-family:century gothic;font-size:18px; color:white");

     //adding all the widgets to the previously cretaed grid layout...
    lay->addWidget(name[j]);
    lay->addWidget(lab);
    lay->addWidget(line);  //adding all the widgets to the previously cretaed grid layout...


    //connecting the button containg name with the onnameclicked function...
     connect(name[j],SIGNAL(clicked()),this,SLOT(onnameclicked()));
     ui->scrollContents->setLayout(lay);
    }

    //putting the layout inside the scroll area...

    connClose();

    //hiding the frame that will show the profile...
    ui->frame_2->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}
QString buttonText;
//assigning zero to the static int created in header...
int MainWindow::x=0;
//creating an array of button, vector can be used as well...
QPushButton *pButton[100];

//this is the function that will be called if any name is clicked...
void MainWindow::onnameclicked()
{
    //here we are decreasing the width of the frame containing the scroll area so that we  can show the profile...
    ui->frame->setGeometry(300,50,521,581);

    //making the profile containing frame visible...
    ui->frame_2->show();

    //initializing the previously created button...
    pButton[x]=new QPushButton;

    //recieving the button (name) clicked and assigning it to the pButton[x]...
    pButton[x] = qobject_cast<QPushButton*>(sender());

    //we have increased the value of x by 1 each time. so if the next name is clicked the previous name will go back to the default stylesheet in the codes below...
    if(x>0)
    {

        pButton[x-1]->setObjectName("btnName_1");
        pButton[x-1]->setStyleSheet("   QPushButton#btnName_1 {"
                                    "     background:transparent; Text-align:left;font-family:century gothic;font-size:18px; color:orange;"
                                    " }"
                                    " QPushButton#btnName_1:hover {"
                                    "     color: yellow;font-size:25px;"
                                    " }");
    }


    //style sheet for the clicked button...
    pButton[x]->setStyleSheet("color: yellow;font-size:25px;Text-align:left;font-family:century gothic");

    //assigning the text of the button to the string "buttonText"..
    buttonText = pButton[x]->text();

    // recognize buttonText here
    ui->label_3->setText(buttonText);
    ui->label_3->setStyleSheet("font-family:century gothic;background:transparent;font-size:20px;color:orange;Text-align:center");

    //fetching the necessary informations from the database and assigning them to the desired labels...
    connOpen();
        QSqlQuery qry;
        qry.prepare("select * from users where name='"+buttonText+"'");
        if(qry.exec())
        while(qry.next()){
            QPixmap pro_pic;
            QString location=qry.value(10).toString();
            pro_pic.load(location);
            ui->label->setPixmap(pro_pic);
            ui->label->setScaledContents(true);
            ui->label_4->setText("<font color=white><u>Phone</u>: (</font>"+qry.value(8).toString()+"<font color=white>)</font>");
        }

        //increment of the value x by 1;
        x++;
    connClose();

}


//this is where we upload profile picture...
void MainWindow::on_upload_pic_clicked()
{
    QString picpath=QFileDialog::getOpenFileName
            (
                this,
            tr("Open File"),
            "C://",
            "All File (*.*);;JPG File (*.jpg);;PNG File (*.png)"
                );

    connOpen();
    QSqlQuery qry;
    qry.prepare("update users set propic ='"+picpath+"'where name='"+buttonText+"'");
    if(qry.exec()){

       QMessageBox::information(this,tr("Update Result"),tr("<font color=black>Profile Photo Update Completed"));
    connClose();
    connOpen();
    QSqlQuery qry;
    qry.prepare("select * from users where name='"+buttonText+"'");
    if(qry.exec())
        while(qry.next())
        {
            QPixmap pro_pic;
            QString location=qry.value(10).toString();
            pro_pic.load(location);
            ui->label->setPixmap(pro_pic);
            ui->label->setScaledContents(true);
        }
    connClose();
    }
}

void MainWindow::on_pushButton_clicked()
{
    connOpen();
    QSqlQueryModel *modal= new QSqlQueryModel();
    QSqlQueryModel *modal1= new QSqlQueryModel();
    QSqlQueryModel *modal2= new QSqlQueryModel();

    QSqlQuery *qry= new QSqlQuery(mydb);
    QSqlQuery *qry1= new QSqlQuery(mydb);
    QSqlQuery *qry3= new QSqlQuery(mydb);

    qry->prepare("select name from users");
    qry1->prepare("select bg from users");
    qry3->prepare("select Address from users");

    qry->exec();
    qry1->exec();
    qry3->exec();
    modal->setQuery(*qry);
    modal1->setQuery(*qry1);
    modal2->setQuery(*qry3);
    QVBoxLayout *lay=new QVBoxLayout(this);
    QString s=ui->lineEdit->text();
    QPushButton *label;
    for(int j=0;j<=modal->rowCount();j++)
    {
        QString str=modal->record(j).value(0).toString();
        QString str1=modal1->record(j).value(0).toString();
        QString str2=modal2->record(j).value(0).toString();



        //checking for match
        int x = QString::compare(s, str1, Qt::CaseInsensitive);
        //if matches....
        if(x==0)
        {
            //clearing previous layout
            if ( ui->scrollContents->layout() != NULL )
            {
                QLayoutItem* item;
                while ( ( item = ui->scrollContents->layout()->takeAt( 0 ) ) != NULL )
                {
                    delete item->widget();
                    delete item;
                }
                delete ui->scrollContents->layout();
            }

            //setting up new layout
            ui->frame_2->hide();


            ui->frame->setGeometry(300,50,891,581);



            label=new QPushButton(str);
            label->setObjectName(str);

            QLabel *lab=new QLabel("Blood Group: "+str1+", Address: "+str2+".");
            lab->setStyleSheet("color:white");
            QFrame *line=new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            line->setStyleSheet("background:white");

            label->setStyleSheet(
                        "   QPushButton {"
                        "     background:transparent; Text-align:left;font-family:century gothic;font-size:18px; color:orange;"
                        " }"
                        " QPushButton:hover {"
                        "     color: rgb(224, 255, 0);font-size:25px;"
                        " }");
            lay->addWidget(label);
            lay->addWidget(lab);
            lay->addWidget(line);
            connect(label,SIGNAL(clicked()),this,SLOT(onnameclicked()));

        }
    }

    ui->scrollContents->setLayout(lay);
    //ui->scrollContents->setStyleSheet("background: transparent;border-color: rgb(62, 62, 62);padding-left:20px;padding-top:20px");
    ui->scrollContents->setVisible(true);
    ui->scrollContents->show();
}

