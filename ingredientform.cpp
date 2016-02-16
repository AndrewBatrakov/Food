#include "ingredientform.h"
#include <QtSql>
#include "fordelete.h"
#include "numprefix.h"

IngredientForm::IngredientForm(QString id, QWidget *parent, bool onlyForRead):
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

    QPushButton *addTableButton = new QPushButton(tr("Add"));
    QPixmap pixAdd(":/add.png");
    addTableButton->setIcon(pixAdd);
    connect(addTableButton,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    QPushButton *deleteTableButton = new QPushButton(tr("Delete"));
    QPixmap pixDelete(":/delete.png");
    deleteTableButton->setIcon(pixDelete);
    connect(deleteTableButton,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    QPushButton *editTableButton = new QPushButton(tr("Edit"));
    QPixmap pixEdit(":/edit.png");
    editTableButton->setIcon(pixEdit);
    connect(editTableButton,SIGNAL(clicked()),this,SLOT(editRecordOfTable()));

    QHBoxLayout *buttonTableBox = new QHBoxLayout;
    buttonTableBox->addWidget(addTableButton);
    buttonTableBox->addWidget(deleteTableButton);
    buttonTableBox->addWidget(editTableButton);
    buttonTableBox->addStretch();

    //*****************************************************
    //Filter for table
    //*****************************************************
    QString filterID = "treatmentdocid = '";
    filterID += indexTemp;
    filterID += "'";

    //*****************************************************
    //Medical Service
    //*****************************************************
    nomenclaturaWidget = new QTableWidget(0,4);
    nomenclaturaWidget->setHorizontalHeaderLabels(QStringList()<<tr("Name")<<tr("Coefficient")<<tr("Cost")<<tr("Number"));
    QHeaderView *headN = nomenclaturaWidget->horizontalHeader();
    nomenclaturaWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    nomenclaturaWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //medicalServiceWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    nomenclaturaWidget->setAlternatingRowColors(true);
    nomenclaturaWidget->resizeColumnsToContents();
    headN->setStretchLastSection(true);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT ingridientname FROM ingridient WHERE ingridientid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(0).toString());

            int row = 0;
            QSqlQuery queryTable;
            queryTable.prepare("SELECT ingridienttableid, nomenclatureid, coefficient FROM ingridienttable WHERE ingridientid = :id");
            queryTable.bindValue(":id",indexTemp);
            queryTable.exec();
            while(queryTable.next()){
                QSqlQuery queryPrice;
                queryPrice.prepare("SELECT nomenclatureprice FROM price WHERE nomenclatureid = :id");
                queryPrice.bindValue(":id",queryTable.value(1).toString());
                queryPrice.exec();
                while(queryPrice.next()){
                    nomenclaturaWidget->insertRow(row);

                    QTableWidgetItem *item = new QTableWidgetItem;
                    nomenclaturaWidget->setItem(row,0,item);
                    nomenclaturaWidget->item(row,0)->setText(queryTable.value(1).toString());
                    QTableWidgetItem *item1 = new QTableWidgetItem;
                    nomenclaturaWidget->setItem(row,1,item1);
                    nomenclaturaWidget->item(row,1)->setText(queryPrice.value(0).toString());

                    QSqlQuery queryFoodDebris;
                    queryFoodDebris.prepare("SELECT number FROM fooddebris WHERE (nomenclatureid = :id AND changedate = :date)");
                    queryFoodDebris.bindValue(":id",queryTable.value(1).toString());
                    queryFoodDebris.bindValue(":date",QDate::currentDate());
                    queryFoodDebris.exec();
                    while(queryFoodDebris.next()){
                        QTableWidgetItem *item3 = new QTableWidgetItem;
                        nomenclaturaWidget->setItem(row,2,item3);
                        nomenclaturaWidget->item(row,2)->setText(queryFoodDebris.value(0).toString());
                    }
                }
            }
        }
    }else{
        NumPrefix numPrefix(this);
        indexTemp = numPrefix.getPrefix("ingridient");
        if(indexTemp == ""){
            close();
        }
        editName->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addLayout(buttonTableBox,1,0,1,2);
    mainLayout->addWidget(nomenclaturaWidget,2,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,3,1);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Ingridient"));

    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    QWidget::setTabOrder(editName,saveButton);
    QWidget::setTabOrder(saveButton,cancelButton);
}

void IngredientForm::deleteRecord()
{
    QSqlQuery queryQ;
    queryQ.prepare("SELECT * FROM ingridient WHERE indridientid = :id");
    queryQ.bindValue(":id",indexTemp);
    queryQ.exec();
    queryQ.next();
    if(queryQ.isValid()){
        ForDelete forDelete(indexTemp,"ingridient",this);
        forDelete.result();
        forDelete.deleteOnDefault();
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        QSqlQuery query;
        query.prepare("DELETE FROM ingridient WHERE igridientid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        line += "DELETE FROM ingridient WHERE ingridientid = '";
        line += indexTemp;
        line += "'";
        line += "\r\n";
        stream<<line;
    }
}

void IngredientForm::editRecord()
{
    if(!editName->text().isEmpty()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }

        QSqlQuery query;
        query.prepare("SELECT * FROM ingridient WHERE ingridientid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        if(query.isValid()){
            QSqlQuery query;
            query.prepare("UPDATE ingridient SET ingridientname = :name"
                          " WHERE ingridientid = :id");
            query.bindValue(":name",editName->text());
            query.bindValue(":id",indexTemp);
            query.exec();

            line += "UPDATE ingridient SET ingridientname = '";
            line += editName->text().toUtf8();
            line += "' WHERE treatmentdocid = '";
            line += indexTemp;
            line += "'";
            line += "\r\n";
            stream<<line;
        }else{
            QSqlQuery query;
            query.prepare("INSERT INTO ingridient (ingridientid, ingridientname) "
                          "VALUES(:id, :name)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            line += "INSERT INTO ingridient(ingridientid, ingridientname) VALUES('";
            line += indexTemp;
            line += "', '";
            line += editName->text().toUtf8();
            line += "')";
            line += "\r\n";
            stream<<line;
        }
        emit accept();
        close();
    }else{
        editName->setStyleSheet("background-color: red");
        QMessageBox::warning(this,QObject::tr("Attention!!!"),tr("Name don't be empty!"));
        editName->setStyleSheet("background-color: white");
    }
}

void IngredientForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    restoreGeometry(settings.value("Ingredient").toByteArray());
}

void IngredientForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    settings.setValue("Ingredient", saveGeometry());
}

void IngredientForm::done(int result)
{
    exchangeFile.close();
    writeSettings();
    QDialog::done(result);
}

void IngredientForm::addRecordOfTable()
{

}

void IngredientForm::deleteRecordOfTable()
{

}

void IngredientForm::editRecordOfTable()
{

}
