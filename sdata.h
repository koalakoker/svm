#ifndef SDATA_H
#define SDATA_H

#include <QVector>

class SData
{
public:
    SData();
    SData(double d);
    SData(double d1, double d2);
    SData(double d1, double d2, double d3);
    SData(double d1, double d2, double d3, double d4);
    SData(double d1, double d2, double d3, double d4, double d5);
    SData(double d1, double d2, double d3, double d4, double d5, double d6);
    SData(double d1, double d2, double d3, double d4, double d5, double d6, double d7);
    SData(double d1, double d2, double d3, double d4, double d5, double d6, double d7, double d8);
    SData(double d1, double d2, double d3, double d4, double d5, double d6, double d7, double d8, double d9);
    SData(double d1, double d2, double d3, double d4, double d5, double d6, double d7, double d8, double d9, double d10);

    QVector<double> data() {return m_data;}
    void setData(QVector<double> data){m_data = data;}
    void append(double d){m_data.append(d);}
    void prepend(double d){m_data.prepend(d);}
    int size() {return m_data.size();}

    double value(){return elementAt(0);}
    double elementAt(int pos) {return m_data[pos];}
    void setElementAt(int pos, double y);

    double& operator[](int i){return m_data[i];}

private:
    QVector<double> m_data;
};

#endif // SDATA_H
