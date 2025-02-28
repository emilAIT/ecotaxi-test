#ifndef CHARGESBYDRIVERS_H
#define CHARGESBYDRIVERS_H

#include "QVariantList"
#include "QString"
#include "QDateTime"

using namespace std;

class ChargesByDrivers
{
private:
    int id;
    int carId;
    int driverId;
    float kwh;
    QDateTime date;

public:
    ChargesByDrivers();
    ChargesByDrivers(QVariantList charge);
    int getId();
    int getCarId();
    int getDriverId();
    float getKwh();
    QDateTime getDate();

    void setCarId(int newCarId);
    void setDriverId(int newDriverId);
    void setKwh(float newKwh);
    void setDate(QDateTime newDate);
};

#endif // CHARGE_H
