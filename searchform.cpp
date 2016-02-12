#include <QtSql>
#include "searchform.h"

SearchForm::SearchForm(QString valueTempModel,QWidget *parent) :
    QDialog(parent)
{
    stTemp = "";
    labelSearch = new QLabel(tr("Search:"));
    editSearch = new LineEdit;

    searchPushButton = new QPushButton(tr("Search"));
    connect(searchPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    searchPushButton->setDefault(true);

    cancelPushButton = new QPushButton(tr("Cancel"));
    //cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(close()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(searchPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelSearch,0,0);
    mainLayout->addWidget(editSearch,0,1);
    //mainLayout->addWidget(field,1,1);
    mainLayout->addWidget(buttonBox,2,1);
    setLayout(mainLayout);
    setWindowTitle(tr("Search"));
}


void SearchForm::done(int result)
{
    stTemp = "";
    if (result == QDialog::Accepted) {
        stTemp = editSearch->text().simplified();
    }
    QDialog::done(result);
}
