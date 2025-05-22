#ifndef TRIE_H
#define TRIE_H

#include <QString>
#include <QStringList>
#include <QChar>
#include <QDebug>

const int SIZE = 26; // Assuming only lowercase English letters

class Node {
public:
    Node* children[SIZE];
    bool isEndOfWord;
    QString phone;
    QString email;
    QString lastName;
    QString company;
    bool favourite;

    Node* createNode();
    void insertion(Node* root, QString data, QString phnNo, QString email, QString lastName, QString company, bool favourites);
    bool searching(Node* root, QString data);
    bool isEmpty(Node* root);
    Node* deletion(Node* root, QString data, int depth);
    void display(Node* root, int &count, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies, QStringList &favourites, QString prefix = "");
    void searchPrefix(Node* root, QString prefix, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies);
    void collectAllContacts(Node* node, QString prefix, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies);
    bool editContact(Node* root, const QString &name, const QString &newEmail, const QString &newLastName, const QString &newCompany);
    bool addFavourite(Node* root, const QString &name);
    void collectFavorites(Node* current, QString prefix, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies);
};

#endif // TRIE_H
