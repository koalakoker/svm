#include "jsonserial.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

JSONSerial::JSONSerial()
{
    m_object = new QJsonObject();
}

void JSONSerial::add(QString name, int value) {
    m_object->insert(name, value);
}
void JSONSerial::add(QString name, double value){
    m_object->insert(name, value);
}
void JSONSerial::add(QString name, bool value){
    m_object->insert(name, value);
}
void JSONSerial::add(QString name, QString value) {
    m_object->insert(name, value);
}

void JSONSerial::read(QString name, int& value) {
    value = m_object->value(name).toInt();
}
void JSONSerial::read(QString name, double& value) {
    value = m_object->value(name).toDouble();
}
void JSONSerial::read(QString name, bool& value) {
    value = m_object->value(name).toBool();
}
void JSONSerial::read(QString name, QString& value) {
    value = m_object->value(name).toString();
}

void JSONSerial::save(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << toJSON();
}
void JSONSerial::load(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);

    QString line;
    while (!in.atEnd()) {
        line += in.readLine();
    }
    if (m_object) {
        delete m_object;
    }
    m_object = new QJsonObject();
    fromJSON(line);
}
QString JSONSerial::toJSON() {
    QJsonDocument jsonDoc;
    jsonDoc.setObject(*m_object);
    return jsonDoc.toJson();
}
void JSONSerial::fromJSON(QString jsonString) {
    QByteArray qbJSON = jsonString.toUtf8();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(qbJSON);
    if(jsonDocument.isObject() == false) qDebug() << "It is not a JSON object";
    *m_object = jsonDocument.object();
}
