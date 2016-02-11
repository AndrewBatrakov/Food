#include "unitform.h"
#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"

UnitForm::UnitForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    readSettings();
    indexTemp = id;

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f.\"]{5}");
    editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);
    labelFullName = new QLabel(tr("Full Name:"));
    editFullName = new LineEdit;
    editFullName->setReadOnly(onlyForRead);
    QRegExp regExpF("[\\x0410-\\x044f.\"]{100}");
    editFullName->setValidator(new QRegExpValidator(regExpF,this));
    labelFullName->setBuddy(editFullName);

    saveButton = new QPushButton(tr("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    saveButton->setToolTip(tr("Save And Close Button"));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelButton->setToolTip(tr("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT unitname, fullname FROM unit WHERE unitid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(0).toString());
            editFullName->setText(query.value(1).toString());
        }
    }else{
        editName->clear();
        editFullName->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addWidget(labelFullName,1,0);
    mainLayout->addWidget(editFullName,1,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,2,1);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Unit"));

    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    QWidget::setTabOrder(editName,editFullName);
    QWidget::setTabOrder(editFullName,saveButton);
    QWidget::setTabOrder(saveButton,cancelButton);
    //QWidget::setTabOrder(cancelButton,editName);
}

void UnitForm::editRecord()
{
    if(!editName->text().isEmpty()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        if(indexTemp != ""){
            QSqlQuery query;
            query.prepare("UPDATE unit SET unitname = :name, fullname = :fullname"
                          " WHERE unitid = :id");
            query.bindValue(":name",editName->text());
            query.bindValue(":fullname",editFullName->text());
            query.bindValue(":id",indexTemp);
            query.exec();
            line += "UPDATE unit SET unitnname = '";
            line += editName->text().toUtf8();
            line += "', fullname = '";
            line += editFullName->text().toUtf8();
            line += "' WHERE unitid = '";
            line += indexTemp;
            line += "'";
            line += "\r\n";
            stream<<line;
        }else{
            QSqlQuery query;
            query.prepare("SELECT * FROM unit WHERE unitname = :name");
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            query.next();
            if(!query.isValid()){
                NumPrefix numPrefix(this);
                indexTemp = numPrefix.getPrefix("unit");
                if(indexTemp == ""){
                    close();
                }else{
                    QSqlQuery query;
                    query.prepare("INSERT INTO unit (unitid, unitname, fullname) "
                                  "VALUES(:id, :name, :fullname)");
                    query.bindValue(":id",indexTemp);
                    query.bindValue(":name",editName->text().simplified());
                    query.bindValue(":fullname",editFullName->text().simplified());
                    query.exec();
                    line += "INSERT INTO unit (unitid, unitname, fullname) VALUES('";
                    line += indexTemp;
                    line += "', '";
                    line += editName->text().toUtf8();
                    line += "', '";
                    line += editFullName->text().toUtf8();
                    line += "')";
                    line += "\r\n";
                    stream<<line;
                }
            }else{
                QString tempString = editName->text();
                tempString += QObject::tr(" is availble!");
                QMessageBox::warning(this,QObject::tr("Attention!!!"),tempString);
            }
        }
        emit accept();
        close();
    }else{
        QMessageBox::warning(this,QObject::tr("Attention!!!"),tr("Name don't be empty!"));
    }
}

void UnitForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"unit",this);
    forDelete.result();
    forDelete.deleteOnDefault();
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM unit WHERE unitid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    line += "DELETE FROM unit WHERE unitid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void UnitForm::done(int result)
{
    exchangeFile.close();
    writeSettings();
    QDialog::done(result);
}

void UnitForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    restoreGeometry(settings.value("UnitForm").toByteArray());
}

void UnitForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    settings.setValue("UnitForm", saveGeometry());
}
