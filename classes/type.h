#ifndef TYPE_H
#define TYPE_H

#include "QVariantList"
#include "QString"

class Type
{
private:
    int id;
    QString name;
    QString description;
public:
    Type();
    Type(QVariantList type);
    int getId();
    QString getName();
    QString getDescription();

    void setName(QString newName);
    void setDescription(QString newDescription);
};

#endif // TYPE_H
