#ifndef PREFIXFORM_H
#define PREFIXFORM_H

#include <QtWidgets>
#include "lineedit.h"

class PrefixForm : public QDialog
{
    Q_OBJECT

public:
    PrefixForm(QWidget *parent);

private slots:
    void editPN();

private:
    QLabel *labelPrefix;
    LineEdit *editPrefix;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;
};

#endif // PREFIXFORM_H
