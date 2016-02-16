#ifndef INGREDIENTFORM_H
#define INGREDIENTFORM_H

#include <QtWidgets>
#include "lineedit.h"

class IngredientForm : public QDialog
{
    Q_OBJECT
public:
    IngredientForm(QString, QWidget *, bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

    void addRecordOfTable();
    void deleteRecordOfTable();
    void editRecordOfTable();

private:
    QLabel *labelName;
    LineEdit *editName;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QFile exchangeFile;

    QTableWidget *nomenclaturaWidget;
};

#endif // INGREDIENTFORM_H
