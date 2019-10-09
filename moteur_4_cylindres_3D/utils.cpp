#include "utils.h"

Utils::Utils()
{

}


void Utils::add_int_to_vector(QVector<int> *v, int nbValues, ...)
{
    //qDebug() << "\t<" << __FUNCTION__ << ">";
    va_list ap;
    va_start(ap, nbValues);

    while (nbValues > 0)
    {
        int p;
        p = va_arg(ap, int);
        v->append(p);
        --nbValues;
    }
    va_end(ap);
    //qDebug() << "\t</" << __FUNCTION__ << ">";
}

void Utils::add_float_to_vector(QVector<GLfloat> *v, int nbValues, ...)
{
    //qDebug() << "\t<" << __FUNCTION__ << ">";
    va_list ap;
    va_start(ap, nbValues);

    while (nbValues > 0)
    {
        GLfloat p;
        double dbP;
        dbP = va_arg(ap, double);
        p = GLfloat(dbP);
        v->append(GLfloat(p));
        --nbValues;
    }
    va_end(ap);
    //qDebug() << "\t</" << __FUNCTION__ << ">";
}
