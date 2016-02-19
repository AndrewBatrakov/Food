#include "widgetlisttable.h"
#include <QtSql>

WidgetListTable::WidgetListTable(QString idTable, QString nameTable, QWidget *parent) : QDialog(parent)
{
    tableName = nameTable;
    iDTemp = idTable;
    labelName = "";

    addButton = new QPushButton(tr("Add record"));
    QPixmap pixAdd(":/add.png");
    addButton->setIcon(pixAdd);
    connect(addButton,SIGNAL(clicked()),this,SLOT(addRecord()));

    deleteButton = new QPushButton(tr("Delete record"));
    QPixmap pixDel(":/delete.png");
    deleteButton->setIcon(pixDel);
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteRecord()));

    editButton = new QPushButton(tr("Edit record"));
    QPixmap pixEdit(":/edit.png");
    editButton->setIcon(pixEdit);
    connect(editButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    searchButton = new QPushButton(tr("Search (by Name)"));
    connect(searchButton,SIGNAL(clicked()),this,SLOT(searchProcedure()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addStretch();

    tableView = new QTableWidget;

    viewTemplateTable(tableName);
    if(iDTemp != ""){
//        for(int row = 0; row  < templateModel->rowCount(); ++row){
//            QSqlRecord record = templateModel->record(row);
//            QModelIndex index = templateModel->index(row,1);
//            if(record.value(0).toString() == iDTemp){
//                tableView->setCurrentIndex(index);
//                break;
//            }
//        }
    }else{
        //tableView->setCurrentIndex(templateModel->index(0,0));
    }

    //tableView->setHorizontalHeaderLabels(listString);//QStringList()<<tr("Name")<<tr("Coefficient")<<tr("Cost")<<tr("Number"));
    headTable = tableView->horizontalHeader();
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //medicalServiceWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setAlternatingRowColors(true);
    tableView->resizeColumnsToContents();
    headTable->setStretchLastSection(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(tableView);
    setLayout(mainLayout);

    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));


    setWindowTitle(tr("List of %1").arg(labelName));
    resize(tableView->size().width()+200,tableView->size().height());

    createContextMenu();
}

void WidgetListTable::done(int res)
{
//    if(res == QDialog::Accepted){
//        QModelIndex index = tableView->currentIndex();
//        if(index.isValid()){
//            QSqlRecord record = templateModel->record(index.row());
//            QString per = tableName;
//            per += "id";
//            iDTemp = record.value(per).toString();
//        }else{
//            iDTemp = "";
//        }
//    }
    QDialog::done(res);
}

void WidgetListTable::createContextMenu()
{
    addAction = new QAction(tr("Add Record"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecord()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(tr("Delete Record"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecord()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(tr("Edit Record"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecord()));

    tableView->addAction(addAction);
    tableView->addAction(deleteAction);
    tableView->addAction(editAction);
    tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void WidgetListTable::viewTemplateTable(QString)
{
    int columnTable;
    if(tableName == "nomenclature"){
        columnTable = 4;
        tableView->setColumnCount(columnTable);
        tableView->setHorizontalHeaderLabels(QStringList()<<tr("Code")<<tr("Name")<<tr("Unit")<<tr("Cost"));
        labelName = tr("Nomenclature");
        tableView->setColumnHidden(0,true);
//        if(setFilter){
//            templateModel->setFilter(QString("unitname LIKE '%%1%'").arg(filterTable));
//        }
    }
    QString queryText = "SELECT * FROM '";
    queryText += tableName;
    queryText += "'";
    QSqlQuery query;
    query.exec(queryText);
    int row = 0;
    QTableWidgetItem *item = 0;
    while(query.next()){
        tableView->insertRow(row);
        for(int column = 0; column <= columnTable; ++column){
            item = new QTableWidgetItem;
            tableView->setItem(row,column,item);
            if(column == 2){
                item->setText(tableSQL("unit",query.value(column).toString()));
            }else{
                item->setText(query.value(column).toString());
            }
        }
        ++row;
    }
}

QString WidgetListTable::tableSQL(QString tab, QString id)
{
    QString queryText = "SELECT ";
    queryText += tab;
    queryText += "name FROM ";
    queryText += tab;
    queryText += " WHERE ";
    queryText += tab;
    queryText += "id = '";
    queryText += id;
    queryText += "'";
    QSqlQuery query;
    query.exec(queryText);
    query.next();
    return query.value(0).toString();
}
