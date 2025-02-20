#ifndef DRIVERSCHARGES_H
#define DRIVERSCHARGES_H

#include "QVariantList"
#include "QString"
#include "QDateTime"

using namespace std;

class DriversCharges
{
private:
    QString name;

    int id;
    int carId;
    int driverId;
    int locationId;
    float kwh;
    float duration;
    QDateTime date;

public:
    DriversCharges();
    DriversCharges(QVariantList driverscharges);
    QString getName();

    int getId();
    int getCarId();
    int getDriverId();
    int getLocationId();
    float getKwh();
    float getDuration();
    QDateTime getDate();

    void setCarId(int newCarId);
    void setDriverId(int newDriverId);
    void setLocationId(int newLocationId);
    void setKwh(float newKwh);
    void setDuration(float newDuration);
    void setDate(QDateTime newDate);
};

#endif // DRIVERSCHARGES_H
