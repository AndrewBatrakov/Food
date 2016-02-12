#include "mainwindow.h"

//Service
#include "prefixform.h"
#include "boolitemdelegate.h"
#include "horizontalitemdelegate.h"

//Reference
#include "nodeform.h"
#include "unitform.h"
#include "storagespaceform.h"
#include "contractorform.h"
#include "organizationform.h"
#include "nomenclatureform.h"
#include "ingredientform.h"

//Documents

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QString programmName = tr("Food v.");
    QSettings iniSettings("Food.ini",QSettings::IniFormat);
    QString version = iniSettings.value("Version").toString();
    programmName += version;
    setWindowTitle(programmName);

    QPixmap pixLogo(":/food.png");
    setWindowIcon(pixLogo);

    QFile file(":/QWidgetStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleSheetString = QLatin1String(file.readAll());
    QWidget::setStyleSheet(styleSheetString);

    readSettings();
    createPanel();
    setFilter =false;
    createActions();
    createMenu();
    createContextMenu();
    fileExchange.setFileName("exchange.txt");
    fileExchange.open(QIODevice::ReadWrite);
    viewTemplateTable("node");
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QApplication::alert(this);
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Food"));
    msgBox.setText(tr("Close programm?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton(QMessageBox::Yes);
    QPushButton *noButton = msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(noButton);

    noButton->setStyleSheet("QPushButton:hover {color: red}");
    int r = msgBox.exec();
    if(r == QMessageBox::Yes){
        writeSettings();
        //QSqlDatabase::removeDatabase("FA_SQLite.dat");
        if(fileExchange.size() == 0){
            fileExchange.remove();
        }else{
            QSettings settings("AO_Batrakov_Inc.", "Food");
            if(settings.value("Send").toBool() == false){
                fileExchange.close();
            }else{
                settings.remove("Send");
                fileExchange.close();
                QFile fileDel("exchange.txt");
                fileDel.remove();
            }
        }
        event->accept();
        qApp->quit();
    }else if(r == QMessageBox::No){
        event->ignore();
    }
}

void MainWindow::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    settings.setValue("Geometry", saveGeometry());
    settings.remove("CurrentUser");
}

void MainWindow::createPanel()
{
    panel = new QWidget;
    tableLabel = new QLabel;
    tableView = new QTableView(this);
    templateModel = new QSqlRelationalTableModel(this);
    templateModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);

    addRecordButton = new QPushButton(tr("Add"));
    QPixmap pixAdd(":/add.png");
    addRecordButton->setIcon(pixAdd);
    connect(addRecordButton,SIGNAL(clicked()),this,SLOT(addRecordOfTable()));

    deleteRecordButton = new QPushButton(tr("Delete"));
    QPixmap pixDelete(":/delete.png");
    deleteRecordButton->setIcon(pixDelete);
    connect(deleteRecordButton,SIGNAL(clicked()),this,SLOT(deleteRecordOfTable()));

    editRecordButton = new QPushButton(tr("Edit"));
    QPixmap pixEdit(":/edit.png");
    editRecordButton->setIcon(pixEdit);
    connect(editRecordButton,SIGNAL(clicked()),this,SLOT(editRecordOfTable()));

    QHBoxLayout *buttonBox = new QHBoxLayout;

    buttonBox->addWidget(addRecordButton);
    buttonBox->addWidget(deleteRecordButton);
    buttonBox->addWidget(editRecordButton);
    buttonBox->addStretch();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonBox);
    mainLayout->addWidget(tableLabel);
    mainLayout->addWidget(tableView);
    panel->setLayout(mainLayout);
    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editRecordOfTable()));

    splitterCreate();
}

