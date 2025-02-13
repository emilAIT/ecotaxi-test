#include "operations.h"
#include <QVariantList>

Operations::Operations() {}

// all
QList<Investor> Operations::selectAllInvestors()
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM investors");
    QList<Investor> investors;
    foreach (QVariant row, data)
    {
        Investor investor(row.toList());
        investors.append(investor);
    }
    return investors;
}

QList<Car> Operations::selectAllCars()
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM cars");
    QList<Car> cars;
    foreach (QVariant row, data)
    {
        Car car(row.toList());
        cars.append(car);
    }
    return cars;
}

QList<Driver> Operations::selectAllDrivers()
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM drivers");
    QList<Driver> drivers;
    foreach (QVariant row, data)
    {
        Driver driver(row.toList());
        drivers.append(driver);
    }
    return drivers;
}

QList<Event> Operations::selectAllEvents()
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM events");
    QList<Event> events;
    foreach (QVariant row, data)
    {
        Event event(row.toList());
        events.append(event);
    }
    return events;
}

QList<Event> Operations::selectEventsByDate(QDate date)
{
    QDate toDate = date.addDays(1);
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM events WHERE date BETWEEN '" + date.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'");
    QList<Event> events;
    foreach (QVariant row, data)
    {
        Event event(row.toList());
        events.append(event);
    }
    return events;
}

QList<Charge> Operations::selectAllCharges()
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM charges");
    QList<Charge> charges;
    foreach (QVariant row, data)
    {
        Charge charge(row.toList());
        charges.append(charge);
    }
    return charges;
}

QList<Charge> Operations::selectChargesByDate(QDate date)
{
    QDate toDate = date.addDays(1);
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM charges WHERE date BETWEEN '" + date.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'");
    QList<Charge> charges;
    foreach (QVariant row, data)
    {
        Charge charge(row.toList());
        charges.append(charge);
    }
    return charges;
}

QList<Type> Operations::selectAllTypes()
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM types");
    QList<Type> types;
    foreach (QVariant row, data)
    {
        Type type(row.toList());
        types.append(type);
    }
    return types;
}

QList<Location> Operations::selectAllLocations()
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM locations");
    QList<Location> locations;
    foreach (QVariant row, data)
    {
        Location location(row.toList());
        locations.append(location);
    }
    return locations;
}

QList<User> Operations::selectAllUsers()
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM users");
    QList<User> users;
    foreach (QVariant row, data)
    {
        User user(row.toList());
        users.append(user);
    }
    return users;
}

// Investor
bool Operations::addInvestor(Investor investor)
{
    dbManager &db = dbManager::getInstance();
    QString name = investor.getName();
    QString description = investor.getDescription();
    return db.executeSet("INSERT INTO investors (name, description) VALUES ('" + name + "', '" + description + "')");
}
bool Operations::updateInvestor(Investor newInvestor)
{
    dbManager &db = dbManager::getInstance();
    QString name = newInvestor.getName();
    QString description = newInvestor.getDescription();
    return db.executeSet("UPDATE investors SET name = '" + name + "', description = '" + description + "' WHERE id = " + QString::number(newInvestor.getId()));
}
bool Operations::deleteInvestor(int id)
{
    dbManager &db = dbManager::getInstance();
    backupManager::createInvestorBackup(id);
    deleteInvestorCars(id);
    return db.executeSet("DELETE FROM investors WHERE id = " + QString::number(id));
}
Investor Operations::getInvestor(int id)
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM investors WHERE id = " + QString::number(id));
    qDebug() << data;
    return Investor(data[0].toList());
}

void Operations::deleteInvestorCars(int id)
{
    dbManager &db = dbManager::getInstance();
    db.executeSet("DELETE FROM cars WHERE investorId = " + QString::number(id));
}

