#include "firebasedatabasehandler.h"
#include <QDebug>
#include <QNetworkRequest>

FirebaseDatabaseHandler::FirebaseDatabaseHandler(QObject *parent)
    : QObject(parent)
{
    m_networkAccessManager = new QNetworkAccessManager(this);
}

void FirebaseDatabaseHandler::setAuthHandler(AuthHandler *authHandler)
{
    m_authHandler = authHandler;
}

void FirebaseDatabaseHandler::performAuthenticatedGET(const QString &url)
{
    QNetworkRequest request(QUrl(url));
    request.setRawHeader("Authorization", "Bearer " + m_authHandler->getIdToken().toUtf8());

    m_networkReply = m_networkAccessManager->get(request);
    connect(m_networkReply, &QNetworkReply::readyRead, this, &FirebaseDatabaseHandler::onDatabaseReplyReadyRead);
}

void FirebaseDatabaseHandler::onDatabaseReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
    qDebug() << "Database response:" << response;
    m_networkReply->deleteLater();
}
