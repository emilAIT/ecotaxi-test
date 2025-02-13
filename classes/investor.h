#ifndef INVESTOR_H
#define INVESTOR_H

#include "QVariantList"

using namespace std;

class   Investor
{
private:
    int id;
    QString name;
    QString description;

public:
    Investor();
    Investor(QVariantList investor);

    int getId();
    QString getName();
    QString getDescription();
    void setName(QString newName);
    void setDescription(QString newDescription);
};

#endif // INVESTOR_H
