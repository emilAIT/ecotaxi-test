#ifndef CHARGEBYDRIVERS_H
#define CHARGEBYDRIVERS_H
#include "QVariantList"
#include "QString"
#include "QDateTime"

using namespace std;

class ChargeByDrivers
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
    ChargeByDrivers();
    ChargeByDrivers(QVariantList chargebydrivers);
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




#endif // CHARGEBYDRIVERS_H
