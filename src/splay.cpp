#include "splay.h"
#include <iostream>
#include <QString>
#include <QDebug>

using namespace std;

SplayNode splayTree;
SplayNode* rootSplay = nullptr;

SplayNode::SplayNode()
    : name(""), phnNo(""), email(""), lastName(""), company(""), favourite(false), left(NULL), right(NULL) {}

SplayNode::SplayNode(QString name, QString phnNo, QString email, QString lastName, QString company, bool favourite)
    : name(name), phnNo(phnNo), email(email), lastName(lastName), company(company), favourite(favourite), left(NULL), right(NULL) {}

SplayNode* SplayNode::rightRotate(SplayNode* x)
{
    SplayNode* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

SplayNode* SplayNode::leftRotate(SplayNode* x)
{
    SplayNode* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

SplayNode* SplayNode::splay(SplayNode* root, QString phnNo)
{
    if (root == NULL || root->phnNo == phnNo)
        return root;

    if (phnNo < root->phnNo)
    {
        if (root->left == NULL)
            return root;

        if (phnNo < root->left->phnNo)
        {
            root->left->left = splay(root->left->left, phnNo);
            root = rightRotate(root);
        }
        else if (phnNo > root->left->phnNo)
        {
            root->left->right = splay(root->left->right, phnNo);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }
        return (root->left == NULL) ? root : rightRotate(root);
    }
    else
    {
        if (root->right == NULL)
            return root;

        if (phnNo > root->right->phnNo)
        {
            root->right->right = splay(root->right->right, phnNo);
            root = leftRotate(root);
        }
        else if (phnNo < root->right->phnNo)
        {
            root->right->left = splay(root->right->left, phnNo);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        return (root->right == NULL) ? root : leftRotate(root);
    }
}

SplayNode* SplayNode::insertion(SplayNode* root, QString name, QString phnNo, QString email, QString lastName, QString company, bool favourite)
{
    if (root == NULL)
        return new SplayNode(name, phnNo, email, lastName, company, favourite);

    root = splay(root, phnNo);

    if (root->phnNo == phnNo)
        return root;

    SplayNode* newNode = new SplayNode(name, phnNo, email, lastName, company, favourite);

    if (phnNo < root->phnNo)
    {
        newNode->right = root;
        newNode->left = root->left;
        root->left = NULL;
    }
    else
    {
        newNode->left = root;
        newNode->right = root->right;
        root->right = NULL;
    }

    return newNode;
}

SplayNode* SplayNode::deleteNode(SplayNode* root, QString phnNo)
{
    if (root == NULL)
        return NULL;

    root = splay(root, phnNo);

    if (root->phnNo != phnNo)
        return root;

    SplayNode* temp;
    if (root->left == NULL)
    {
        temp = root;
        root = root->right;
    }
    else
    {
        temp = root;
        root = splay(root->left, phnNo);
        root->right = temp->right;
    }
    delete temp;
    return root;
}


void SplayNode::display(SplayNode* rootSplay, QStringList& names, QStringList& phnNos, QStringList& emails, QStringList& lastNames, QStringList& companies, QStringList& favourites)
{
    if (rootSplay == nullptr) {
        return;
    }

    display(rootSplay->left, names, phnNos, emails, lastNames, companies, favourites);

    qDebug() << "Processing Splay node: " ;
    names.append(rootSplay->name);
    phnNos.append(rootSplay->phnNo);
    emails.append(rootSplay->email);
    lastNames.append(rootSplay->lastName);
    companies.append(rootSplay->company);
    favourites.append(rootSplay->favourite ? "true" : "false");
    display(rootSplay->right, names, phnNos, emails, lastNames, companies, favourites);
}


void SplayNode::displayTopFive(SplayNode* root, int &count, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies)
{
    if (root == NULL || count >= 5)
        return;

    displayTopFive(root->right, count, names, phnNo, emails, lastNames, companies);

    if (count < 5)
    {
        names.append(root->name);
        phnNo.append(root->phnNo);
        emails.append(root->email);
        lastNames.append(root->lastName);
        companies.append(root->company);
        count++;
    }

    displayTopFive(root->left, count, names, phnNo, emails, lastNames, companies);
}

