#ifndef DETAILS_H
#define DETAILS_H

#include <QDialog>
#include "edit.h"

namespace Ui {
class details;
}

class edit;

class details : public QDialog
{
    Q_OBJECT

public:
    explicit details(const QString &name, const QString &phone, const QString &email,
                     const QString &lastName, const QString &company, QWidget *parent = nullptr);
    ~details();

private slots:
    void on_moreOptions_currentIndexChanged(int index);

private:
    Ui::details *ui;
    edit *editing;
};

#endif // DETAILS_H
