#include "chargebydrivers.h"
#include "iostream"
using namespace std;

ChargeByDrivers::ChargeByDrivers() {}

ChargeByDrivers::ChargeByDrivers(QVariantList chargebydrivers)
{
    this->id = chargebydrivers[0].toInt();
    this->carId = chargebydrivers[1].toInt();
    this->driverId = chargebydrivers[2].toInt();
    this->locationId = chargebydrivers[3].toInt();
    this->kwh = chargebydrivers[4].toFloat();
    this->duration = chargebydrivers[5].toFloat();
    this->date = chargebydrivers[6].toDateTime();
}

QString ChargeByDrivers::getName()
{
    return name;
}
int ChargeByDrivers::getId()
{
    return id;
}

int ChargeByDrivers::getCarId()
{
    return carId;
}

int ChargeByDrivers::getDriverId()
{
    return driverId;
}

int ChargeByDrivers::getLocationId()
{
    return locationId;
}

float ChargeByDrivers::getKwh()
{
    return kwh;
}

float ChargeByDrivers::getDuration()
{
    return duration;
}

QDateTime ChargeByDrivers::getDate() {
    return date;
}

void ChargeByDrivers::setCarId(int newCarId)
{
    carId = newCarId;
}

void ChargeByDrivers::setDriverId(int newDriverId)
{
    driverId = newDriverId;
}

void ChargeByDrivers::setLocationId(int newLocationId)
{
    locationId = newLocationId;
}

void ChargeByDrivers::setKwh(float newKwh)
{
    kwh = newKwh;
}

void ChargeByDrivers::setDuration(float newDuration)
{
    duration = newDuration;
}

void ChargeByDrivers::setDate(QDateTime newDate) {
    date = newDate;
};
