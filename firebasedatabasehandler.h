#ifndef FIREBASEDATABASEHANDLER_H
#define FIREBASEDATABASEHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include "authhandler.h"

class FirebaseDatabaseHandler : public QObject
{
    Q_OBJECT
public:
    explicit FirebaseDatabaseHandler(QObject *parent = nullptr);
    void setAuthHandler(AuthHandler *authHandler);
    void performAuthenticatedGET(const QString &url);

private slots:
    void onDatabaseReplyReadyRead();

private:
    QNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_networkReply;
    AuthHandler *m_authHandler;
};

#endif // FIREBASEDATABASEHANDLER_H
