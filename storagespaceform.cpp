#include "storagespaceform.h"
#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"

StorageSpaceForm::StorageSpaceForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    readSettings();
    indexTemp = id;

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f.- 0-9\"]{100}");
    editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);

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
        query.prepare("SELECT storagespacename FROM storagespace WHERE storagespaceid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(0).toString());
        }
    }else{
        editName->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,1,1);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Storage Space"));

    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    QWidget::setTabOrder(editName,saveButton);
    QWidget::setTabOrder(saveButton,cancelButton);
}

void StorageSpaceForm::editRecord()
{
    if(!editName->text().isEmpty()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        QSqlQuery query;
        query.prepare("SELECT * FROM storagespace WHERE storagespacename = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            if(indexTemp != ""){
                QSqlQuery query;
                query.prepare("UPDATE storagespace SET storagespacename = :name"
                              " WHERE storagespaceid = :id");
                query.bindValue(":name",editName->text());
                query.bindValue(":id",indexTemp);
                query.exec();
                line += "UPDATE storagespace SET storagespacename = '";
                line += editName->text().toUtf8();
                line += "' WHERE storagespaceid = '";
                line += indexTemp;
                line += "'";
                line += "\r\n";
                stream<<line;
            }else{
                NumPrefix numPrefix(this);
                indexTemp = numPrefix.getPrefix("storagespace");
                if(indexTemp == ""){
                    close();
                }else{
                    QSqlQuery query;
                    query.prepare("INSERT INTO storagespace (storagespaceid, storagespacename) "
                                  "VALUES(:id, :name)");
                    query.bindValue(":id",indexTemp);
                    query.bindValue(":name",editName->text().simplified());
                    query.exec();
                    line += "INSERT INTO storagespace (storagespaceid, storagespacename) VALUES('";
                    line += indexTemp;
                    line += "', '";
                    line += editName->text().toUtf8();
                    line += "')";
                    line += "\r\n";
                    stream<<line;
                }
            }
            emit accept();
            close();
        }else{
            QString tempString = editName->text();
            tempString += QObject::tr(" is availble!");
            QMessageBox::warning(this,QObject::tr("Attention!!!"),tempString);
        }

    }else{
        QMessageBox::warning(this,QObject::tr("Attention!!!"),tr("Name don't be empty!"));
    }
}

void StorageSpaceForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"storagespace",this);
    forDelete.result();
    forDelete.deleteOnDefault();
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM storagespace WHERE storagespaceid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    line += "DELETE FROM storagespace WHERE storagespaceid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void StorageSpaceForm::done(int result)
{
    exchangeFile.close();
    writeSettings();
    QDialog::done(result);
}

void StorageSpaceForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    restoreGeometry(settings.value("StorageSpaceForm").toByteArray());
}

void StorageSpaceForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    settings.setValue("StorageSpaceForm", saveGeometry());
}
