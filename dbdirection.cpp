#include "dbdirection.h"

DbDirection::DbDirection(QWidget *parent) :
    QDialog(parent)
{
}

bool DbDirection::connectDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Food.dat");
    if(!db.open()){
        QMessageBox::warning(0,QObject::tr("QSQLite Database Error"),
                             db.lastError().text());
        return false;
    }else{
        return true;
    }

}
