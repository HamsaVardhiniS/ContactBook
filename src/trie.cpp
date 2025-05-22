#include "trie.h"
#include <iostream>
#include <QString>
#include <QDebug>

using namespace std;

Node trie;
Node* root = trie.createNode();


Node* Node::createNode()
{
    Node* ptr = new Node();
    for (int i = 0; i < SIZE; i++)
    {
        ptr->children[i] = NULL;
    }
    ptr->isEndOfWord = false;
    ptr->phone = "";
    ptr->email = "";
    ptr->lastName = "";
    ptr->company = "";
    ptr->favourite=false;
    return ptr;
}

void Node::insertion(Node* root, QString data, QString phnNo, QString email, QString lastName, QString company, bool favourites)
{
    QString lowerData = data.toLower().trimmed();
    Node* temp = root;
    int index = 0;
    qDebug() << "Inserting data: " << lowerData;
    for (int i = 0; i < lowerData.length(); i++)
    {
        index = lowerData[i].unicode() - 'a';
        if (index < 0 || index >= SIZE)
        {
            return;
        }
        if (temp->children[index] == NULL)
        {
            temp->children[index] = createNode();
        }
        temp = temp->children[index];
    }

    temp->isEndOfWord = true;
    temp->phone = phnNo;
    temp->email = email;
    temp->lastName = lastName;
    temp->company = company;
    temp->favourite=favourites;
    qDebug() << "Insertion complete. Phone:" << phnNo << "Email:" << email << "Last Name:" << lastName << "Company:" << company;
}

bool Node::searching(Node* root, QString data)
{
    Node* temp = root;
    int index = 0;
    QString lowerData = data.toLower();
    for (int i = 0; i < lowerData.length(); i++)
    {
        index = lowerData[i].unicode() - 'a';
        if (index < 0 || index >= SIZE)
        {
            return false;
        }
        if (temp->children[index] == NULL)
        {
            return false;
        }
        temp = temp->children[index];
    }
    bool found = (temp != NULL && temp->isEndOfWord);
    return found;
}

bool Node::isEmpty(Node* root)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (root->children[i] != NULL)
        {
            return false;
        }
    }
    return true;
}

Node* Node::deletion(Node* root, QString data, int depth)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (depth == data.size())
    {
        if (root->isEndOfWord)
        {
            root->isEndOfWord = false;
        }

        if (isEmpty(root))
        {
            delete root;
            return NULL;
        }

        return root;
    }

    int index = data[depth].unicode() - 'a';
    if (index < 0 || index >= SIZE)
    {
        return root;
    }

    root->children[index] = deletion(root->children[index], data, depth + 1);

    if (isEmpty(root) && !root->isEndOfWord)
    {
        delete root;
        return NULL;
    }
    return root;
}

void Node::display(Node* root, int &count, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies, QStringList &favourites, QString prefix)
{
    if (root == NULL)
    {
        return;
    }
    for (int i = 0; i < SIZE; i++)
    {
        if (root->children[i] != NULL)
        {
            QChar nextChar = QChar('a' + i);
            display(root->children[i], count, names, phnNo, emails, lastNames, companies, favourites, prefix + nextChar); // Pass updated prefix
        }
    }
    if (root->isEndOfWord)
    {
        count++;
        names.append(prefix);
        phnNo.append(root->phone);
        emails.append(root->email);
        lastNames.append(root->lastName);
        companies.append(root->company);
        favourites.append(root->favourite ? "true" : "false");
        qDebug() << "Contact displayed:" << prefix << root->phone << root->email << root->lastName << root->company << "Favorite:" << (root->favourite ? "true" : "false");
    }
}

void Node::searchPrefix(Node* root, QString prefix, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies)
{
    if (root == NULL || prefix.isEmpty()) {
        qDebug() << "Root is NULL or prefix is empty.";
        return;
    }

    Node* temp = root;
    int index = 0;
    QString lowerPrefix = prefix.toLower().trimmed();
    qDebug() << "Starting search for prefix:" << lowerPrefix;

    for (int i = 0; i < lowerPrefix.length(); i++)
    {
        index = lowerPrefix[i].unicode() - 'a';
        if (index < 0 || index >= SIZE)
        {
            qDebug() << "Invalid index calculated for character:" << lowerPrefix[i];
            return;
        }

        if (temp->children[index] == NULL)
        {
            qDebug() << "No node found for character:" << lowerPrefix[i];
            return;
        }
        qDebug() << "Traversing node for character:" << lowerPrefix[i];
        temp = temp->children[index];
    }

    qDebug() << "Prefix found. Now collecting all contacts.";
    collectAllContacts(temp, lowerPrefix, names, phnNo, emails, lastNames, companies);
}

void Node::collectAllContacts(Node* node, QString prefix, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies)
{
    if (node == NULL) {
        qDebug() << "Node is NULL.";
        return;
    }

    if (node->isEndOfWord)
    {
        names.append(prefix);
        phnNo.append(node->phone);
        emails.append(node->email);
        lastNames.append(node->lastName);
        companies.append(node->company);
    }

    for (int i = 0; i < SIZE; i++)
    {
        if (node->children[i] != NULL)
        {
            QChar nextChar = QChar('a' + i);
            collectAllContacts(node->children[i], prefix + nextChar, names, phnNo, emails, lastNames, companies);
        }
    }
}

bool Node::editContact(Node* root, const QString &name, const QString &newEmail, const QString &newLastName, const QString &newCompany)
{
    QString lowerName = name.toLower().trimmed();
    Node* current = root;
    int index = 0;

    qDebug() << "Editing contact for name: " << lowerName;

    for (QChar ch : lowerName)
    {
        index = ch.unicode() - 'a';
        if (index < 0 || index >= SIZE)
        {
            qDebug() << "Invalid character encountered in name: " << ch;
            return false;
        }
        if (current->children[index] == nullptr)
        {
            qDebug() << "No contact found for this name. Character: " << ch;
            return false;
        }
        current = current->children[index];
    }

    if (current->isEndOfWord)
    {
        current->email = newEmail.isEmpty() ? current->email : newEmail;
        current->lastName = newLastName.isEmpty() ? current->lastName : newLastName;
        current->company = newCompany.isEmpty() ? current->company : newCompany;
        return true;
    }
    else
    {
        qDebug() << "No contact found with this name.";
        return false;
    }
}


bool Node::addFavourite(Node* root, const QString &name)
{
    QString lowerName = name.toLower().trimmed();
    Node* current = root;
    int index = 0;

    for (QChar ch : lowerName)
    {
        index = ch.unicode() - 'a';
        if (index < 0 || index >= SIZE)
        {
            return false;
        }
        current = current->children[index];
    }

    if (current->isEndOfWord)
    {
        current->favourite = true;
        return true;
    }
    else
    {
        return false;
    }
}

void Node::collectFavorites(Node* current, QString prefix, QStringList &names, QStringList &phnNo, QStringList &emails, QStringList &lastNames, QStringList &companies)
{
    if (current == NULL) {
        qDebug() << "Node is NULL";
        return;
    }

    if (current->isEndOfWord && current->favourite)
    {
        names.append(prefix);
        phnNo.append(current->phone);
        emails.append(current->email);
        lastNames.append(current->lastName);
        companies.append(current->company);
    }

    for (int i = 0; i < SIZE; i++)
    {
        if (current->children[i] != NULL)
        {
            QChar nextChar = QChar('a' + i);
            collectFavorites(current->children[i], prefix + nextChar, names, phnNo, emails, lastNames, companies);
        }
    }
}
