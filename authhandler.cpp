#include "authhandler.h"
#include <QDebug>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>

AuthHandler::AuthHandler(QObject *parent)
    : QObject(parent)
    , m_apiKey(QString())
{
    m_networkAccessManager = new QNetworkAccessManager(this);
    connect(this, &AuthHandler::userSignedIn, this, &AuthHandler::performAuthenticatedDatabaseCall);
}

AuthHandler::~AuthHandler()
{
    m_networkAccessManager->deleteLater();
}

void AuthHandler::setAPIKey(const QString &apiKey)
{
    m_apiKey = apiKey;
}

void AuthHandler::signUserUp(const QString &emailAddress, const QString &password)
{
    QString signupEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + m_apiKey;
    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant(variantPayload);
    performPOST(signupEndpoint, jsonPayload);
}

void AuthHandler::signUserIn(const QString &emailAddress, const QString &password)
{
    QString signInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + m_apiKey;
    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant(variantPayload);
    performPOST(signInEndpoint, jsonPayload);
}

void AuthHandler::networkReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
    qDebug() << "Network Reply:" << response;
    m_networkReply->deleteLater();
    parseResponse(response);
}

void AuthHandler::performAuthenticatedDatabaseCall()
{
    QString endPoint = "https://fir-qt-da647-default-rtdb.firebaseio.com/Pets.json?auth=" + m_idToken;
    m_networkReply = m_networkAccessManager->get(QNetworkRequest(QUrl(endPoint)));
    connect(m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead);
}

void AuthHandler::performPOST(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest newRequest((QUrl(url)));
    newRequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    m_networkReply = m_networkAccessManager->post(newRequest, payload.toJson());
    connect(m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead);
}

void AuthHandler::parseResponse(const QByteArray &response)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(response);
    if (jsonDocument.object().contains("error"))
    {
        qDebug() << "Error!" << response;
    }
    else if (jsonDocument.object().contains("kind"))
    {
        QString idToken = jsonDocument.object().value("idToken").toString();
        qDebug() << "User signed in successfully!";
        m_idToken = idToken;
        emit userSignedIn(idToken);  // Emit signal with idToken
    }
    else
    {
        qDebug() << "The response was:" << response;
    }
}

QString AuthHandler::getIdToken() const
{
    return m_idToken;
}
