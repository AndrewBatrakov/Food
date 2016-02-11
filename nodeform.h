#ifndef NODEFORM_H
#define NODEFORM_H

#include <QtWidgets>
#include "lineedit.h"

class NodeForm : public QDialog
{
    Q_OBJECT
public:
    NodeForm(QString, QWidget *, bool);
    void done(int result);
    QString rowOut() const {return indexTemp;};

public slots:
    void deleteRecord();

private slots:
    void editRecord();
    void readSettings();
    void writeSettings();

private:
    QLabel *labelKod;
    LineEdit *editKod;
    QLabel *labelName;
    LineEdit *editName;
    QLabel *labelHead;
    QCheckBox *editHead;
    

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QString indexTemp;
    QFile exchangeFile;
};

#endif // NODEFORM_H
