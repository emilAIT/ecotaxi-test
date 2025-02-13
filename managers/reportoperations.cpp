#include "reportoperations.h"

ReportOperations::ReportOperations() {}

QVariantList ReportOperations::getCarsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "WITH car_stats AS (\n"
        "    SELECT\n"
        "        cars.id as carId,\n"
        "        cars.sid as carSid,\n"
        "        investors.name as investorName,\n"
        "        SUM(COALESCE(events.amount, 0)) as income,\n"
        "        SUM(CASE WHEN events.amount < 0 THEN COALESCE(events.amount, 0) ELSE 0 END) as outcome,\n"
        "        SUM(COALESCE(events.amount, 0)) as profit,\n"
        "        COUNT(DISTINCT DATE(events.date)) as daysWorked\n"
        "    FROM cars\n"
        "    LEFT JOIN events ON events.carId = cars.id AND DATE(events.date) BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    JOIN investors ON investors.id = cars.investorId\n"
        "    GROUP BY cars.id, investors.name\n"
        ")\n"
        "SELECT\n"
        "    carId,\n"
        "    carSid,\n"
        "    investorName,\n"
        "    income,\n"
        "    outcome,\n"
        "    profit,\n"
        "    daysWorked,\n"
        "    FLOOR(profit / MAX(1, daysWorked)) as averageProfitPerDay\n"
        "FROM car_stats;";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllCarsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "	SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "	SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "	SUM(events.amount) as profit\n"
        "FROM cars\n"
        "JOIN investors ON investors.id = cars.investorId\n"
        "JOIN events ON cars.id = events.carId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getTypesReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    types.id as typeId,\n"
        "    types.name as typeName,\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "    SUM(events.amount) as profit\n"
        "FROM types\n"
        "JOIN events ON events.typeId = types.id\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "GROUP BY types.id, types.name";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllTypesReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "	SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "	SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "	SUM(events.amount) as profit\n"
        "FROM types\n"
        "JOIN events ON events.typeId = types.id\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getDriversReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    drivers.id as driverId,\n"
        "    drivers.name as driverName,\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "    SUM(events.amount) as profit\n"
        "FROM drivers\n"
        "JOIN events ON events.driverId = drivers.id\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "GROUP BY drivers.id, drivers.name";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllDriversReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "	SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "	SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "	SUM(events.amount) as profit\n"
        "FROM drivers\n"
        "JOIN events ON events.driverId = drivers.id\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getInvestorsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    investors.id as investorId,\n"
        "    investors.name as investorName,\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "    SUM(events.amount) as profit\n"
        "FROM investors\n"
        "LEFT JOIN cars ON cars.investorId = investors.id\n"
        "LEFT JOIN events ON cars.id = events.carId AND events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "WHERE investors.id IN (SELECT investorId FROM cars)\n"
        "GROUP BY investors.id, investors.name";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllInvestorsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "	SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "	SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "	SUM(events.amount) as profit\n"
        "FROM investors\n"
        "JOIN cars ON cars.investorId = investors.id\n"
        "JOIN events ON cars.id = events.carId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getLocationsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    locations.id as locationId,\n"
        "    locations.name as locationName,\n"
        "    SUM(charges.kwh) as kwhSum\n"
        "FROM locations\n"
        "JOIN charges ON charges.locationId = locations.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "GROUP BY locations.id, locations.name";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllLocationsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "	SUM(charges.kwh) as kwhSum\n"
        "FROM locations\n"
        "JOIN charges ON charges.locationId = locations.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getChargesReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    cars.id as carId,\n"
        "    cars.sid as carSid,\n"
        "    SUM(charges.kwh) as kwhSum,\n"
        "    SUM(charges.duration) as durationSum\n"
        "FROM cars\n"
        "JOIN charges ON charges.carId = cars.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "GROUP BY cars.id";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    qDebug() << result;
    return result;
}

QVariantList ReportOperations::getAllChargesReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "	SUM(charges.kwh) as kwhSum\n"
        "FROM cars\n"
        "JOIN charges ON charges.carId = cars.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getUsersReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    logins.userId as userId,\n"
        "    logins.id as id,\n"
        "    logins.date as loginDateTime,\n"
        "    users.name as userName\n"
        "FROM logins\n"
        "JOIN users ON users.id = logins.userId\n"
        "WHERE logins.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "ORDER BY logins.date DESC";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        QVariantList rowList = row.toList();
        result.append(QVariant(rowList));
    }
    return result;
}

QVariantList ReportOperations::getAllUsersReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    COUNT(*) as loginCount\n"
        "FROM logins\n"
        "WHERE logins.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

///////////////////////////// reports for item /////////////////////////////////////////////////////
QVariantList ReportOperations::getCarReport(int carId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    events.date as eventDate,\n"
        "    CASE WHEN events.typeId = 0 THEN '-' ELSE CASE WHEN types.id IS NULL THEN 'удален' ELSE types.name END END as eventTypeName,\n"
        "    CASE WHEN events.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END as driverIdAndName,\n"
        "    events.amount as eventAmount,\n"
        "    events.description as eventDescription\n"
        "FROM events\n"
        "LEFT JOIN drivers ON drivers.id = events.driverId\n"
        "LEFT JOIN types ON types.id = events.typeId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND events.carId = " +
        QString::number(carId);
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllCarReport(int carId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as totalIncome,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as totalOutcome,\n"
        "    SUM(events.amount) as totalAmount\n"
        "FROM events\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND events.carId = " +
        QString::number(carId);
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result = data[0].toList();
    }
    return result;
}

