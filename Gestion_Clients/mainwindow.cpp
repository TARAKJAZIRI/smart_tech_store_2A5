#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <QMessageBox>
#include <iostream>
#include <QTextDocument>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>




using std::uint8_t;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(tmpclient.afficherClient());

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_ajouter_clicked()
{
    int cin;
        QString nom,prenom;
        cin=ui->lineEditCin->text().toInt();

        nom=ui->lineEdit_nom->text();
        prenom=ui->lineEdit_pren->text();
       // ui->tableView->setModel(tmpclient.afficherClient());


        Client c(cin,nom,prenom);
      bool test =  c.addClient();
        if(test)
        {


        QMessageBox::information(nullptr, QObject::tr("ajouter un client"),
                          QObject::tr("client ajouté.\n"
                                      "Click Cancel to exit."), QMessageBox::Cancel);

        }
          else{
              QMessageBox::critical(nullptr, QObject::tr("ajouter une client"),
                          QObject::tr("Erreur !.\n"
                                      "Click Cancel to exit."), QMessageBox::Cancel);

        }


}

void MainWindow::on_afficher_clicked()
{
    ui->tableView->setModel(tmpclient.afficherClient());

}

void MainWindow::on_supprimer_clicked()
{
    int cin=ui->lineEditCin_supp->text().toInt();
       bool test = tmpclient.removeClient(cin);
        if(test)
        {


        QMessageBox::information(nullptr, QObject::tr("supprimer un client"),
                          QObject::tr("client supprimé.\n"
                                      "Click Cancel to exit."), QMessageBox::Cancel);

        }
          else{
              QMessageBox::critical(nullptr, QObject::tr("supprimer une client"),
                          QObject::tr("Erreur !.\n"
                                      "Click Cancel to exit."), QMessageBox::Cancel);

        }


}

void MainWindow::on_modifier_clicked()
{
    int cin;
        QString nom,prenom;

        cin=ui->lineEditCin_modif->text().toInt();
        nom=ui->lineEdit_nom_modif->text();
        prenom=ui->lineEdit_pren_modif->text();

    bool test = tmpclient.modifierClient(nom,prenom,cin);

    if(test)
    {


    QMessageBox::information(nullptr, QObject::tr("modifier un client"),
                      QObject::tr("client modifié.\n"
                                  "Click Cancel to exit."), QMessageBox::Cancel);

    }
      else{
          QMessageBox::critical(nullptr, QObject::tr("modifier une client"),
                      QObject::tr("Erreur !.\n"
                                  "Click Cancel to exit."), QMessageBox::Cancel);

    }



}


void MainWindow::on_pushButton_TriASC_clicked()
{
    ui->tableView_tri->setModel(tmpclient.trierAc());
}

void MainWindow::on_pushButton_TriDESC_clicked()
{
    ui->tableView_tri->setModel(tmpclient.trierDec());
}

void MainWindow::on_pushButton_pdf_clicked()
{
    QSqlDatabase db;
                    QTableView table_client;
                    QSqlQueryModel * Modal=new  QSqlQueryModel();

                    QSqlQuery qry;
                     qry.prepare("SELECT * FROM CLIENT ");
                     qry.exec();
                     Modal->setQuery(qry);
                     table_client.setModel(Modal);



                     db.close();


                     QString strStream;
                     QTextStream out(&strStream);


                     const int rowCount = table_client.model()->rowCount();
                     const int columnCount =  table_client.model()->columnCount();


                     const QString strTitle ="Document";


                     out <<  "<html>\n"
                             "<img src='C:/Users/ASUS/Documents/kiosqueK/logocin.png' height='120' width='120'/>"
                         "<head>\n"
                             "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                             "<img src='e-care.png'>"
                         <<  QString("<title>%1</title>\n").arg(strTitle)
                         <<  "</head>\n"
                         "<body bgcolor=#ffffff link=#5000A0>\n"
                        << QString("<h3 style=\" font-size: 50px; font-family: Arial, Helvetica, sans-serif; color: #e80e32; font-weight: lighter; text-align: center;\">%1</h3>\n").arg("Liste des clients")
                        <<"<br>"

                        <<"<table border=1 cellspacing=0 cellpadding=2 width=\"100%\">\n";
                     out << "<thead><tr bgcolor=#f0f0f0>";
                     for (int column = 0; column < columnCount; column++)
                         if (!table_client.isColumnHidden(column))
                             out << QString("<th>%1</th>").arg(table_client.model()->headerData(column, Qt::Horizontal).toString());
                     out << "</tr></thead>\n";

                     for (int row = 0; row < rowCount; row++) {
                         out << "<tr>";
                         for (int column = 0; column < columnCount; column++) {
                             if (!table_client.isColumnHidden(column)) {
                                 QString data = table_client.model()->data(table_client.model()->index(row, column)).toString().simplified();
                                 out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                             }
                         }
                         out << "</tr>\n";
                     }
                     out <<  "</table>\n"
                             "<br><br>"
                             <<"<br>"
                             <<"<table border=1 cellspacing=0 cellpadding=2>\n";


                         out << "<thead><tr bgcolor=#f0f0f0>";

                             out <<  "</table>\n"

                         "</body>\n"
                         "</html>\n";

                     QTextDocument *document = new QTextDocument();
                     document->setHtml(strStream);

                     QPrinter printer;
                     QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
                     if (dialog->exec() == QDialog::Accepted) {

                         QLabel lab;
                          QPixmap pixmap(":/img/aze.png");
                         lab.setPixmap(pixmap);
                         QPainter painter(&lab);
                         //QPrinter printer(QPrinter::PrinterResolution);

                         //pixmap.load("aze.png");
                         //painter.drawPixmap(0,0,this->width(),this->height(),pixmap);
                         //painter.drawPixmap(10,10,50,50, pixmap);

                         document->print(&printer);
                     }

                     printer.setOutputFormat(QPrinter::PdfFormat);
                     printer.setPaperSize(QPrinter::A4);
                     printer.setOutputFileName("/tmp/produit.pdf");
                     printer.setPageMargins(QMarginsF(15, 15, 15, 15));



                     delete document;
}