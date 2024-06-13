#include <QCoreApplication>
#include <QTimer>
#include "authhandler.h"




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AuthHandler authHandler;
    authHandler.setAPIKey("AIzaSyAC92p6Wp-ezOQ335J7_rNxy2Coj4rndrs");

    // Connect the custom signal to handle the response once it's ready
    QObject::connect(&authHandler, &AuthHandler::userSignedIn, [](const QString &idToken){
        qDebug() << "ID Token obtained:" << idToken;
    });

    // Sign in the user
    authHandler.signUserIn("test@gmail.com", "Password123!");


    return a.exec();
}