void MainWindow::createActions()
{
    //File Action
    prefixAction = new QAction(tr("Prefix..."),this);
    connect(prefixAction,SIGNAL(triggered()),this,SLOT(prefixForm()));
    exitAction = new QAction(tr("Exit..."),this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    //References Action
    unitAction = new QAction(tr("Unit..."),this);
    connect(unitAction,SIGNAL(triggered()),this,SLOT(viewUnit()));
    nodeAction = new QAction(tr("Nodes..."),this);
    connect(nodeAction,SIGNAL(triggered()),this,SLOT(viewNode()));
    storageSpaceAction = new QAction(tr("Storage Space..."),this);
    connect(storageSpaceAction,SIGNAL(triggered()),this,SLOT(viewStorageSpace()));
    contractorAction = new QAction(tr("Contractor..."),this);
    connect(contractorAction,SIGNAL(triggered()),this,SLOT(viewContractor()));
    organizationAction = new QAction(tr("Organization..."),this);
    connect(organizationAction,SIGNAL(triggered()),this,SLOT(viewOrganization()));
    nomenclatureAction = new QAction(tr("Nomenclature..."),this);
    connect(nomenclatureAction,SIGNAL(triggered()),this,SLOT(viewNomenclature()));
    ingridientAction = new QAction(tr("Ingridient..."),this);
    connect(ingridientAction,SIGNAL(triggered()),this,SLOT(viewIngridient()));
//    diseaseAction = new QAction(tr("Disease..."),this);
//    connect(diseaseAction,SIGNAL(triggered()),this,SLOT(viewDisease()));
//    locationActon = new QAction(tr("Location..."),this);
//    connect(locationActon,SIGNAL(triggered()),this,SLOT(viewLocation()));
//    typeOfWorkAction = new QAction(tr("Type Of Work..."),this);
//    connect(typeOfWorkAction,SIGNAL(triggered()),this,SLOT(viewTypeOfWork()));
//    preparationAction = new QAction(tr("Preparation..."),this);
//    connect(preparationAction,SIGNAL(triggered()),this,SLOT(viewPreparation()));

    //Documents Action
    //preparationDocAction = new QAction(tr("Preparation Document..."),this);
    //connect(preparationDocAction,SIGNAL(triggered()),this,SLOT(viewPreparationDoc()));

    //Reports Action

    //Service Action
    //prefixAction = new QAction(tr("Prefix..."),this);
    //connect(prefixAction,SIGNAL(triggered()),this,SLOT(prefixFunction()));

    //putAction = new QAction(tr("Put Base on FTP"),this);
    //connect(putAction,SIGNAL(triggered()),this,SLOT(putBaseProcedure()));

    //getAction = new QAction(tr("Get Base from FTP"),this);
    //connect(getAction,SIGNAL(triggered()),this,SLOT(getBaseProcedure()));

    //searchAction = new QAction(tr("Search In Journal By Contens..."),this);
    //connect(searchAction,SIGNAL(triggered()),this,SLOT(searchProcedure()));

    //exchangeAction = new QAction(tr("Exchange Data Base..."),this);
    //connect(exchangeAction,SIGNAL(triggered()),this,SLOT(exchangeData()));

    //Help Action
    aboutQtAction = new QAction(tr("About Qt..."),this);
    connect(aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    aboutProgAction = new QAction(tr("About programm..."),this);
    connect(aboutProgAction,SIGNAL(triggered()),this,SLOT(aboutProgramm()));
}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    menuBar()->setStyleSheet("font: bold; color: darkblue;");
    fileMenu->addAction(prefixAction);
    fileMenu->addAction(exitAction);

    referenceMenu = menuBar()->addMenu(tr("References"));
    referenceMenu->addAction(unitAction);
    referenceMenu->addAction(storageSpaceAction);
    referenceMenu->addAction(contractorAction);
    referenceMenu->addAction(organizationAction);
    referenceMenu->addAction(nomenclatureAction);
    referenceMenu->addAction(ingridientAction);
//    referenceMenu->addSeparator();
//    referenceMenu->addAction(preparationAction);
//    referenceMenu->addAction(medicalService);
//    //referenceMenu->addAction(treatmentAction);
//    referenceMenu->addAction(diseaseAction);
//    referenceMenu->addAction(typeOfWorkAction);
//    referenceMenu->addAction(locationActon);
    referenceMenu->addSeparator();
    referenceMenu->addAction(nodeAction);
    referenceMenu->addSeparator();

    documentMenu = menuBar()->addMenu(tr("Documents"));
//    documentMenu->addAction(preparationDocAction);
//    documentMenu->addSeparator();

    reportMenu = menuBar()->addMenu(tr("Reports"));
//    reportMenu->addSeparator();

    serviceMenu = menuBar()->addMenu(tr("Service"));
//    //serviceMenu->addAction(prefixAction);
//    serviceMenu->addAction(putAction);
//    serviceMenu->addAction(getAction);
//    serviceMenu->addSeparator();
//    serviceMenu->addAction(searchAction);
//    serviceMenu->addSeparator();
//    serviceMenu->addAction(exchangeAction);

    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutQtAction);
    helpMenu->addAction(aboutProgAction);
}

