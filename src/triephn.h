#ifndef TRIEPHN_H
#define TRIEPHN_H

#include <QString>
#include <QStringList>

const int SIZEOFNO = 10;
class NodePhn
{
private:
    NodePhn* children[SIZEOFNO];
    bool isEndOfWord;
    QString name;
    QString email;
    QString lastName;
    QString company;
    bool favourite;

    bool isEmpty(NodePhn* root);

public:
    NodePhn* createNode();
    void insertion(NodePhn* rootPhn, const QString &phone, const QString &name, const QString &email, const QString &lastName, const QString &company, bool favourites);
    NodePhn* deletion(NodePhn* root, QString phone, int depth = 0);
    bool searching(NodePhn* root, QString phone);
    void display(NodePhn* rootPhn, int &count, QStringList &phnPhones, QStringList &phnNames, QStringList &phnEmails, QStringList &lastNames, QStringList &companies, QStringList &favourites, QString prefix);
    void searchPrefix(NodePhn* root, QString prefix, QStringList &phones, QStringList &names, QStringList &emails, QStringList &lastNames, QStringList &companies);
    void collectAllContacts(NodePhn* node, QString prefix, QStringList &phones, QStringList &names, QStringList &emails, QStringList &lastNames, QStringList &companies);
    bool editContact(NodePhn* rootPhn, const QString &phone, const QString &newEmail, const QString &newLastName, const QString &newCompany);
    bool addFavourite(NodePhn* rootPhn, const QString &phone);
};

#endif // TRIEPHN_H
