#ifndef NUMPREFIX_H
#define NUMPREFIX_H

#include <QtWidgets>
#include <QtSql>

class NumPrefix : public QDialog
{
    Q_OBJECT
public:
    NumPrefix(QWidget *parent = 0);

public slots:
    QString getPrefix(QString tableName);

};

#endif // NUMPREFIX_H
