
#include "dbmanager.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

dbManager &dbManager::getInstance()
{
    static dbManager instance;
    return instance;
}

dbManager::dbManager() {}

dbManager::~dbManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool dbManager::connect()
{
    dbName = "ecotaxi.db";

    QString appDir = QCoreApplication::applicationDirPath();
    QString dbPath = appDir + QDir::separator() + dbName;
    qDebug() << "connecting to db. db path: " << dbPath;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open())
    {
        qDebug() << "database connection error: " << db.lastError().text();
        return false;
    }
    else
    {
        qDebug() << "database connected successfully!";
        createTables();
        return true;
    }
}

void dbManager::resetDB()
{
    executeSet("DELETE FROM events;");
    executeSet("DELETE FROM charges;");
    executeSet("DELETE FROM drivers;");
    executeSet("DELETE FROM investors;");
    executeSet("DELETE FROM cars;");
    executeSet("DELETE FROM locations;");
    executeSet("DELETE FROM users;");
    executeSet("DELETE FROM types;");
    executeSet("DELETE FROM logins;");
    createTables();
}

void dbManager::createTables()
{
    createCarTable();
    createChargeTable();
    createDriverTable();
    createEventTable();
    createInvestorTable();
    createTypeTable();
    createLocationTable();
    createUserTable();
    createLoginTable();
}

bool dbManager::isConnected() const
{
    return db.isOpen();
}

bool dbManager::executeSet(const QString query)
{
    qDebug() << "set query execution: " << query;
    QSqlQuery q;
    if (!q.exec(query))
    {
        qDebug() << "set query error: " << q.lastError();
        return false;
    }
    return true;
}

QVariantList dbManager::executeGet(const QString query)
{
    qDebug() << "get query execution: " << query;
    QSqlQuery q;
    QVariantList out;
    if (q.exec(query))
    {
        while (q.next())
        {
            out.push_back(getValuesFromRecord(q.record()));
        }
    }
    else
    {
        qDebug() << "get query error: " << q.lastError();
    }
    return out;
}

QVariantList dbManager::getValuesFromRecord(const QSqlRecord &record)
{
    QVariantList values;
    for (int i = 0; i < record.count(); ++i)
    {
        values.append(record.value(i));
    }
    return values;
}

void dbManager::createInvestorTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS investors (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createCarTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS cars (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            sid INTEGER NOT NULL,
            brand TEXT NOT NULL,
            model TEXT NOT NULL,
            licensePlate TEXT NOT NULL,
            year INTEGER NOT NULL,
            investorId INTEGER NOT NULL,
            mileage FLOAT NOT NULL,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createDriverTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS drivers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createEventTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS events (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            carId INTEGER NOT NULL,
            driverId INTEGER NOT NULL,
            typeId INTEGER NOT NULL,
            amount FLOAT NOT NULL,
            description TEXT,
            date DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createChargeTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS charges (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            carId INTEGER NOT NULL,
            driverId INTEGER NOT NULL,
            locationId INTEGER NOT NULL,
            kwh FLOAT NOT NULL,
            duration FLOAT NOT NULL,
            date DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createTypeTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS types (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createLocationTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS locations (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createUserTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            password TEXT NOT NULL,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createLoginTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS logins (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date DATETIME DEFAULT CURRENT_TIMESTAMP,
            userId INTEGER NOT NULL
        )
    )Q";
    this->executeSet(createTableQuery);
}
