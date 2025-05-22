#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QString>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "trie.h"
#include "triephn.h"
#include "newcontact.h"
#include "details.h"
#include "splay.h"

extern Node trie;
extern Node* root;
extern NodePhn triePhn;
extern NodePhn* rootPhn;
extern SplayNode* rootSplay;
extern SplayNode splayTree;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), details1(nullptr), newContact(nullptr)
{
    ui->setupUi(this);
    root = trie.createNode();
    rootPhn = triePhn.createNode();
    loadCSVData("trie.csv", "triePhn.csv", "splay.csv" ,root, rootPhn, rootSplay);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete details1;
    delete newContact;
}

void MainWindow::refreshContacts()
{
    on_showAllContacts_clicked();
    on_showFavoriteContacts_clicked();
    showRecentlyAccessedContacts();
}

void MainWindow::on_newContact_clicked()
{
    if (!newContact) {
        newContact = new newcontact(this);
    }
    newContact->show();
}

void MainWindow::on_search_clicked()
{
    QString input = ui->searchName->text().trimmed();
    QStringList names, phnNo, emails, lastNames, companies;

    bool isPhoneNumber = true;

    for (int i = 0; i < input.length(); i++)
    {
        if (!input[i].isDigit())
        {
            isPhoneNumber = false;
            break;
        }
    }

    if (isPhoneNumber)
    {
        triePhn.searchPrefix(rootPhn, input, names, phnNo, emails, lastNames, companies);
    }
    else
    {
        trie.searchPrefix(root, input, names, phnNo, emails, lastNames, companies);
    }

    if (names.isEmpty())
    {
        QMessageBox::information(this, "Search", "No contacts found with this prefix.");
        refreshContacts();
    }
    else
    {
        QStandardItemModel* model = new QStandardItemModel(names.size(), 2, this);
        model->setHorizontalHeaderLabels(QStringList() << "Name" << "Phone No");
        if(isPhoneNumber)
        {
            for (int i = 0; i < names.size(); i++)
            {
                names[i][0] = names[i][0].toUpper();
                phnNo[i][0] =phnNo[i][0].toUpper();
                model->setItem(i, 1, new QStandardItem(names[i]));
                model->setItem(i, 0, new QStandardItem(phnNo[i]));
            }
        }
       else
        {
            for (int i = 0; i < names.size(); i++)
            {
                names[i][0] = names[i][0].toUpper();
                model->setItem(i, 0, new QStandardItem(names[i]));
                model->setItem(i, 1, new QStandardItem(phnNo[i]));
            }
        }
        ui->tableView->setModel(model);
        ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #a6afae; color: black; }");
        ui->tableView->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: #a6afae; color: black; }");
    }
}

void MainWindow::on_showAllContacts_clicked()
{
    int count = 0;
    QStringList names, phnNo, emails, lastNames, companies, favourites;
    trie.display(root, count, names, phnNo, emails, lastNames, favourites, companies);

    if (count == 0)
    {
        qDebug() << "No contacts found.";
        return;
    }

    QStandardItemModel* model = new QStandardItemModel(count, 2, this);
    model->setHorizontalHeaderLabels(QStringList() << "Name" << "Phone No");

    for (int i = 0; i < count; i++)
    {
        names[i][0] = names[i][0].toUpper();
        model->setItem(i, 0, new QStandardItem(names[i]));
        model->setItem(i, 1, new QStandardItem(phnNo[i]));
    }

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 500);
    ui->tableView->setColumnWidth(1, 450);

    connect(ui->tableView, &QTableView::clicked, this, &MainWindow::on_contactSelected);

}

