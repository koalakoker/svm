#include "sdata.h"

SData::SData()
{
}

SData::SData(double d)
{
    m_data.append(d);
}

SData::SData(double d1, double d2)
{
    m_data.append(d1);
    m_data.append(d2);
}

SData::SData(double d1, double d2, double d3)
{
    m_data.append(d1);
    m_data.append(d2);
    m_data.append(d3);
}

SData::SData(double d1, double d2, double d3, double d4)
{
    m_data.append(d1);
    m_data.append(d2);
    m_data.append(d3);
    m_data.append(d4);
}

SData::SData(double d1, double d2, double d3, double d4, double d5)
{
    m_data.append(d1);
    m_data.append(d2);
    m_data.append(d3);
    m_data.append(d4);
    m_data.append(d5);
}

SData::SData(double d1, double d2, double d3, double d4, double d5, double d6)
{
    m_data.append(d1);
    m_data.append(d2);
    m_data.append(d3);
    m_data.append(d4);
    m_data.append(d5);
    m_data.append(d6);
}

SData::SData(double d1, double d2, double d3, double d4, double d5, double d6, double d7)
{
    m_data.append(d1);
    m_data.append(d2);
    m_data.append(d3);
    m_data.append(d4);
    m_data.append(d5);
    m_data.append(d6);
    m_data.append(d7);
}

SData::SData(double d1, double d2, double d3, double d4, double d5, double d6, double d7, double d8)
{
    m_data.append(d1);
    m_data.append(d2);
    m_data.append(d3);
    m_data.append(d4);
    m_data.append(d5);
    m_data.append(d6);
    m_data.append(d7);
    m_data.append(d8);
}

SData::SData(double d1, double d2, double d3, double d4, double d5, double d6, double d7, double d8, double d9)
{
    m_data.append(d1);
    m_data.append(d2);
    m_data.append(d3);
    m_data.append(d4);
    m_data.append(d5);
    m_data.append(d6);
    m_data.append(d7);
    m_data.append(d8);
    m_data.append(d9);
}

SData::SData(double d1, double d2, double d3, double d4, double d5, double d6, double d7, double d8, double d9, double d10)
{
    m_data.append(d1);
    m_data.append(d2);
    m_data.append(d3);
    m_data.append(d4);
    m_data.append(d5);
    m_data.append(d6);
    m_data.append(d7);
    m_data.append(d8);
    m_data.append(d9);
    m_data.append(d10);
}

void SData::setElementAt(int pos, double y)
{
    m_data[pos] = y;
}
