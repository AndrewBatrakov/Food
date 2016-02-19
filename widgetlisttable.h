#ifndef WIDGETLISTTABLE_H
#define WIDGETLISTTABLE_H

#include <QtWidgets>

class WidgetListTable : public QDialog
{
    Q_OBJECT

public:
    WidgetListTable(QString, QString, QWidget *parent);

    void done(int);
    QString rowOut() const {return iDTemp;};

private slots:
//    void addRecord();
//    void deleteRecord();
//    void editRecord();
//    void updatePanel(QModelIndex);
    void createContextMenu();
//    void searchProcedure();
    void viewTemplateTable(QString);
    QString tableSQL(QString,QString);

private:
    //QSqlRelationalTableModel *templateModel;
    QTableWidget *tableView;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;

    QString iDTemp, tableName;
    QString labelName, filterTable;
    bool setFilter;

    QHeaderView *headTable;
    //QTableWidgetItem *item;
};

#endif // WIDGETLISTTABLE_H