void MainWindow::loadCSVData(const QString &trieFile, const QString &triePhnFile, const QString &splayFile, Node* root, NodePhn* rootPhn, SplayNode* rootSplay)
{
    QFile fileTrie(trieFile), fileTriePhn(triePhnFile), fileSplay(splayFile);

    qDebug() << "Starting to load CSV data...";

    // Load Trie Data
    if (fileTrie.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fileTrie);
        qDebug() << "Reading trie file...";

        while (!in.atEnd())
        {
            QStringList fields = in.readLine().split(",");
            qDebug() << "Fields read: " << fields;

            if (fields.size() >= 3)
            {
                QString name = fields[0].trimmed();
                QString phone = fields[1].trimmed();
                QString email = fields[2].trimmed();
                QString lastName = fields.size() > 3 ? fields[3].trimmed() : "";
                QString company = fields.size() > 4 ? fields[4].trimmed() : "";
                bool isFavorite = fields.size() > 5 && fields[5].toLower() == "true";

                qDebug() << "Inserting into trie: " << name << ", " << phone;
                trie.insertion(root, name, phone, email, lastName, company, isFavorite);
                qDebug() << "Trie insertion completed for: " << name;
            }
        }
        fileTrie.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Could not open trie.csv for reading.");
        qDebug() << "Failed to open trie.csv.";
    }

    // Load Phone Trie Data
    if (fileTriePhn.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fileTriePhn);
        qDebug() << "Reading triePhn file...";

        while (!in.atEnd())
        {
            QStringList fields = in.readLine().split(",");
            qDebug() << "Fields read (phone): " << fields;

            if (fields.size() >= 3)
            {
                QString phone = fields[1].trimmed(); // This should be the phone number
                QString name = fields[0].trimmed();  // This should be the name
                QString email = fields[2].trimmed();
                QString lastName = fields.size() > 3 ? fields[3].trimmed() : "";
                QString company = fields.size() > 4 ? fields[4].trimmed() : "";
                bool isFavorite = fields.size() > 5 && fields[5].toLower() == "true";

                qDebug() << "Inserting into triePhn: " << phone << ", " << name;
                triePhn.insertion(rootPhn, phone, name, email, lastName, company, isFavorite);
                qDebug() << "Phone trie insertion completed for: " << phone;
            }
        }
        fileTriePhn.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Could not open triePhn.csv for reading.");
        qDebug() << "Failed to open triePhn.csv.";
    }

    // Load Splay Data
    if (fileSplay.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fileSplay);
        qDebug() << "Reading splay file...";

        while (!in.atEnd())
        {
            QStringList fields = in.readLine().split(",");
            qDebug() << "Fields read (splay): " << fields;

            if (fields.size() >= 3)
            {
                QString name = fields[0].trimmed();
                QString phone = fields[1].trimmed();
                QString email = fields[2].trimmed();
                QString lastName = fields.size() > 3 ? fields[3].trimmed() : "";
                QString company = fields.size() > 4 ? fields[4].trimmed() : "";
                bool isFavorite = fields.size() > 5 && fields[5].toLower() == "true";

                qDebug() << "Inserting into splay: " << name << ", " << phone;
                rootSplay=splayTree.insertion(rootSplay, name, phone, email, lastName, company, isFavorite);
                qDebug() << "Splay insertion completed for: " << name;
            }
        }
        fileSplay.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Could not open splay.csv for reading.");
        qDebug() << "Failed to open splay.csv.";
    }

    qDebug() << "CSV data load complete. Refreshing contacts.";
    refreshContacts();
}


