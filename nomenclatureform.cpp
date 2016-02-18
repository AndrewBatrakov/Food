#include "nomenclatureform.h"
#include <QtSql>
#include "numprefix.h"
#include "fordelete.h"
#include "viewlisttable.h"
#include "unitform.h"

NomenclatureForm::NomenclatureForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    QFile file(":/QToolButtonStyle.txt");
    file.open(QFile::ReadOnly);
    QString styleToolButton = QLatin1String(file.readAll());

    readSettings();
    indexTemp = id;

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f./0-9% ()\"]{150}");
    editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);
    connect(editName,SIGNAL(textChanged(QString)),SLOT(firstLetter(QString)));

    labelUnit = new QLabel(tr("Unit:"));
    editUnit = new LineEdit;
    editUnit->setValidator(new QRegExpValidator(regExp,this));
    QSqlQueryModel *unitModel = new QSqlQueryModel;
    unitModel->setQuery("SELECT unitname FROM unit");
    QCompleter *unitComplieter = new QCompleter(unitModel);
    unitComplieter->setCompletionMode(QCompleter::InlineCompletion);
    unitComplieter->setCompletionMode(QCompleter::PopupCompletion);
    unitComplieter->setCaseSensitivity(Qt::CaseInsensitive);
    editUnit->setCompleter(unitComplieter);
    QToolButton *addUnitButton = new QToolButton;
    QPixmap addPix(":/add.png");
    addUnitButton->setIcon(addPix);
    addUnitButton->setToolTip(tr("Add new Unit"));
    addUnitButton->setStyleSheet(styleToolButton);
    connect(addUnitButton,SIGNAL(clicked()),this,SLOT(addUnitRecord()));
    QToolButton *seeUnitButton = new QToolButton;
    QPixmap seePix(":/see.png");
    seeUnitButton->setIcon(seePix);
    seeUnitButton->setToolTip(tr("See this Unit"));
    seeUnitButton->setStyleSheet(styleToolButton);
    connect(seeUnitButton,SIGNAL(clicked()),this,SLOT(seeUnitRecord()));
    QToolButton *listUnitButton = new QToolButton;
    QPixmap listPix(":/list.png");
    listUnitButton->setIcon(listPix);
    listUnitButton->setToolTip(tr("List of Unit"));
    listUnitButton->setStyleSheet(styleToolButton);
    connect(listUnitButton,SIGNAL(clicked()),this,SLOT(listUnitRecord()));
    QHBoxLayout *unitLayout = new QHBoxLayout;
    unitLayout->addWidget(editUnit);
    unitLayout->addWidget(addUnitButton);
    unitLayout->addWidget(seeUnitButton);
    unitLayout->addWidget(listUnitButton);
    unitLayout->addStretch();

    labelCost = new QLabel(tr("Cost:"));
    editCost = new LineEdit;
    QRegExp regCost("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}");
    editCost->setValidator(new QRegExpValidator(regCost,this));
    connect(editCost,SIGNAL(textEdited(QString)),SLOT(changeDot(QString)));

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
        query.prepare("SELECT nom.nomenclaturename, "
                      "(SELECT un.unitname FROM unit AS un WHERE un.unitid = nom.unitid), "
                      "nom.cost "
                      "FROM nomenclature AS nom WHERE nom.nomenclatureid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        while(query.next()){
            editName->setText(query.value(0).toString());
            editUnit->setText(query.value(1).toString());
            double tt = query.value(2).toDouble();
            editCost->setText(QString::number(tt,'f',2));
        }
    }else{
        editName->clear();
        editUnit->clear();
        editCost->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addWidget(labelUnit,1,0);
    mainLayout->addLayout(unitLayout,1,1);
    mainLayout->addWidget(labelCost,2,0);
    mainLayout->addWidget(editCost,2,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,3,1);
        editName->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(tr("Nomenclature"));

    exchangeFile.setFileName("exchange.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }
    QWidget::setTabOrder(editName,editUnit);
    QWidget::setTabOrder(editUnit,saveButton);
    QWidget::setTabOrder(saveButton,cancelButton);
}

void NomenclatureForm::editRecord()
{
    if(!editName->text().isEmpty()){
        QTextStream stream(&exchangeFile);
        QString line;
        while(!stream.atEnd()){
            stream.readLine();
        }
        QSqlQuery query;
        query.prepare("SELECT * FROM nomenclature WHERE (nomenclaturename = :name AND cost = :cost)");
        query.bindValue(":name",editName->text().simplified());
        query.bindValue(":cost",editCost->text().toDouble());
        query.exec();
        query.next();
        if(!query.isValid()){
            if(indexTemp != ""){
                QSqlQuery query;
                query.prepare("UPDATE nomenclature SET nomenclaturename = :name, "
                              "unitid = (SELECT unitid FROM unit WHERE unitname = :unitname), "
                              "cost = :cost "
                              " WHERE nomenclatureid = :id");
                query.bindValue(":name",editName->text());
                query.bindValue(":unitname",editUnit->text());
                query.bindValue(":id",indexTemp);
                query.bindValue(":cost",editCost->text());
                query.exec();
                line += "UPDATE nomenclature SET nomenclaturename = '";
                line += editName->text().toUtf8();
                line += "', unitid = (SELECT unitid FROM unit WHERE unitname = '";
                line += editUnit->text().toUtf8();
                line += "', cost = '";
                line += editCost->text().toDouble();
                line += "' WHERE nomenclatureid = '";
                line += indexTemp;
                line += "'";
                line += "\r\n";
                stream<<line;
            }else{
                NumPrefix numPrefix(this);
                indexTemp = numPrefix.getPrefix("nomenclature");
                if(indexTemp == ""){
                    close();
                }else{
                    QSqlQuery query;
                    query.prepare("INSERT INTO nomenclature (nomenclatureid, nomenclaturename, unitid, cost) "
                                  "VALUES(:id, :name, "
                                  "(SELECT unitid FROM unit WHERE unitname = :unitname), :cost)");
                    query.bindValue(":id",indexTemp);
                    query.bindValue(":name",editName->text().simplified());
                    query.bindValue(":unitname",editUnit->text().simplified());
                    query.bindValue(":cost",editCost->text().toDouble());
                    query.exec();
                    line += "INSERT INTO unit (unitid, unitname, unitid) VALUES('";
                    line += indexTemp;
                    line += "', '";
                    line += editName->text().toUtf8();
                    line += "', (SELECT unitid FROM unit WHERE unitname = '";
                    line += editUnit->text().toUtf8();
                    line += "', '";
                    line += editCost->text().toDouble();
                    line += "')";
                    line += "\r\n";
                    stream<<line;
                }
            }
            emit accept();
            close();
        }else{
            QString tempString = editName->text();
            //tempString
            tempString += QObject::tr(" is availble!");
            QMessageBox::warning(this,QObject::tr("Attention!!!"),tempString);
        }
    }else{
        QMessageBox::warning(this,QObject::tr("Attention!!!"),tr("Name don't be empty!"));
    }
}

void NomenclatureForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"nomenclature",this);
    forDelete.result();
    forDelete.deleteOnDefault();
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    QSqlQuery query;
    query.prepare("DELETE FROM nomenclature WHERE nomenclatureid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    line += "DELETE FROM nomenclature WHERE nomenclatureid = '";
    line += indexTemp;
    line += "'";
    line += "\r\n";
    stream<<line;
}

