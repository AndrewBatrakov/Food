#include "numprefix.h"
#include "prefixform.h"

NumPrefix::NumPrefix(QWidget *parent) :
    QDialog(parent)
{
}

QString NumPrefix::getPrefix(QString tableName)
{
    QString returnString;

    QSettings settings("AO_Batrakov_Inc.", "Food");
    QString  prefName= settings.value("numprefix").toString();
    if(prefName == ""){
        QMessageBox::warning(this,tr("Attention!"),tr("Insert Prefix Number!!!"));
        returnString = "";
        close();
        PrefixForm prefixForm(this);
        prefixForm.exec();
    }else{
        //QString prefName = queryPref.value(0).toString();
        QSqlQuery query;
        QString stringQuery = "SELECT ";
        stringQuery += tableName;
        stringQuery += "id FROM ";
        stringQuery += tableName;
        stringQuery += " WHERE ";
        stringQuery += tableName;
        stringQuery += "id LIKE '%";
        stringQuery += prefName;
        stringQuery += "%' ";
        stringQuery += " ORDER BY ";
        stringQuery += tableName;
        stringQuery += "id";
        //stringQuery.arg(prefName);
        query.exec(stringQuery);
        QString numberTemp, numberLast;

        query.last();
        numberLast = query.value(0).toString();
        if(numberLast == ""){
            numberLast = "0";
        }
        numberTemp = numberLast.right(9);
        int numNum = numberTemp.toInt();
        ++numNum;
        QString nTemp = QString::number(numNum);
        int www;
        www = nTemp.count();
        QString tt;
        for(int i = www; i < 9; ++i){
            tt += "0";
        }
        tt += QString::number(numNum);
        tt = prefName + tt;
        returnString = tt;
    }
   return returnString;
}