// Car
bool Operations::addCar(Car car)
{
    dbManager &db = dbManager::getInstance();
    QString brand = car.getBrand();
    QString model = car.getModel();
    QString licensePlate = car.getLicensePlate();
    int year = car.getYear();
    int investor = car.getInvestorId();
    float milleage = car.getMilleage();
    QString description = car.getDescription();
    return db.executeSet("INSERT INTO cars (sid, brand, model, licensePlate, year, investorId, mileage, description) VALUES (" + QString::number(car.getSid()) + ", '" + brand + "','" + model + "','" + licensePlate + "'," + QString::number(year) + "," + QString::number(investor) + "," + QString::number(milleage) + ",'" + description + "')");
}
bool Operations::updateCar(int id, Car newCar)
{
    dbManager &db = dbManager::getInstance();
    QString query = "UPDATE cars SET sid = " + QString::number(newCar.getSid()) + ", brand = '" + newCar.getBrand() + "', model = '" + newCar.getModel() + "', licensePlate = '" + newCar.getLicensePlate() + "', year = " + QString::number(newCar.getYear()) + ", investorId = " + QString::number(newCar.getInvestorId()) + ", mileage = " + QString::number(newCar.getMilleage()) + ", description = '" + newCar.getDescription() + "' WHERE id = " + QString::number(id);
    return db.executeSet(query);
}
bool Operations::deleteCar(int id)
{
    dbManager &db = dbManager::getInstance();
    backupManager::createCarBackup(id);
    return db.executeSet("DELETE FROM cars WHERE id = " + QString::number(id));
}
Car Operations::getCar(int id)
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM cars WHERE id = " + QString::number(id));
    if (data.isEmpty())
        return Car(QVariantList());
    else
        return Car(data[0].toList());
}

// Driver
bool Operations::addDriver(Driver driver)
{
    dbManager &db = dbManager::getInstance();
    QString name = driver.getName();
    QString description = driver.getDescription();
    return db.executeSet("INSERT INTO drivers (name, description) VALUES ('" + name + "','" + description + "')");
}

bool Operations::updateDriver(Driver driver)
{
    dbManager &db = dbManager::getInstance();
    return db.executeSet("UPDATE drivers SET name = '" + driver.getName() + "', description = '" + driver.getDescription() + "' WHERE id = " + QString::number(driver.getId()));
}

bool Operations::deleteDriver(int id)
{
    dbManager &db = dbManager::getInstance();
    backupManager::createDriverBackup(id);
    return db.executeSet("DELETE FROM drivers WHERE id = " + QString::number(id));
}

Driver Operations::getDriver(int id)
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM drivers WHERE id = " + QString::number(id));
    if (data.isEmpty())
        return Driver(QVariantList());
    else
        return Driver(data[0].toList());
}

// Event
bool Operations::addEvent(Event event)
{
    dbManager &db = dbManager::getInstance();
    int driverId = event.getDriverId();
    int typeId = event.getTypeId();
    float amount = event.getAmount();
    QDateTime date = event.getDate();
    QString description = event.getDescription();
    return db.executeSet("INSERT INTO events (driverId, carId, typeId, amount, description, date) VALUES (" + QString::number(driverId) + "," + QString::number(event.getCarId()) + "," + QString::number(typeId) + "," + QString::number(amount) + ",'" + description + "','" + date.toString("yyyy-MM-dd hh:mm:ss") + "')");
}

bool Operations::updateEvent(Event event)
{
    dbManager &db = dbManager::getInstance();
    return db.executeSet("UPDATE events SET driverId = " + QString::number(event.getDriverId()) + ", carId = " + QString::number(event.getCarId()) + ", typeId = " + QString::number(event.getTypeId()) + ", amount = " + QString::number(event.getAmount()) + ", description = '" + event.getDescription() + "' WHERE id = " + QString::number(event.getId()));
}

bool Operations::deleteEvent(int id)
{
    dbManager &db = dbManager::getInstance();
    return db.executeSet("DELETE FROM events WHERE id = " + QString::number(id));
}

Event Operations::getEvent(int id)
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM events WHERE id = " + QString::number(id));
    return Event(data[0].toList());
}

// Charge
bool Operations::addCharge(Charge charge)
{
    dbManager &db = dbManager::getInstance();
    int carId = charge.getCarId();
    int driverId = charge.getDriverId();
    int locationId = charge.getLocationId();
    float kwh = charge.getKwh();
    float duration = charge.getDuration();
    QDateTime date = charge.getDate();
    return db.executeSet("INSERT INTO charges (carId, driverId, locationId, kwh, duration, date) VALUES (" + QString::number(carId) + "," + QString::number(driverId) + "," + QString::number(locationId) + "," + QString::number(kwh) + "," + QString::number(duration) + ",'" + date.toString("yyyy-MM-dd hh:mm:ss") + "')");
}

