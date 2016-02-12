#ifndef NOMENCLATUREFORM_H
#define NOMENCLATUREFORM_H

#include <QtWidgets>
#include "lineedit.h"

class NomenclatureForm : public QDialog
{
    Q_OBJECT

public:
    NomenclatureForm(QString, QWidget *, bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

    void addUnitRecord();
    void seeUnitRecord();
    void listUnitRecord();

private:
    QLabel *labelName;
    LineEdit *editName;
    QLabel *labelUnit;
    LineEdit *editUnit;
    QLabel *labelCost;
    LineEdit *editCost;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QFile exchangeFile;
};

#endif // NOMENCLATUREFORM_H
