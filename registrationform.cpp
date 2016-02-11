#include "registrationform.h"
#include "dbdirection.h"
#include "getbase.h"

RegistrationForm::RegistrationForm(QWidget *parent) : QDialog(parent)
{
    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);
}

bool RegistrationForm::checkInput()
{
    bool value;
    DbDirection manipulation;
    if(!manipulation.connectDataBase()){
        close();
        value = false;
    }else{
        value = true;
    }
    return value;
}

bool RegistrationForm::checkFile()
{
    QFile file("Food.dat");
    if(!file.exists()){
        int quest = QMessageBox::warning(this,tr("Attention!"),
                                         tr("No DataBase File!\n"
                                            "Upload from Server?"),
                                         QMessageBox::Yes,QMessageBox::No);
        if(quest == QMessageBox::Yes){
            GetBase getBase(this);
            getBase.getBaseHttp();
            return true;
        }else{
            return false;
        }
    }else{
        return true;
    }
}
