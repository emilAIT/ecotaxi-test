#include "pages/login.h"
#include "managers/stylemanager.h"
#include "managers/dbmanager.h"
#include "managers/backupmanager.h"
#include <QDebug>
#include "QFontDatabase"
#include <QApplication>

#include <QSqlDatabase>
#include <QSqlError>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont("qrc:///Inter.ttf");

    styleManager s(&a);

    backupManager::createFolders();

    EncryptionManager &em = EncryptionManager::getInstance();

    em.setKey("kjafibsnkfnerougbervnmfejgrubdinsm");

    login w;

    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "\plugins\sqldrivers");


    dbManager &db = dbManager::getInstance();

    db.connectDB();

    w.show();
    return a.exec();
}