void MainWindow::writeToCSV(Node* root, const QString& trieFile, NodePhn* rootPhn, const QString& triePhnFile, SplayNode* rootSplay, const QString& splayFile)
{
    QFile fileTrie(trieFile), fileTriePhn(triePhnFile), fileSplay(splayFile);

    qDebug() << "Starting writeToCSV function";

    // Open the Trie file for writing
    if (!fileTrie.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening" << trieFile << "for writing:" << fileTrie.errorString();
        return;
    } else {
        qDebug() << "Trie file opened successfully: " << trieFile;
    }

    // Open the Phone Trie file for writing
    if (!fileTriePhn.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening" << triePhnFile << "for writing:" << fileTriePhn.errorString();
        return;
    } else {
        qDebug() << "Phone Trie file opened successfully: " << triePhnFile;
    }

    // Open the Splay file for writing
    if (!fileSplay.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening" << splayFile << "for writing:" << fileSplay.errorString();
        return;
    } else {
        qDebug() << "Splay file opened successfully: " << splayFile;
    }

    QTextStream outTrie(&fileTrie);
    QTextStream outTriePhn(&fileTriePhn);
    QTextStream outSplay(&fileSplay);

    QStringList names, phnNos, emails, lastNames, companies, favourites;
    int contactCount = 0;

    // Write Trie Data
    qDebug() << "Writing Trie data...";
    trie.display(root, contactCount, names, phnNos, emails, lastNames, companies, favourites, "");
    qDebug() << "Number of contacts in Trie: " << contactCount;

    for (int i = 0; i < contactCount; i++) {
        qDebug() << "Writing contact " << i << " to Trie file: "
                 << names[i] << ", " << phnNos[i] << ", " << emails[i]
                 << ", " << lastNames[i] << ", " << companies[i];

        outTrie << names[i] << "," << phnNos[i] << "," << emails[i] << ","
                << lastNames[i] << "," << companies[i] << ","
                << (favourites[i] == "true" ? "true" : "false") << "\n";
    }

    qDebug() << "Trie data writing complete.";
    contactCount = 0; // Reset for Phone Trie

    // Write Phone Trie Data
    qDebug() << "Writing Phone Trie data...";
    triePhn.display(rootPhn, contactCount, names, phnNos, emails, lastNames, companies, favourites, "");
    qDebug() << "Number of contacts in Phone Trie: " << contactCount;

    for (int i = 0; i < contactCount; i++) {
        qDebug() << "Writing contact " << i << " to Phone Trie file: "
                 << phnNos[i] << ", " << names[i] << ", " << emails[i]
                 << ", " << lastNames[i] << ", " << companies[i];

        outTriePhn << names[i] << "," << phnNos[i] << "," << emails[i] << ","
                   << lastNames[i] << "," << companies[i] << ","
                   << (favourites[i] == "true" ? "true" : "false") << "\n";
    }

    qDebug() << "Phone Trie data writing complete.";
    contactCount = 0; // Reset for Splay Tree

    // Write Splay Tree Data
    qDebug() << "Writing Splay Tree data...";

    if (rootSplay == nullptr) {
        qDebug() << "Splay Tree root is nullptr, skipping writing Splay data.";
        return;
    }

    // Add debug before calling display
    qDebug() << "Displaying Splay Tree...";
    splayTree.display(rootSplay, names, phnNos, emails, lastNames, companies, favourites);

    // Add debug after calling display to ensure it completes
    qDebug() << "Splay Tree display complete. Number of contacts: " << names.size();

    // Write Splay Tree data to file
    for (int i = 0; i < names.size(); i++) {
        qDebug() << "Splay contact " << i << " details: "
                 << "Name: " << names[i] << ", Phone: " << phnNos[i]
                 << ", Email: " << emails[i] << ", LastName: " << lastNames[i]
                 << ", Company: " << companies[i] << ", Favourite: " << favourites[i];

        outSplay << names[i] << "," << phnNos[i] << "," << emails[i] << ","
                 << lastNames[i] << "," << companies[i] << ","
                 << (favourites[i] == "true" ? "true" : "false") << "\n";
    }

    qDebug() << "Splay Tree data writing complete.";

    // Close the files
    fileTrie.close();
    fileTriePhn.close();
    fileSplay.close();

    qDebug() << "All files closed.";

    // Refresh contacts
    refreshContacts();
}



void MainWindow::on_contactSelected(const QModelIndex &index)
{
    int row = index.row();
    QAbstractItemModel *model = ui->tableView->model();
    QString selectedName = model->data(model->index(row, 0)).toString();

    QStringList names, phnNo, emails, lastNames, companies;
    trie.searchPrefix(root, selectedName, names, phnNo, emails, lastNames, companies);

    if (!names.isEmpty())
    {
        QString name = names[0];
        QString phone = phnNo.isEmpty() ? "" : phnNo[0];
        QString email = emails.isEmpty() ? "" : emails[0];
        QString lastName = lastNames.isEmpty() ? "" : lastNames[0];
        QString company = companies.isEmpty() ? "" : companies[0];

        details *details1 = new details(name, phone, email, lastName, company, this);
        details1->exec();
        delete details1;
        rootSplay = splayTree.splay(rootSplay, phone);
    }
    else
    {
        QMessageBox::information(this, "Error", "No contact details found for the selected name.");
    }
}


