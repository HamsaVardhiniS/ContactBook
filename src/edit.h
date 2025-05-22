#ifndef EDIT_H
#define EDIT_H

#include <QDialog>
#include "mainwindow.h"

class MainWindow;

namespace Ui {
class edit;
}

class edit : public QDialog
{
    Q_OBJECT

public:
    explicit edit(QWidget *parent = nullptr);
    ~edit();
    void getInput(QString name, QString phone);

private slots:
    void on_cancel_clicked();
    void on_save_clicked();

private:
    Ui::edit *ui;
    MainWindow* mainWindow;  // Pointer to MainWindow
};

#endif // EDIT_H
