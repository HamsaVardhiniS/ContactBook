#include "details.h"
#include "ui_details.h"

#include "trie.h"
#include "triephn.h"
#include "mainwindow.h"
#include "edit.h"

#include <QMessageBox>
#include <QRegularExpression>

extern Node* root;
extern Node trie;
extern NodePhn triePhn;
extern NodePhn* rootPhn;
extern SplayNode* rootSplay;
extern SplayNode splayTree;

QString names;
QString phnNo;

details::details(const QString &name, const QString &phone, const QString &email,
                 const QString &lastName, const QString &company, QWidget *parent)
    : QDialog(parent), ui(new Ui::details), editing(new edit(this))
{
    ui->setupUi(this);
    names=name;
    phnNo=phone;
    ui->nameLabel->setText(name);
    ui->phoneNoLabel->setText(phone);
    ui->emailLabel->setText(email);
    ui->lastNameLabel->setText(lastName);
    ui->companyLabel->setText(company);
}

details::~details()
{
    delete ui;
    delete editing;
}

void details::on_moreOptions_currentIndexChanged(int index)
{
    switch (index)
    {
    case 1:
    {
        QString input = names.toLower();
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Delete Contact", "Are you sure you want to permanently delete this contact?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            bool contactFound = false;

            if (root == nullptr && rootPhn == nullptr)
            {
                hide();
                QMessageBox::warning(this, "Delete Contact", "The contact book is empty. Create your contact list.");
                return;
            }

            if (root != nullptr && trie.searching(root, input))
            {
                QStringList names, phones, emails, lastNames, companies;
                trie.searchPrefix(root, input, names, phones, emails, lastNames, companies);

                if (!phones.isEmpty())
                {
                    QString associatedPhone = phones.first();
                    root = trie.deletion(root, input, 0);
                    rootPhn = triePhn.deletion(rootPhn, associatedPhone, 0);
                    contactFound = true;
                }
            }

            if (contactFound)
            {
                hide();
                QMessageBox::information(this, "Delete Contact", "Contact deleted successfully.");
                QWidget* parentWidget = this->parentWidget();
                MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget);
                if (mainWindow)
                {
                    mainWindow->refreshContacts();
                    mainWindow->writeToCSV(root, "trie.csv", rootPhn, "triePhn.csv", rootSplay, "splay.csv");
                }
            }
            else
            {
                hide();
                QMessageBox::warning(this, "Delete Contact", "Contact not found.");
            }
        }
        break;
    }

    case 2:
    {
        QWidget* parentWidget = this->parentWidget();
        MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget);
        bool contactFoundTrie = false;
        bool contactFoundPhn = false;

        contactFoundTrie = trie.addFavourite(root, names.toLower());
        contactFoundPhn = triePhn.addFavourite(rootPhn, phnNo);

        if (contactFoundTrie && contactFoundPhn)
        {
            if (mainWindow)
            {
                qDebug() << "Contact added to favorites. Writing to CSV.";
                mainWindow->writeToCSV(root, "trie.csv", rootPhn, "triePhn.csv",rootSplay, "splay.csv");
            }
            else
            {
                qDebug() << "MainWindow pointer is null or invalid!";
            }
        }
        else
        {
            qDebug() << "Failed to add contact to favorites.";
        }

        if (mainWindow)
        {
            mainWindow->on_showFavoriteContacts_clicked();
        }
        break;
    }

    case 3:
    {
        hide();
        rootSplay = splayTree.splay(rootSplay, names);
        editing->getInput(names, phnNo);
        editing->exec();
        break;
    }

    default:
        break;
    }
}
