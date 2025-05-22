#ifndef SPLAY_H
#define SPLAY_H

#include <QString>
#include <iostream>
using namespace std;



class SplayNode
{
private:
    QString name;
    QString phnNo;
    QString email;
    QString lastName;
    QString company;
    bool favourite;

    SplayNode *left;
    SplayNode *right;

    SplayNode* rightRotate(SplayNode* x);
    SplayNode* leftRotate(SplayNode* x);

public:
    SplayNode();
    SplayNode(QString name, QString phnNo, QString email = "", QString lastName = "", QString company = "", bool favourite = false);
    SplayNode* splay(SplayNode* root, QString phnNo);
    SplayNode* insertion(SplayNode* root, QString name, QString phnNo, QString email = "", QString lastName = "", QString company = "", bool favourite = false);
    SplayNode* deleteNode(SplayNode* root, QString phnNo);
    void display(SplayNode* node, QStringList& names, QStringList& phnNos,
                 QStringList& emails, QStringList& lastNames,
                 QStringList& companies, QStringList& favourites);
    void displayTopFive(SplayNode* root, int &count, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies);
};

extern SplayNode splayTree;
extern SplayNode* rootSplay;

#endif // SPLAY_H
