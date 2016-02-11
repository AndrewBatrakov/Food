#include "nodeform.h"
#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"

NodeForm::NodeForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    readSettings();
    indexTemp = id;

    labelKod = new QLabel(tr("Code:"));
    editKod = new LineEdit;
    editKod->setReadOnly(onlyForRead);
    labelKod->setBuddy(editKod);

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    labelName->setBuddy(editName);

    labelHead = new QLabel(tr("Head"));
    editHead = new QCheckBox;

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
        query.prepare("SELECT nodename, head FROM node WHERE nodeid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        while(query.next()){
            editKod->setText(indexTemp);
            editName->setText(query.value(0).toString());
            editHead->setChecked(query.value(1).toBool());
        }
    }else{
        editKod->clear();
        editName->clear();
        editHead->setCheckState(Qt::Unchecked);
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelKod,0,0);
    mainLayout->addWidget(editKod,0,1);
    mainLayout->addWidget(labelName,1,0);
    mainLayout->addWidget(editName,1,1);
    mainLayout->addWidget(labelHead,2,0);
    mainLayout->addWidget(editHead,2,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,3,1);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Node"));

    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    QWidget::setTabOrder(editKod,editName);
    QWidget::setTabOrder(editName,editHead);
    QWidget::setTabOrder(editHead,saveButton);
    QWidget::setTabOrder(saveButton,cancelButton);
    //QWidget::setTabOrder(cancelButton,editName);
}

void NodeForm::editRecord()
{
    if(!editKod->text().isEmpty()){
        if(!editName->text().isEmpty()){
            QTextStream stream(&exchangeFile);
            QString line;
            while(!stream.atEnd()){
                stream.readLine();
            }
            if(indexTemp != ""){
                QSqlQuery query;
                query.prepare("UPDATE node SET nodename = :name, head = :head"
                              " WHERE nodeid = :id");
                query.bindValue(":name",editName->text());
                query.bindValue(":head",editHead->checkState());
                query.bindValue(":id",indexTemp);
                query.exec();
                line += "UPDATE node SET nodename = '";
                line += editName->text().toUtf8();
                line += "', head = '";
                line += QString::number(editHead->checkState());
                line += "' WHERE nodeid = '";
                line += indexTemp;
                line += "'";
                line += "\r\n";
                stream<<line;
            }else{
                indexTemp = editKod->text().simplified();
                QSqlQuery query;
                query.prepare("SELECT * FROM node WHERE nodename = :name");
                query.bindValue(":name",editName->text().simplified());
                query.exec();
                query.next();
                if(!query.isValid()){
                    QSqlQuery query;
                    query.prepare("INSERT INTO node (nodeid, nodename, head) "
                                  "VALUES(:id, :name, :head)");
                    query.bindValue(":id",indexTemp);
                    query.bindValue(":name",editName->text().simplified());
                    query.bindValue(":head",editHead->checkState());
                    query.exec();
                    line += "INSERT INTO node (nodeid, nodename, head) VALUES('";
                    line += indexTemp;
                    line += "', '";
                    line += editName->text().toUtf8();
                    line += "', '";
                    line += QString::number(editHead->checkState());
                    line += "')";
                    line += "\r\n";
                    stream<<line;
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
    }else{
        QMessageBox::warning(this,QObject::tr("Attention!!!"),tr("Code don't be empty!"));
    }
}

void NodeForm::deleteRecord()
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
    query.prepare("DELETE FROM node WHERE nodeid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    line += "DELETE FROM node WHERE nodeid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void NodeForm::done(int result)
{
    exchangeFile.close();
    writeSettings();
    QDialog::done(result);
}

void NodeForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    restoreGeometry(settings.value("NodeForm").toByteArray());
}

void NodeForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    settings.setValue("NodeForm", saveGeometry());
}
