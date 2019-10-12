#include "discretisation.h"

#include <QDebug>

Discretisation::Discretisation()
{

}



std::vector<Point> Discretisation::discretiser_surface(Surface *surf, int type,
        int nbParam1, int nbParam2)
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    std::vector<Point> listPoint;
    float pas1=0.f;
    float pas2=0.f;

    if (type == HOMOGENE)
    {
        pas1 = 1.f / float(nbParam1-1);
        pas2 = 1.f / float(nbParam2-1);
    }
    else {
        ;
    }

//    qDebug() << "EVAL:";

    float u=0.f;
    float v=0.f;
    for (int i=0; i<nbParam1; i++)
    {
        if (u>1.f)  u=1.f;
//        qDebug() << "u:" << u;
        for (int j=0; j<nbParam2; j++)
        {
            if (v>1.f)  v=1.f;
            listPoint.push_back(surf->eval(u, v));
            v+=pas2;
        }
        u+=pas1;
        v=0.f;
    }

//    qDebug() << "</" << __FUNCTION__ << ">";
    return listPoint;
}
