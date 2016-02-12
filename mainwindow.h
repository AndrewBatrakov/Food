#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtSql>
#include "lineedit.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readSettings();
    void writeSettings();
    void createPanel();
    void createActions();
    void createMenu();
    void aboutProgramm();
    void splitterCreate();
    void createContextMenu();
    void viewTemplateTable(QString);
    void sortTable(int);
    void updatePanel(QModelIndex);
    void prefixForm();

    void addRecordOfTable();
    void deleteRecordOfTable();
    void editRecordOfTable();
    //void copyRecordOfTable();

    void viewNode();
    void viewUnit();
    void viewStorageSpace();
    void viewContractor();
    void viewNomenclature();
    void viewIngridient();
    //void viewDisease();
    //void viewLocation();
//    void viewTypeOfWork();
    void viewOrganization();
//    void viewPreparation();
//    void viewPreparationDoc();

//    void putBaseProcedure();
//    void getBaseProcedure();
//    void searchProcedure();
//    //void readItem();
//    void exchangeData();

//    void sendMail();

private:
    QVBoxLayout *mainLayout;
    //QListWidget *listWidget;
    QWidget *panel;
    QSplitter *splitter;

    QTableView *tableView;
    QLabel *tableLabel;
    QSqlRelationalTableModel *templateModel;

    QPushButton *addRecordButton;
    QPushButton *editRecordButton;
    QPushButton *deleteRecordButton;

    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;
    QAction *copyAction;

    QMenu *fileMenu;
    QAction *prefixAction;
    QAction *exitAction;

    QMenu *referenceMenu;
    QAction *nodeAction;
    QAction *unitAction;
    QAction *storageSpaceAction;
    QAction *contractorAction;
    QAction *organizationAction;
    QAction *nomenclatureAction;
    QAction *ingridientAction;

    QMenu *documentMenu;
    QAction *preparationDocAction;

    QMenu *reportMenu;

    QMenu *serviceMenu;
    //QAction *ademantAction;
    //QAction *ftpAction;
    QAction *searchAction;
    QAction *putAction;
    QAction *getAction;
    QAction *exchangeAction;
    //QAction *vaccumAction;

    QMenu *helpMenu;
    QAction *aboutQtAction;
    QAction *aboutProgAction;

    //int addressInt, portInt;
    bool setFilter;
    QString filterTable;
    QFile fileExchange;

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
