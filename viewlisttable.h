#ifndef VIEWLISTTABLE_H
#define VIEWLISTTABLE_H

#include <QtWidgets>
#include <QtSql>

class QSqlRelationalTableModel;
class QTableView;

class ViewListTable : public QDialog
{
    Q_OBJECT

public:
    ViewListTable(QString, QString, QWidget *parent);

    void done(int);
    QString rowOut() const {return iDTemp;};

private slots:
    void addRecord();
    void deleteRecord();
    void editRecord();
    void updatePanel(QModelIndex);
    void createContextMenu();
    void searchProcedure();
    void viewTemplateTable(QString);

private:
    QSqlRelationalTableModel *templateModel;
    QTableView *tableView;
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
};

#endif // VIEWLISTTABLE_H
