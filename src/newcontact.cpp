#include "newcontact.h"
#include "ui_newcontact.h"

#include"mainwindow.h"
#include"trie.h"
#include"triephn.h"
#include"splay.h"
#include <QRegularExpression>
#include <QMessageBox>


extern Node trie;
extern Node* root;
extern NodePhn triePhn;
extern NodePhn* rootPhn;
extern SplayNode* rootSplay;
extern SplayNode splayTree;

newcontact::newcontact(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::newcontact)
{
    ui->setupUi(this);
}

newcontact::~newcontact()
{
    delete ui;
}


void newcontact::on_Save_clicked()
{
    QString lastName = ui->lastName->text().toLower();
    QString company = ui->company->text().toLower();
    QString email = ui->email->text().toLower();
    QString name = ui->name->text().toLower();

    if (name.isEmpty())
    {
        QMessageBox::warning(this, "New Contact", "Invalid name.");
        return;
    }

    QString phnNo = ui->phoneNo->text();
    QRegularExpression phoneRegex("^[6-9]\\d{9}$");
    if (!phoneRegex.match(phnNo).hasMatch())
    {
        QMessageBox::warning(this, "New Contact", "Invalid phone number. The number must be exactly 10 digits long and start with a digit between 6-9.");
        return;
    }

    if (!email.isEmpty())
    {
        QRegularExpression emailRegex(R"((^[a-zA-Z]\w*(\.|_)?\w*@\w+(\.\w+)+$))");
        if (!emailRegex.match(email).hasMatch())
        {
            QMessageBox::warning(this, "New Contact", "Invalid email address. The email must start with an alphabet, contain '@' and '.', and cannot end with a '.' character.");
            return;
        }
    }

    hide();

    trie.insertion(root, name, phnNo, email, lastName, company, false);
    rootSplay=splayTree.insertion(rootSplay,name,phnNo,email,lastName,company,false);
    triePhn.insertion(rootPhn, phnNo, name, email, lastName, company, false);

    QWidget* parentWidget = this->parentWidget();
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget);
    if (mainWindow)
    {
        mainWindow->writeToCSV(root, "trie.csv", rootPhn, "triePhn.csv",rootSplay,"splay.csv");
    }
    else
    {
        qDebug() << "MainWindow pointer is null or invalid!";
    }
    hide();
}

void newcontact::on_Cancel_clicked()
{
    hide();
}

