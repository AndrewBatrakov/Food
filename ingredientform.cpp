#include "ingredientform.h"
#include <QtSql>

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
    QHeaderView *headMS = nomenclaturaWidget->horizontalHeader();
    nomenclaturaWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    nomenclaturaWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //medicalServiceWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    nomenclaturaWidget->setAlternatingRowColors(true);
    nomenclaturaWidget->resizeColumnsToContents();

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT ingridientname FROM ingridient WHERE ingridientid = :id");
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
    mainLayout->addWidget(nomenclaturaWidget,1,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,2,1);
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

}

void IngredientForm::editRecord()
{

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
