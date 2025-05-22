#include "triephn.h"
#include <QDebug>

#include <iostream>
#include <QString>
#include <QDebug>

using namespace std;

NodePhn triePhn;
NodePhn* rootPhn = triePhn.createNode();

NodePhn* NodePhn::createNode()
{
    NodePhn* newNode = new NodePhn();
    newNode->isEndOfWord = false;
    newNode->name = "";
    newNode->email = "";
    newNode->lastName = "";
    newNode->company = "";
    newNode->favourite = false;

    for (int i = 0; i < SIZEOFNO; i++)
    {
        newNode->children[i] = nullptr;
    }
    return newNode;
}

void NodePhn::insertion(NodePhn* rootPhn, const QString &phone, const QString &name, const QString &email, const QString &lastName, const QString &company, bool favourites)
{
    if (rootPhn == nullptr) {
        qDebug() << "Error: rootPhn is null!";
        return;
    }

    NodePhn* current = rootPhn;

    for (QChar digit : phone)
    {
        int index = digit.digitValue();

        if (index < 0 || index > 9)
        {
            qDebug() << "Error: Invalid character in phone number: " << digit;
            return;
        }

        if (current->children[index] == nullptr)
        {
            qDebug() << "Creating new node for digit: " << digit;
            current->children[index] = createNode();
        }

        current = current->children[index];
    }

    current->isEndOfWord = true;
    current->name = name;
    current->email = email;
    current->lastName = lastName;
    current->company = company;
    current->favourite = favourites;

    qDebug() << "Insertion complete for phone: " << phone;
}

bool NodePhn::searching(NodePhn* root, QString phone)
{
    NodePhn* temp = root;

    for (int i = 0; i < phone.length(); i++)
    {
        int index = phone[i].unicode() - '0';
        if (index < 0 || index >= SIZEOFNO || temp->children[index] == nullptr)
        {
            return false;
        }
        temp = temp->children[index];
    }

    bool found = temp != nullptr && temp->isEndOfWord;
    return found;
}

bool NodePhn::isEmpty(NodePhn* root)
{
    for (int i = 0; i < SIZEOFNO; i++)
    {
        if (root->children[i] != nullptr)
        {
            return false;
        }
    }
    return true;
}

NodePhn* NodePhn::deletion(NodePhn* root, QString phone, int depth)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    if (depth == phone.size())
    {
        if (root->isEndOfWord)
        {
            root->isEndOfWord = false;
        }

        if (isEmpty(root))
        {
            delete root;
            return nullptr;
        }
        return root;
    }

    int index = phone[depth].unicode() - '0';
    root->children[index] = deletion(root->children[index], phone, depth + 1);

    if (isEmpty(root) && !root->isEndOfWord)
    {
        delete root;
        return nullptr;
    }
    return root;
}

void NodePhn::display(NodePhn* rootPhn, int &count, QStringList &phnPhones, QStringList &phnNames, QStringList &phnEmails, QStringList &lastNames, QStringList &companies, QStringList &favourites, QString prefix)
{
    if (rootPhn == nullptr)
    {
        return;
    }

    for (int i = 0; i < SIZEOFNO; i++)
    {
        if (rootPhn->children[i] != nullptr)
        {
            QString nextChar = QString::number(i);
            display(rootPhn->children[i], count, phnPhones, phnNames, phnEmails, lastNames, companies, favourites, prefix + nextChar);
        }
    }

    if (rootPhn->isEndOfWord)
    {
        count++;
        phnPhones.append(prefix);
        phnNames.append(rootPhn->name);
        phnEmails.append(rootPhn->email);
        lastNames.append(rootPhn->lastName);
        companies.append(rootPhn->company);
        favourites.append(rootPhn->favourite ? "true" : "false");
    }
}

void NodePhn::searchPrefix(NodePhn* root, QString prefix, QStringList &phones, QStringList &names, QStringList &emails, QStringList &lastNames, QStringList &companies)
{
    if (root == nullptr || prefix.isEmpty())
    {
        return;
    }

    NodePhn* temp = root;
    for (int i = 0; i < prefix.length(); i++)
    {
        int index = prefix[i].unicode() - '0';

        if (index < 0 || index >= SIZEOFNO || temp->children[index] == nullptr)
        {
            return;
        }
        temp = temp->children[index];
    }

    collectAllContacts(temp, prefix, phones, names, emails, lastNames, companies);
}

void NodePhn::collectAllContacts(NodePhn* node, QString prefix, QStringList &phones, QStringList &names, QStringList &emails, QStringList &lastNames, QStringList &companies)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->isEndOfWord)
    {
        phones.append(prefix);
        names.append(node->name);
        emails.append(node->email);
        lastNames.append(node->lastName);
        companies.append(node->company);
    }

    for (int i = 0; i < SIZEOFNO; i++)
    {
        if (node->children[i] != nullptr)
        {
            QChar nextChar = QChar('0' + i);
            collectAllContacts(node->children[i], prefix + nextChar, phones, names, emails, lastNames, companies);
        }
    }
}

bool NodePhn::editContact(NodePhn* rootPhn, const QString &phone, const QString &newEmail, const QString &newLastName, const QString &newCompany)
{
    NodePhn* current = rootPhn;
    for (QChar digit : phone)
    {
        int index = digit.digitValue();
        if (current->children[index] == nullptr)
        {
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
        return false;
    }
}

bool NodePhn::addFavourite(NodePhn* rootPhn, const QString &phone)
{
    NodePhn* current = rootPhn;

    for (QChar digit : phone)
    {
        int index = digit.digitValue();
        if (index < 0 || index >= SIZEOFNO)
        {
            return false;
        }

        if (current->children[index] == nullptr)
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
