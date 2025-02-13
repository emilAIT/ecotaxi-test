#include "investor.h"
#include "iostream"
using namespace std;

Investor::Investor() {}

Investor::Investor(QVariantList investor)
{
    if (investor.isEmpty())
    {
        this->id = -1;
        this->name = "удален";
        this->description = "";
    }
    else
    {
        this->id = investor[0].toInt();
        this->name = investor[1].toString();
        this->description = investor[2].toString();
    }
}

int Investor::getId()
{
    return id;
}

QString Investor::getName()
{
    return name;
}

QString Investor::getDescription()
{
    return description;
}

void Investor::setName(QString newName)
{
    name = newName;
}

void Investor::setDescription(QString newDescription)
{
    description = newDescription;
}
