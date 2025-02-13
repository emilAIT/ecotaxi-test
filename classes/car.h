#ifndef CAR_H
#define CAR_H

#include "QVariantList"
#include "QString"

using namespace std;

class Car {
private:
    int id;
    int sid;
    QString brand;
    QString model;
    QString licensePlate;
    int year;
    int investorId;
    float milleage;
    QString description;

public:
    Car();
    Car(QVariantList car);

    int getId();
    int getSid();
    QString getBrand();
    QString getModel();
    QString getLicensePlate();
    int getYear();
    int getInvestorId();
    float getMilleage();
    QString getDescription();

    void setBrand(QString newBrand);
    void setModel(QString newModel);
    void setLicensePlate(QString newLicensePlate);
    void setYear(int newYear);
    void setInvestor(int newInvestorId);
    void setMilleage(float newMilleage);
    void setDescription(QString newDescription);
};

#endif // CAR_H
