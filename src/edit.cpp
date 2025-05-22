#include "edit.h"
#include "ui_edit.h"

#include "trie.h"
#include "triephn.h"
#include "mainwindow.h"

#include <QRegularExpression>
#include <QMessageBox>

extern Node trie;
extern Node* root;
extern NodePhn triePhn;
extern NodePhn* rootPhn;
extern SplayNode* rootSplay;
extern SplayNode splayTree;

edit::edit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::edit)
{
    ui->setupUi(this);
}

edit::~edit()
{
    delete ui;
}

extern QString names;
extern QString phnNo;

void edit::getInput(QString name, QString phone)
{
    names = name;
    phnNo = phone;

    QString modifiedName = name;
    if (!modifiedName.isEmpty())
    {
        modifiedName[0] = modifiedName[0].toUpper();
    }
    ui->nameLabel->setText(modifiedName);
    ui->phnNoLabel->setText(phone);
}

void edit::on_save_clicked()
{
    QString lastName = ui->lastName->text().toLower();
    QString company = ui->company->text().toLower();
    QString email = ui->email->text().toLower();

    if (!email.isEmpty())
    {
        QRegularExpression emailRegex(R"((^[a-zA-Z]\w*(\.|_)?\w*@\w+(\.\w+)+$))");
        if (!emailRegex.match(email).hasMatch())
        {
            QMessageBox::warning(this, "Edit Contact", "Invalid email address. The email must start with an alphabet, contain '@' and '.', and cannot end with a '.' character.");
            return;
        }
    }

    hide();

    if (!trie.editContact(root, names, email, lastName, company))
    {
        qDebug() << "Failed to update contact in the name trie.";
    }

    if (!triePhn.editContact(rootPhn, phnNo, email, lastName, company))
    {
        qDebug() << "Failed to update contact in the phone trie.";
    }

    QWidget* parentWidget = this->parentWidget();
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget->parentWidget());

    if (mainWindow)
    {
        qDebug() << "Saving updated data to CSV files...";
        mainWindow->writeToCSV(root, "trie.csv", rootPhn, "triePhn.csv", rootSplay,"splay.csv");
    }
    else
    {
        qDebug() << "MainWindow pointer is null or invalid!";
    }
    mainWindow->refreshContacts();
}


void edit::on_cancel_clicked()
{
    hide();
}
