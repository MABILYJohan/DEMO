#include "point.h"
#include <QDebug>

Point::Point()
{
    coords = new float[3];
}

Point::~Point()
{
    delete [] coords;
    coords = nullptr;
}

Point::Point(const Point& p)
{
    coords = new float[3];
    for (unsigned i=0; i<3; ++i)
        coords[i] = p.coords[i];
}

void Point::display(int numPoint)
{
    qDebug() << "P" << numPoint <<  "=\tx :" << getX() << "\ty :" << getY() << "\tz :" << getZ() ;
}

float Point::getX() const
{
    return getN(0);
}

float Point::getY() const
{
    return getN(1);
}

float Point::getZ() const
{
    return getN(2);
}

void Point::get(float * t) const
{
    for (unsigned i=0; i<3; ++i)
        t[i] = coords[i];
}

float Point::getN(unsigned r) const
{
    if (r>2)
        r=2;
    return coords[r] ;
}

void Point::setN(unsigned r, const float & v)
{
    if (r>2)
        r=2;
    coords[r] = v;
}

void Point::set(const float * t)
{
    for (unsigned i=0; i<3; ++i)
        coords[i] = t[i];
}

void Point::set(float x, float y, float z)
{
    coords[0] = x;  coords[1] = y;  coords[2] = z;
}

void Point::setX(const float& v)
{
    setN(0, v);
}
void Point::setY(const float& v)
{
    setN(1, v);
}
void Point::setZ(const float& v)
{
    setN(2, v);
}

Point& Point::operator= (const Point &p)
{
    for (unsigned i=0; i<3; ++i)
        coords[i] = p.coords[i];

return *this;
}

std::ostream& operator<< (std::ostream& out, const Point& p)
{
    return out << "[ " << p.coords[0] << " " << p.coords[1] << " " << p.coords[2] << " ]"; // can access private member Y::data
}
