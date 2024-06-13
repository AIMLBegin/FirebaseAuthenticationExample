#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

class AuthHandler : public QObject
{
    Q_OBJECT
public:
    explicit AuthHandler(QObject *parent = nullptr);
    ~AuthHandler();
    void setAPIKey(const QString &apiKey);
    void signUserUp(const QString &emailAddress, const QString &password);
    void signUserIn(const QString &emailAddress, const QString &password);
    QString getIdToken() const;

public slots:
    void networkReplyReadyRead();
    void performAuthenticatedDatabaseCall();

signals:
    void userSignedIn(const QString &idToken);  // Modified signal to include idToken

private:
    void performPOST(const QString &url, const QJsonDocument &payload);
    void parseResponse(const QByteArray &response);

    QString m_apiKey;
    QString m_idToken;
    QNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_networkReply;
};

#endif // AUTHHANDLER_H