void MainWindow::aboutProgramm()
{
    QSettings iniSettings("Food.ini",QSettings::IniFormat);
    QString version = iniSettings.value("Version").toString();
    //    int fontId = iniSettings.value("Font").toInt();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Food"));
    QPixmap pixLogo(":/food.png");
    msgBox.setIconPixmap(pixLogo);
    QString textMessage = tr("<p style=\"color:darkblue\"; style=\"font-family:Times New Roman\"; "
                             "style=\"font-size :20pt\"; "
                             "style=\"font: bold\" >"
                             "Food "
                             "<br/> (SQLite version) ");
    textMessage += version;
    textMessage += tr("</p>"
                      "<p style=\"color:darkgreen\"; style=\"font-family:Times New Roman\"; "
                      "style=\"font-size :12pt\">Copyright 2011-16 A+O Batrakov Inc.</p>"
                      "<p style=\"font-size :12pt\">"
                      "The program helps to keep Food information.</p>"
                      "<p style=\"font-family:Times New Roman\"; style=\"font-size :10pt\">"
                      "An how to improve, extend the functionality of the program, press button "
                      "\"Send mail\"");
    msgBox.setText(textMessage);
    //msgBox.setFont(QFont(QFontDatabase::applicationFontFamilies(fontId).first()));
    QPushButton *sendM = new QPushButton(tr("Send mail\nto developer"));
    sendM->setStyleSheet("QPushButton {font: yellow}");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.addButton(sendM,QMessageBox::AcceptRole);
    msgBox.exec();
    if(msgBox.clickedButton() == sendM){
        //sendMail();
    }else{
        msgBox.close();
    }
}

void MainWindow::splitterCreate()
{
    splitter = new QSplitter(Qt::Horizontal);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(panel);
    splitter->setStretchFactor(1,1);
    setCentralWidget(splitter);
}

void MainWindow::createContextMenu()
{
    addAction = new QAction(tr("Add Record"),this);
    QPixmap pixAdd(":/add.png");
    addAction->setIcon(pixAdd);
    connect(addAction,SIGNAL(triggered()),this,SLOT(addRecordOfTable()));

    QPixmap pixDelete(":/delete.png");
    deleteAction = new QAction(tr("Delete Record"),this);
    deleteAction->setIcon(pixDelete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRecordOfTable()));

    QPixmap pixEdit(":/edit.png");
    editAction = new QAction(tr("Edit Record"),this);
    editAction->setIcon(pixEdit);
    connect(editAction,SIGNAL(triggered()),this,SLOT(editRecordOfTable()));

    //QPixmap pixEdit(":/edit.png");
    //copyAction = new QAction(tr("Copy Record"),this);
    //editAction->setIcon(pixEdit);
    //connect(copyAction,SIGNAL(triggered()),this,SLOT(copyRecordOfTable()));

    tableView->addAction(addAction);
    tableView->addAction(deleteAction);
    tableView->addAction(editAction);
    //tableView->addAction(copyAction);
    tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::prefixForm()
{
    PrefixForm openForm(this);
    openForm.exec();
}

