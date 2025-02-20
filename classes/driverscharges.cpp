#include "driverscharges.h"
#include "iostream"
using namespace std;

DriversCharges::DriversCharges() {}

DriversCharges::DriversCharges(QVariantList driverscharges)
{
    this->id = driverscharges[0].toInt();
    this->carId = driverscharges[1].toInt();
    this->driverId = driverscharges[2].toInt();
    this->locationId = driverscharges[3].toInt();
    this->kwh = driverscharges[4].toFloat();
    this->duration = driverscharges[5].toFloat();
    this->date = driverscharges[6].toDateTime();
    this->name = driverscharges[7].toString();
}

QString DriversCharges::getName()
{
    return name;
}

int DriversCharges::getId()
{
    return id;
}

int DriversCharges::getCarId()
{
    return carId;
}

int DriversCharges::getDriverId()
{
    return driverId;
}

int DriversCharges::getLocationId()
{
    return locationId;
}

float DriversCharges::getKwh()
{
    return kwh;
}

float DriversCharges::getDuration()
{
    return duration;
}

QDateTime DriversCharges::getDate() {
    return date;
}

void DriversCharges::setCarId(int newCarId)
{
    carId = newCarId;
}

void DriversCharges::setDriverId(int newDriverId)
{
    driverId = newDriverId;
}

void DriversCharges::setLocationId(int newLocationId)
{
    locationId = newLocationId;
}

void DriversCharges::setKwh(float newKwh)
{
    kwh = newKwh;
}

void DriversCharges::setDuration(float newDuration)
{
    duration = newDuration;
}

void DriversCharges::setDate(QDateTime newDate) {
    date = newDate;
};
