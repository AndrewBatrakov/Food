#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QtWidgets>

class RegistrationForm : public QDialog
{
    Q_OBJECT

public:
    RegistrationForm(QWidget *parent = 0);
    bool checkInput();
    bool checkFile();

};

#endif // REGISTRATIONFORM_H
