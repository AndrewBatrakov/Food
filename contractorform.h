#ifndef CONTRACTORFORM_H
#define CONTRACTORFORM_H

#include <QtWidgets>
#include "lineedit.h"

class ContractorForm : public QDialog
{
    Q_OBJECT

public:
    ContractorForm(QString, QWidget *, bool);
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

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QFile exchangeFile;
};

#endif // CONTRACTORFORM_H
