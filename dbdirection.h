#ifndef DBDIRECTION_H
#define DBDIRECTION_H

#include <QtWidgets>
#include <QtSql>

class DbDirection : public QDialog
{
    Q_OBJECT
public:
    DbDirection(QWidget *parent = 0);

    bool connectDataBase();

private:
    QSqlDatabase db;
};

#endif // DBDIRECTION_H
