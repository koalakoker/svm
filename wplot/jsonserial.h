#ifndef JSONSERIAL_H
#define JSONSERIAL_H

#include <QString>
#include <QVector>
#include <QJsonObject>

class JSONSerial
{
public:
    JSONSerial();

    void add(QString name, int value);
    void add(QString name, double value);
    void add(QString name, bool value);
    void add(QString name, QString value);

    void read(QString name, int& value);
    void read(QString name, double& value);
    void read(QString name, bool& value);
    void read(QString name, QString& value);

    void save(QString fileName);
    void load(QString fileName);

private:
    QString toJSON();
    void fromJSON(QString jsonString);
    QJsonObject* m_object;
};

#endif // JSONSERIAL_H
