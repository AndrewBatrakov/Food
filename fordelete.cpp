#include "fordelete.h"
#include <QtSql>
#include "numprefix.h"

ForDelete::ForDelete(QString id, QString tabNameAud, QWidget *parent) :
    QDialog(parent)
{
    tabNameAuditee = tabNameAud;
    bool closeForm = true;
    indexTemp = id;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QSqlQuery query;
    query.exec("select tbl_name from sqlite_master where type = 'table'");
    while(query.next()){
        if(tabNameAuditee != query.value(0).toString()){//имя таблицы
            QSqlQuery queryTable;
            QString table = query.value(0).toString();
            QString tableid = table;
            tableid += "id";
            QString tableName = table;
            tableName += "name";
            QString textQuery = "SELECT ";
            textQuery += tableid;
            textQuery += ", ";
            textQuery += tableName;
            textQuery += " FROM ";
            textQuery += table;
            textQuery += " WHERE ";
            textQuery += tabNameAuditee;
            textQuery += "id = '%1'";
            textQuery.arg(indexTemp);
            queryTable.exec(textQuery);
            labelLabel = new QLabel;
            labelLabel->setText(tableName);
            labelLabel->setStyleSheet("font: bold; color: darkblue;");
            mainLayout->addWidget(labelLabel);
            int row = 0;
            while(queryTable.next()){
                QLabel *label = new QLabel(this);
                QString toLabel;
                toLabel += queryTable.value(0).toString();
                toLabel += ",  ";
                toLabel += queryTable.value(1).toString();
                label->setText(toLabel);
                mainLayout->addWidget(label);
                ++row;
                closeForm = false;
            }
            if(row == 0){
                mainLayout->removeWidget(labelLabel);
                closeForm = true;
            }
        }
    }
    if(closeForm){
        //emit reject();
        close();
    }else{
        setLayout(mainLayout);
        setWindowTitle(tr("Conditions for removal..."));
    }
}

void ForDelete::deleteOnDefault()
{
    QSqlQuery query;
    query.exec("select tbl_name from sqlite_master where type = 'table'");
    while(query.next()){
        if(tabNameAuditee != query.value(0).toString()){//имя таблицы
            QSqlQuery queryTable;
            QString table = query.value(0).toString();
            QString tableid = table;
            tableid += "id";
            QString tableName = table;
            tableName += "name";
            QString textQuery = "SELECT ";
            textQuery += tableid;
            textQuery += ", ";
            textQuery += tableName;
            textQuery += " FROM ";
            textQuery += table;
            textQuery += " WHERE ";
            textQuery += tabNameAuditee;
            textQuery += "id = %1";
            textQuery.arg(indexTemp);
            queryTable.exec(textQuery);
            while(queryTable.next()){
                QSqlQuery queryCheck;
                QString textCheck = "SELECT ";
                textCheck += tabNameAuditee;
                textCheck += "id FROM ";
                textCheck += tabNameAuditee;
                textCheck += " WHERE ";
                textCheck += tabNameAuditee;
                textCheck += "name = 'No Value'";
                queryCheck.exec(textCheck);
                queryCheck.next();
                QString indexToUpdate;
                if(!queryCheck.isValid()){
                    NumPrefix numPrefix(this);
                    indexToUpdate = numPrefix.getPrefix(tabNameAuditee);
                    QSqlQuery queriInto;
                    QString textInto = "INSERT INTO ";
                    textInto += tabNameAuditee;
                    textInto += " (";
                    textInto += tabNameAuditee;
                    textInto += "id, ";
                    textInto += tabNameAuditee;
                    textInto += "name) VALUES(%1, %2)";
                    textInto.arg(indexToUpdate).arg("No Value");
                    queriInto.exec(textInto);
                }else{
                    QSqlQuery queryReplace;
                    //QString tableid = table;
                    //tableid += "id";
                    //QString tableName = table;
                    //tableName += "name";
                    QString textUpdate = "UPDATE ";
                    textUpdate += table;
                    textUpdate += " SET ";
                    textUpdate += tabNameAuditee;
                    textUpdate += "id = %1";
                    textUpdate += " WHERE ";
                    textUpdate += tableid;
                    textUpdate += "id = %2";
                    textUpdate.arg(indexToUpdate).arg(queryTable.value(0).toString());
                    queryReplace.exec(textUpdate);
                }
            }
        }
    }
}

