#include "surface.h"

#include <QDebug>


Surface::Surface()
{
    ;
}

Surface::~Surface()
{
    ;
}

Surface::Surface(int degreN, int degreM)
{
    _degreN = degreN;
    _degreM = degreM;
    _coords.clear();
}

Surface::Surface(vector<vector<Point>> tab, int degreN, int degreM)
{
    _degreN = degreN;
    _degreM = degreM;
    _coords.clear();
    _coords = set_coords(tab, degreN, degreM);
}

vector<vector<Point>> Surface::coords(){ return _coords; }
int Surface::degre_n(){ return _degreN; }
int Surface::degre_m(){ return _degreM; }

vector<vector<Point>> Surface::set_coords(vector<vector<Point> > tab, int n, int m)
{
    vector<vector<Point>> list(n, vector<Point>(m));
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            list[i][j] = tab[i][j];
        }
    }
    return tab;
}

float Surface::polynome_Berstein(int n, int i, float t)
{
    //qDebug() << "<" << __FUNCTION__ << ">";


    float a = float(Utils::factorielle(n));
    float b = (float(Utils::factorielle(i)) * Utils::factorielle(n-i));
    a = a / b;
    float c = pow(t, i) * pow(1-t, n-i);

    float res = a * c;

    //qDebug() << "\tres = " << res;
    //qDebug() << "</" << __FUNCTION__ << ">";
    return res;
}

void Surface::display()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    for (int i=0; i<_degreN; i++)
    {
        for (int j=0; j<_degreM; j++)
        {
            qDebug() << "coords[" << i << "][" << j << "] = "
                     << _coords[i][j].getX()
                     << _coords[i][j].getY()
                     << _coords[i][j].getZ();
        }
    }
    qDebug() << "</" << __FUNCTION__ << ">";
}


Point Surface::eval(float u, float v)
{
    // Pour que les warning me saoulent pas, mais sinon ça sert à rien
    Point P;
    P.setX(u);
    P.setY(v);
    return P;
}


