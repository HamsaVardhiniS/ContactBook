#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "trie.h"
#include "triephn.h"
#include "newcontact.h"
#include "details.h"
#include "splay.h"

class details;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_newContact_clicked();
    void on_search_clicked();
    void on_showAllContacts_clicked();

public:
    void loadCSVData(const QString &trieFile, const QString &triePhnFile, const QString &splayFile, Node* root, NodePhn* rootPhn, SplayNode* rootSplay);
    void writeToCSV(Node* root, const QString& trieFile, NodePhn* rootPhn, const QString& triePhnFile, SplayNode* rootSplay, const QString& splayFile);
    void refreshContacts();
    void on_showFavoriteContacts_clicked();
    void on_contactSelected(const QModelIndex &index);
    void on_favoriteContactSelected(const QModelIndex &index);
    void on_recentlyAccessedSelected(const QModelIndex & index);
    void showRecentlyAccessedContacts();


    Ui::MainWindow *ui;
    details *details1;
    newcontact *newContact;
};

#endif // MAINWINDOW_H
