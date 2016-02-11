#ifndef GETBASE_H
#define GETBASE_H

#include <QtWidgets>
#include <QtNetwork>

class GetBase : public QDialog
{
    Q_OBJECT

public:
    GetBase(QWidget *parent);

public slots:
    void getBaseHttp();

private slots:
    void httpDone();
    void updateDataReadProgress(qint64,qint64);
    void httpReadyRead();

    void closeConnection();

private:
    QUrl url;
    QNetworkAccessManager http;
    QNetworkReply *reply;
    QFile *fileHttp;
    QProgressDialog *progressDialog;
};

#endif // GETBASE_H
