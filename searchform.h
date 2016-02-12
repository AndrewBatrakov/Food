#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QtWidgets>
#include "lineedit.h"

class SearchForm : public QDialog
{
    Q_OBJECT
public:
    SearchForm(QString, QWidget *parent);

    void done(int result);
    QString rowOut() const {return stTemp;};

private:
    QLabel *labelSearch;
    LineEdit *editSearch;

    QComboBox *field;

    QPushButton *searchPushButton;
    QPushButton *cancelPushButton;
    QDialogButtonBox *buttonBox;

    QString stTemp;
};

#endif // SEARCHFORM_H