void MainWindow::on_showFavoriteContacts_clicked()
{
    int count = 0;
    QStringList names, phnNo, emails, lastNames, companies;
    trie.collectFavorites(root, "", names, phnNo, emails, lastNames, companies);

    count = names.size();

    qDebug() << "Favorite contacts found:" << count;

    if (count == 0) {
        qDebug() << "No favorite contacts found.";
        return;
    }

    QStandardItemModel* model = new QStandardItemModel(count, 1, this);
    model->setHorizontalHeaderLabels(QStringList() << "Name");

    for (int i = 0; i < count; i++)
    {
        names[i][0] = names[i][0].toUpper();
        model->setItem(i, 0, new QStandardItem(names[i]));
    }

    ui->favouriteContactsTable->setModel(model);
    ui->favouriteContactsTable->setColumnWidth(0, 300);
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #a6afae; color: black; }");
    ui->tableView->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: #a6afae; color: black; }");
    connect(ui->favouriteContactsTable, &QTableView::clicked, this, &MainWindow::on_favoriteContactSelected);
}


void MainWindow::on_favoriteContactSelected(const QModelIndex &index)
{
    int row = index.row();
    QAbstractItemModel *model = ui->favouriteContactsTable->model();

    QString selectedName = model->data(model->index(row, 0)).toString();

    QStringList names, phnNo, emails, lastNames, companies;
    trie.searchPrefix(root, selectedName, names, phnNo, emails, lastNames, companies);

    if (!names.isEmpty())
    {
        QString name = names[0];
        QString phone = phnNo[0];
        QString email = emails.isEmpty() ? "" : emails[0];
        QString lastName = lastNames.isEmpty() ? "" : lastNames[0];
        QString company = companies.isEmpty() ? "" : companies[0];
        details *details1 = new details(name, phone, email, lastName, company, this);
        details1->exec();
        delete details1;
        rootSplay = splayTree.splay(rootSplay, phone);
    }
    else
    {
        QMessageBox::information(this, "Error", "No contact details found for the selected name.");
    }
}

void MainWindow::on_recentlyAccessedSelected(const QModelIndex & index)
{
    int row = index.row();
    QAbstractItemModel *model = ui->recentlyAccessedContacts->model();

    QString selectedName = model->data(model->index(row, 0)).toString();


    QStringList names, phnNo, emails, lastNames, companies;
    trie.searchPrefix(root, selectedName, names, phnNo, emails, lastNames, companies);


    if (!names.isEmpty())
    {
        QString name = names[0];
        QString phone = phnNo[0];
        QString email = emails.isEmpty() ? "" : emails[0];
        QString lastName = lastNames.isEmpty() ? "" : lastNames[0];
        QString company = companies.isEmpty() ? "" : companies[0];
        details *details1 = new details(name, phone, email, lastName, company, this);
        details1->exec();
        delete details1;
        rootSplay = splayTree.splay(rootSplay, phone);
    }
    else
    {
        QMessageBox::information(this, "Error", "No contact details found for the selected name.");
    }
}


void MainWindow::showRecentlyAccessedContacts()
{
    int count = 0;
    QStringList names, phnNo, emails, lastNames, companies;

    splayTree.displayTopFive(rootSplay, count, names, phnNo, emails, lastNames, companies);

    if (count == 0)
    {
        qDebug() << "No recently accessed contacts found.";
        return;
    }

    QStandardItemModel* model = new QStandardItemModel(count, 1, this);
    model->setHorizontalHeaderLabels(QStringList() << "Name");

    for (int i = 0; i < count; i++)
    {
        names[i][0] = names[i][0].toUpper();
        model->setItem(i, 0, new QStandardItem(names[i]));
    }

    ui->recentlyAccessedContacts->setModel(model);
    ui->recentlyAccessedContacts->setColumnWidth(0, 300);
    connect(ui->recentlyAccessedContacts, &QTableView::clicked, this, &MainWindow::on_recentlyAccessedSelected);
}
