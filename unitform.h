#ifndef UNITFORM_H
#define UNITFORM_H

#include <QtWidgets>
#include "lineedit.h"

class UnitForm : public QDialog
{
    Q_OBJECT

public:
    UnitForm(QString, QWidget *, bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

private:
    QLabel *labelName;
    LineEdit *editName;
    QLabel *labelFullName;
    LineEdit *editFullName;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QFile exchangeFile;
};

#endif // UNITFORM_H
