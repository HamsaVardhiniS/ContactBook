#ifndef NEWCONTACT_H
#define NEWCONTACT_H

#include <QDialog>

namespace Ui {
class newcontact;
}

class newcontact : public QDialog
{
    Q_OBJECT

public:
    explicit newcontact(QWidget *parent = nullptr);
    ~newcontact();

private slots:
    void on_Save_clicked();
    void on_Cancel_clicked();

private:
    Ui::newcontact *ui;
};

#endif // NEWCONTACT_H