QVariantList ReportOperations::getTypeReport(int typeId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    events.date as eventDate,\n"
        "    CASE WHEN events.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END as driverName,\n"
        "    CASE WHEN events.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удалена' ELSE cars.id END END as carId,\n"
        "    events.amount as eventAmount,\n"
        "    events.description as eventDescription\n"
        "FROM events\n"
        "LEFT JOIN drivers ON drivers.id = events.driverId\n"
        "LEFT JOIN cars ON cars.id = events.carId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND events.typeId = " +
        QString::number(typeId);
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllTypeReport(int typeId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as totalIncome,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as totalOutcome,\n"
        "    SUM(events.amount) as totalAmount\n"
        "FROM events\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND events.typeId = " +
        QString::number(typeId);
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0].toList());
    }
    qDebug() << result;
    return result;
}

QVariantList ReportOperations::getDriverReport(int driverId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    events.date as eventDate,\n"
        "    CASE WHEN events.typeId = 0 THEN '-' ELSE CASE WHEN types.id IS NULL THEN 'удален' ELSE types.name END END as eventTypeName,\n"
        "    CASE WHEN events.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удалена' ELSE cars.id END END as carId,\n"
        "    events.amount as eventAmount,\n"
        "    events.description as eventDescription\n"
        "FROM events\n"
        "JOIN drivers ON drivers.id = events.driverId\n"
        "LEFT JOIN cars ON cars.id = events.carId\n"
        "LEFT JOIN types ON types.id = events.typeId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND events.driverId = " +
        QString::number(driverId);
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllDriverReport(int driverId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as totalIncome,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as totalOutcome,\n"
        "    SUM(events.amount) as totalAmount\n"
        "FROM events\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND events.driverId = " +
        QString::number(driverId);
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result = data[0].toList();
    }
    return result;
}

QVariantList ReportOperations::getInvestorReport(int investorId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    cars.id as carId,\n"
        "    cars.sid as carSid,\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "    SUM(events.amount) as profit\n"
        "FROM investors\n"
        "JOIN cars ON cars.investorId = investors.id\n"
        "LEFT JOIN events ON cars.id = events.carId AND events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "WHERE investors.id = " +
        QString::number(investorId) + "\n" +
        "GROUP BY cars.id, investors.name";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllInvestorReport(int investorId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    investors.name as investorName,\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "    SUM(events.amount) as profit,\n"
        "    COUNT(DISTINCT cars.id) as carsCount\n"
        "FROM investors\n"
        "JOIN cars ON cars.investorId = investors.id\n"
        "LEFT JOIN events ON cars.id = events.carId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND investors.id = " +
        QString::number(investorId) + "\n" +
        "GROUP BY investors.name";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        QVariantList rowData = row.toList();
        result.append(rowData);
    }
    if (result.isEmpty())
    {
        result.append({0, 0, 0, 0});
    }
    return result;
}

QVariantList ReportOperations::getLocationReport(int locationId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    charges.date as date,\n"
        "    CASE WHEN cars.id IS NULL THEN 'удалена' ELSE cars.id END as carId,\n"
        "    CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END as driverIdAndName,\n"
        "    charges.kwh as kwh,\n"
        "    charges.duration as duration\n"
        "FROM locations\n"
        "JOIN charges ON charges.locationId = locations.id\n"
        "LEFT JOIN cars ON cars.id = charges.carId\n"
        "LEFT JOIN drivers ON drivers.id = charges.driverId\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND locations.id = " +
        QString::number(locationId);
    QVariantList data = db.executeGet(query);

    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllLocationReport(int locationId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    SUM(charges.kwh) as kwhSum\n"
        "FROM locations\n"
        "JOIN charges ON charges.locationId = locations.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND locations.id = " +
        QString::number(locationId);
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result = data[0].toList();
    }
    return result;
}

QVariantList ReportOperations::getChargesByCarReport(int id, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    charges.date as chargeDate,\n"
        "    drivers.name as chargeDriver,\n"
        "    locations.name as locationName,\n"
        "    charges.kwh as kwh,\n"
        "    charges.duration as duration\n"
        "FROM cars\n"
        "JOIN charges ON cars.id = charges.carId\n"
        "JOIN drivers ON drivers.id = charges.driverId\n"
        "JOIN locations ON locations.id = charges.locationId\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND cars.id = " +
        QString::number(id);
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    qDebug() << result;
    return result;
}

QVariantList ReportOperations::getAllChargesByCarReport(int id, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    SUM(charges.kwh) as kwhSum\n"
        "FROM cars\n"
        "JOIN charges ON cars.id = charges.carId\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND cars.id = " +
        QString::number(id);
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result = data[0].toList();
    }
    return result;
}

QVariantList ReportOperations::getUserReport(int userId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    logins.id as id,\n"
        "    logins.date as datetime\n"
        "FROM logins\n"
        "WHERE logins.userId = " +
        QString::number(userId) + "\n"
                                  "  AND logins.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd HH:mm:ss") + "' AND '" + toDate.toString("yyyy-MM-dd HH:mm:ss") + "'\n"
                                                                                                        "ORDER BY logins.date DESC";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        result.append(row);
    }
    return result;
}

QVariantList ReportOperations::getAllUserReport(int userId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    COUNT(*) as loginCount\n"
        "FROM logins\n"
        "WHERE logins.userId = " +
        QString::number(userId) + "\n"
                                  "  AND logins.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd HH:mm:ss") + "' AND '" + toDate.toString("yyyy-MM-dd HH:mm:ss") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}