bool Operations::updateCharge(Charge charge)
{
    dbManager &db = dbManager::getInstance();
    return db.executeSet("UPDATE charges SET carId = " + QString::number(charge.getCarId()) + ", driverId = " + QString::number(charge.getDriverId()) + ", locationId = " + QString::number(charge.getLocationId()) + ", kwh = " + QString::number(charge.getKwh()) + ", duration = " + QString::number(charge.getDuration()) + " WHERE id = " + QString::number(charge.getId()));
}

bool Operations::deleteCharge(int id)
{
    dbManager &db = dbManager::getInstance();
    return db.executeSet("DELETE FROM charges WHERE id = " + QString::number(id));
}

Charge Operations::getCharge(int id)
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM charges WHERE id = " + QString::number(id));
    return Charge(data[0].toList());
}

bool Operations::addType(Type type)
{
    dbManager &db = dbManager::getInstance();
    QString name = type.getName();
    QString description = type.getDescription();
    return db.executeSet("INSERT INTO types (name, description) VALUES ('" + name + "','" + description + "')");
}

bool Operations::updateType(Type type)
{
    dbManager &db = dbManager::getInstance();
    return db.executeSet("UPDATE types SET name = '" + type.getName() + "', description = '" + type.getDescription() + "' WHERE id = " + QString::number(type.getId()));
}

bool Operations::deleteType(int id)
{
    dbManager &db = dbManager::getInstance();
    return db.executeSet("DELETE FROM types WHERE id = " + QString::number(id));
}

Type Operations::getType(int id)
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM types WHERE id = " + QString::number(id));
    if (data.isEmpty())
        return Type(QVariantList());
    else
        return Type(data[0].toList());
}

bool Operations::addLocation(Location location)
{
    dbManager &db = dbManager::getInstance();
    QString name = location.getName();
    QString description = location.getDescription();
    return db.executeSet("INSERT INTO locations (name, description) VALUES ('" + name + "','" + description + "')");
}

bool Operations::updateLocation(Location location)
{
    dbManager &db = dbManager::getInstance();
    return db.executeSet("UPDATE locations SET name = '" + location.getName() + "', description = '" + location.getDescription() + "' WHERE id = " + QString::number(location.getId()));
}

bool Operations::deleteLocation(int id)
{
    dbManager &db = dbManager::getInstance();
    backupManager::createLocationBackup(id);
    return db.executeSet("DELETE FROM locations WHERE id = " + QString::number(id));
}

Location Operations::getLocation(int id)
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM locations WHERE id = " + QString::number(id));
    if (data.isEmpty())
        return Location(QVariantList());
    else
        return Location(data[0].toList());
}

// User
bool Operations::addUser(User user)
{
    dbManager &db = dbManager::getInstance();
    QString name = user.getName();
    QString password = user.getPassword();
    QString description = user.getDescription();
    return db.executeSet("INSERT INTO users (name, password, description) VALUES ('" + name + "','" + password + "','" + description + "')");
}

bool Operations::updateUser(User user)
{
    dbManager &db = dbManager::getInstance();
    return db.executeSet("UPDATE users SET name = '" + user.getName() + "', password = '" + user.getPassword() + "', description = '" + user.getDescription() + "' WHERE id = " + QString::number(user.getId()));
}

bool Operations::deleteUser(int id)
{
    dbManager &db = dbManager::getInstance();
    backupManager::createUserBackup(id);
    return db.executeSet("DELETE FROM users WHERE id = " + QString::number(id));
}

User Operations::getUser(int id)
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM users WHERE id = " + QString::number(id));
    if (data.isEmpty())
        return User(QVariantList());
    else
        return User(data[0].toList());
}

bool Operations::checkLoginUser(QString password)
{
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM users WHERE password = '" + password + "'");
    if (!data.isEmpty())
    {
        db.executeSet("INSERT INTO logins (userId) VALUES (" + QString::number(data[0].toList()[0].toInt()) + ")");
        return true;
    }
    return false;
}



QVariantList Operations::getAllEventsReport(QDate fromDate) {
    QDate toDate = fromDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query = "SELECT SUM(amount) FROM events WHERE date BETWEEN '" +
                    fromDate.toString("yyyy-MM-dd") + "' AND '" +
                    toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList Operations::getAllChargesReport(QDate fromDate) {
    QDate toDate = fromDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query = "SELECT SUM(kwh) FROM charges WHERE date BETWEEN '" +
                    fromDate.toString("yyyy-MM-dd") + "' AND '" +
                    toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

