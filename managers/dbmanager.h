#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariantList>
#include <QSqlRecord>
#include <QDir>
#include <QDebug>

class dbManager
{
public:
    static dbManager &getInstance();

    bool connect();
    bool isConnected() const;
    bool executeSet(const QString query);
    QVariantList executeGet(const QString query);

    void resetDB();

private:
    dbManager();
    ~dbManager();

    QVariantList getValuesFromRecord(const QSqlRecord &record);

    void createTables();

    void createInvestorTable();
    void createCarTable();
    void createDriverTable();
    void createEventTable();
    void createChargeTable();
    void createTypeTable();
    void createLocationTable();
    void createUserTable();
    void createLoginTable();

    QString dbName;
    QSqlDatabase db;
};

#endif // DBMANAGER_H