void NomenclatureForm::done(int result)
{
    exchangeFile.close();
    writeSettings();
    QDialog::done(result);
}

void NomenclatureForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    restoreGeometry(settings.value("NomenclatureForm").toByteArray());
}

void NomenclatureForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    settings.setValue("NomenclatureForm", saveGeometry());
}

void NomenclatureForm::addUnitRecord()
{
    UnitForm openForm("",this,false);
    openForm.exec();
    if(openForm.rowOut() != ""){
        QSqlQuery query;
        query.prepare("SELECT unitname FROM unit WHERE unitid = :id");
        query.bindValue(":id",openForm.rowOut());
        query.exec();
        query.next();
        editUnit->setText(query.value(0).toString());
    }
}

void NomenclatureForm::seeUnitRecord()
{
    QSqlQuery query;
    query.prepare("SELECT unitid FROM unit WHERE unitname = :name");
    query.bindValue(":name",editUnit->text());
    query.exec();
    while(query.next()){
        UnitForm openForm(query.value(0).toString(),this,true);
        openForm.exec();
    }
}

void NomenclatureForm::listUnitRecord()
{
    QSqlQuery query;
    query.prepare("SELECT unitid FROM unit WHERE unitname = :name");
    query.bindValue(":name",editUnit->text());
    query.exec();
    query.next();
    ViewListTable listTemp(query.value(0).toString(),"unit",this);
    listTemp.exec();
    if(listTemp.rowOut() != ""){
        QSqlQuery query;
        query.prepare("SELECT unitname FROM unit WHERE unitid = :id");
        query.bindValue(":id",listTemp.rowOut());
        query.exec();
        query.next();
        editUnit->setText(query.value(0).toString());
    }
}

void NomenclatureForm::changeDot(QString st)
{
    if(st.contains(",")){
        st.replace(",",".");
        editCost->setText(st);
    }
}

void NomenclatureForm::firstLetter(QString st)
{
    QString qq = st.left(1).toUpper();
    //qq.toUpper();
    st.replace(0,1,qq);
    editName->setText(st);
}
