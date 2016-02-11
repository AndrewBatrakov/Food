#include "getbase.h"

GetBase::GetBase(QWidget *parent) : QDialog(parent)
{
}

void GetBase::getBaseHttp()
{
    progressDialog = new QProgressDialog(this);
    fileHttp = new QFile("Food.dat");
    fileHttp->open(QIODevice::WriteOnly);
    url = "http://91.102.219.74/QtProject/MSO/Base/Food.dat";

    reply = http.get(QNetworkRequest(url));
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    connect(reply,SIGNAL(finished()),this,SLOT(httpDone()));

    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,
            SLOT(updateDataReadProgress(qint64,qint64)));
    connect(reply,SIGNAL(uploadProgress(qint64,qint64)),this,
            SLOT(updateDataReadProgress(qint64,qint64)));

    progressDialog->setLabelText(tr("Downloading DataBase ..."));
    progressDialog->setEnabled(true);
    progressDialog->exec();
}

void GetBase::httpDone()
{
    fileHttp->close();
    closeConnection();
}

void GetBase::updateDataReadProgress(qint64 readBytes, qint64 totalBytes)
{
    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(readBytes);
}

void GetBase::httpReadyRead()
{
    if(fileHttp)
        fileHttp->write(reply->readAll());
}

void GetBase::closeConnection()
{
    progressDialog->hide();
    http.destroyed();
}
