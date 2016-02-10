#include "prefixform.h"

PrefixForm::PrefixForm(QWidget *parent) : QDialog(parent)
{
    labelPrefix = new QLabel(tr("Name:"));
    editPrefix = new LineEdit;

    QRegExp regExpFamiliya("[A-Za-z]{3}");
    editPrefix->setValidator(new QRegExpValidator(regExpFamiliya,this));
    labelPrefix->setBuddy(editPrefix);

    saveButton = new QPushButton(tr("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editPN()));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    QSettings settings("AO_Batrakov_Inc.", "Food");
    if(settings.value("numprefix").toString() == ""){
        editPrefix->setText(tr("Three letters of the Latin alphabet"));
    }else{
        editPrefix->setText(settings.value("numprefix").toString());
    }
    editPrefix->setReadOnly(false);
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelPrefix,0,0);
    mainLayout->addWidget(editPrefix,0,1);
    mainLayout->addWidget(buttonBox,1,1);
    setLayout(mainLayout);

    setWindowTitle(tr("Prefix Number "));
}

void PrefixForm::editPN()
{
    QSettings settings("AO_Batrakov_Inc.", "Food");
    editPrefix->text().toUpper();
    settings.setValue("numprefix", editPrefix->text());

    close();
}