void MainWindow::viewTemplateTable(QString tempTable)
{
    if(tableView->model())
        for(int i=0; i<tableView->model()->columnCount();i++){
            QAbstractItemDelegate* delegate = tableView->itemDelegateForColumn( i );
            //Remove the delegate from the view
            tableView->setItemDelegateForColumn( i, NULL );
            delete delegate;
        }
    tableView->clearSpans();
    templateModel->clear();
    templateModel->setTable(tempTable);

    QString strivgValue;
    bool delAll = false;
    if(tempTable == "node"){
        templateModel->setHeaderData(0,Qt::Horizontal,tr("Code"));
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        templateModel->setHeaderData(2,Qt::Horizontal,tr("Head"));
        if(setFilter){
            templateModel->setFilter(QString("nodename LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Node");
    }else if(tempTable == "treatmentdoc"){
        templateModel->setHeaderData(0,Qt::Horizontal,tr("Number"));
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Date"));
        templateModel->setHeaderData(2,Qt::Horizontal,tr("FIO"));
        templateModel->setRelation(2,QSqlRelation("employee","employeeid","employeename"));
        if(setFilter){
            templateModel->setFilter(QString("employeename LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Treatment Doc");
    }else if(tempTable == "unit"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        templateModel->setHeaderData(2,Qt::Horizontal,tr("Fuul Name"));
        if(setFilter){
            templateModel->setFilter(QString("unitname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Unit");
    }else if(tempTable == "storagespace"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        if(setFilter){
            templateModel->setFilter(QString("storagespacename LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Storage Space");
    }else if(tempTable == "contractor"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        if(setFilter){
            templateModel->setFilter(QString("contractorname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Contractor");
    }else if(tempTable == "organization"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        if(setFilter){
            templateModel->setFilter(QString("organizationname LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Organization");
    }else if(tempTable == "nomenclature"){
        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
        templateModel->setHeaderData(2,Qt::Horizontal,tr("Unit"));
        templateModel->setRelation(2,QSqlRelation("unit","unitid","unitname"));
        templateModel->setHeaderData(3,Qt::Horizontal,tr("Cost"));
        if(setFilter){
            templateModel->setFilter(QString("nomenclaturename LIKE '%%1%'").arg(filterTable));
        }
        strivgValue = tr("Nomenclature");
    }else if(tempTable == "ingridient"){
//        templateModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
//        templateModel->setHeaderData(2,Qt::Horizontal,tr("Unit"));
//        templateModel->setRelation(2,QSqlRelation("unit","unitid","unitname"));
//        templateModel->setHeaderData(3,Qt::Horizontal,tr("Cost"));
//        if(setFilter){
//            templateModel->setFilter(QString("nomenclaturename LIKE '%%1%'").arg(filterTable));
//        }
        strivgValue = tr("Ingridient");
    }
    if(!delAll){
        templateModel->select();
        QHeaderView *head = tableView->horizontalHeader();
        connect(head,SIGNAL(sectionClicked(int)),this,SLOT(sortTable(int)));
        tableView->setModel(templateModel);
        if(tempTable == "treatmentdoc" || tempTable == "node" || tempTable == "contractdoc"){
            tableView->setColumnHidden(0, false);
        }else{
            tableView->setColumnHidden(0, true);
        }
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        if(tempTable == "node"){
            tableView->setItemDelegateForColumn(2,new BoolItemDelegate(this));
            //templateModel->record().
        }
        if(tempTable == "nomenclature"){
            tableView->setItemDelegateForColumn(3,new HorizontalItemDelegate(this,"left"));
            //templateModel->record().
        }
        tableView->setAlternatingRowColors(true);
        tableView->resizeColumnsToContents();
        //tableView->sizeHintForColumn(int);
        tableView->horizontalHeader()->setResizeContentsPrecision(1000);
        head->setStretchLastSection(true);
        tableLabel->clear();
        tableLabel->setText(tr("Name of Table: %1").arg(strivgValue));
        tableLabel->setStyleSheet("font: bold; color: darkblue;");
        setFilter = false;
    }
}

void MainWindow::sortTable(int index)
{
    templateModel->setSort(index,Qt::AscendingOrder);
    templateModel->select();
}

void MainWindow::updatePanel(QModelIndex inDex)
{
    templateModel->select();
    tableView->resizeColumnsToContents();
    QHeaderView *head = tableView->horizontalHeader();
    head->setStretchLastSection(true);
    tableView->setCurrentIndex(inDex);
}

void MainWindow::addRecordOfTable()
{
    QString valueTemp = templateModel->tableName();
    if(valueTemp == "node"){
        NodeForm form("",this,false);
        form.exec();
    }else if(valueTemp == "unit"){
        UnitForm form("",this,false);
        form.exec();
    }else if(valueTemp == "storagespace"){
        StorageSpaceForm form("",this,false);
        form.exec();
    }else if(valueTemp == "contractor"){
        ContractorForm form("",this,false);
        form.exec();
    }else if(valueTemp == "organization"){
        OrganizationForm form("",this,false);
        form.exec();
    }else if(valueTemp == "nomenclature"){
        NomenclatureForm form("",this,false);
        form.exec();
    }else if(valueTemp == "ingridient"){
        IngredientForm form("",this,false);
        form.exec();
    }
    QModelIndex modIndex = tableView->currentIndex();
    MainWindow::updatePanel(modIndex);
}

void MainWindow::deleteRecordOfTable()
{
    QModelIndex index = tableView->currentIndex();
    if(index.isValid()){
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Attention!!!"));
        msgBox.setText(tr("Really delete?"));

        msgBox.setIcon(QMessageBox::Question);
        msgBox.addButton(QMessageBox::Yes);
        QPushButton *noButton = msgBox.addButton(QMessageBox::No);

        msgBox.setDefaultButton(noButton);

        noButton->setStyleSheet("QPushButton:hover {color: red}");
        int k = msgBox.exec();
        if(k == QMessageBox::Yes){
            QString valueTemp, iDValue;
            valueTemp = templateModel->tableName();
            QModelIndex index = tableView->currentIndex();
            QSqlRecord record = templateModel->record(index.row());
            if(valueTemp == "node"){
                iDValue = record.value("nodeid").toString();
                NodeForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "unit"){
                iDValue = record.value("unitid").toString();
                UnitForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "storagespace"){
                iDValue = record.value("storagespaceid").toString();
                StorageSpaceForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "contractor"){
                iDValue = record.value("contractorid").toString();
                ContractorForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "organization"){
                iDValue = record.value("organizationid").toString();
                OrganizationForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "nomenclature"){
                iDValue = record.value("nomenclatureid").toString();
                NomenclatureForm form(iDValue,this,false);
                form.deleteRecord();
            }else if(valueTemp == "ingridient"){
                iDValue = record.value("ingridientid").toString();
                IngredientForm form(iDValue,this,false);
                //form.deleteRecord();
            }
        }
    }
    updatePanel(index);
}

void MainWindow::editRecordOfTable()
{
    QString stringVar = templateModel->tableName();
    QModelIndex index = tableView->currentIndex();
    if(index.isValid()){
        QSqlRecord record =templateModel->record(index.row());
        if(stringVar == "node"){
            QString iD = record.value("nodeid").toString();
            NodeForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "unit"){
            QString iD = record.value("unitid").toString();
            UnitForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "storagespace"){
            QString iD = record.value("storagespaceid").toString();
            StorageSpaceForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "contractor"){
            QString iD = record.value("contractorid").toString();
            ContractorForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "organization"){
            QString iD = record.value("organizationid").toString();
            OrganizationForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "nomenclature"){
            QString iD = record.value("nomenclatureid").toString();
            NomenclatureForm form(iD, this, false);
            form.exec();
        }else if(stringVar == "ingridient"){
            QString iD = record.value("ingridientid").toString();
            IngredientForm form(iD, this, false);
            form.exec();
        }
    }

    QModelIndex modIndex = tableView->currentIndex();
    MainWindow::updatePanel(modIndex);
}

void MainWindow::viewUnit()
{
    viewTemplateTable("unit");
}

void MainWindow::viewNode()
{
    viewTemplateTable("node");
}

void MainWindow::viewStorageSpace()
{
    viewTemplateTable("storagespace");
}

void MainWindow::viewContractor()
{
    viewTemplateTable("contractor");
}

void MainWindow::viewOrganization()
{
    viewTemplateTable("organization");
}

void MainWindow::viewNomenclature()
{
    viewTemplateTable("nomenclature");
}

void MainWindow::viewIngridient()
{
    viewTemplateTable("ingridient");
}
