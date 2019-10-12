#include "carreauxbezier.h"

#include <QDebug>
#include "discretisation.h"

CarreauxBezier::CarreauxBezier()
{
    ;
}

CarreauxBezier::~CarreauxBezier()
{
    ;
}

/*------------------------------------------------
 * Constructeur avec génération aléatoire
 * Initialise un carreaux de 4 lignes et 4 colonnes
 * avec la méthode <0> (modifiable à souhait).
 * ----------------------------------------------*/
CarreauxBezier::CarreauxBezier(int degreN, int degreM) : Surface(degreN, degreM)
{
    ;
    _coords = generer_ptsCtrl_aleatoire(ORDER);
    generer_carreaux(0, 4, 4);
}

vector<vector<Point>> CarreauxBezier::coordsCarreaux(){ return _coordsCarreaux; }

/*----------------------------------------------------------------------
 *  Génère un polygone de contrôle de degré 3 (4x4 points) aléatoire
 * --------------------------------------------------------------------*/
vector<vector<Point>> CarreauxBezier::generer_ptsCtrl_aleatoire(bool desordre)
{
    qsrand(time(NULL));
    vector<vector<Point>> list(_degreN, vector<Point>(_degreM));
    float xp, yp, zp;
    Point X;

    if (desordre)
    {
        for (int i=0; i<4; i++)
        {
            for (int j=0; j<4; j++) {
                xp = float(Utils::randInt(-100, 100)) / 100;
                yp = float(Utils::randInt(-100, 100)) / 100;
                zp = float(Utils::randInt(-100, 100)) / 100;
                X.set(xp, yp, zp);
                list[i][j] = X;
            }
        }
    }
    else
    {
        for (int i=0; i<4; i++)
        {
            for (int j=0; j<4; j++) {
                int intervMin = (j*50)-200;
                int intervMax = (j*50)-100;
                xp = float(Utils::randInt((i*50)+intervMin, (i*50)+intervMax)) / 100;
                yp = float(Utils::randInt(intervMin+50, intervMax+50)) / 100;
                zp = float(Utils::randInt(intervMin, intervMax)) / 100;
                X.set(xp, yp, zp);
                list[i][j] = X;
            }
        }
    }
    return list;
}

void CarreauxBezier::generer_carreaux(int methode, float nbU, float nbV)
{
    _nbU = nbU;
    _nbV = nbV;
    vector<Point> liste = Discretisation::discretiser_surface(this,
                                                               HOMOGENE, _nbU, _nbV);
    _coordsCarreaux.clear();
    _coordsCarreaux = Utils::list_to_tab(liste, nbU, nbV, 0);
}

// override
Point CarreauxBezier::eval(float u, float v)
{
    //qDebug() << "<" << __FUNCTION__ << ">";
    if (u<0.f || u>1.f || v<0.f || v>1.f) {
        qDebug() << "ERROR in " << __FUNCTION__ << " only @u and @v between 0 and 1";
        exit (1);
    }
    Point P;

    //qDebug() << "\tn = " << _degreN << "m=" << _degreM;
    float xp=0.f, yp=0.f, zp=0.f;
    for (int i=0; i<_degreN; i++)
    {
        float bin = polynome_Berstein(_degreN-1, i, u);
        for (int j=0; j<_degreM; j++)
        {
            float bjm = polynome_Berstein(_degreM-1, j, v);
            xp+= bin * bjm * _coords[i][j].getX();
            yp+= bin * bjm * _coords[i][j].getY();
            zp+= bin * bjm * _coords[i][j].getZ();
        }
    }
    P.set(xp, yp, zp);
    //qDebug() << "\tu=" << u << "v=" << v << "\n\t\txp=" << xp << "yp" << yp << "zp" << zp;

    //qDebug() << "</" << __FUNCTION__ << ">";
    return P;
}

// override
void CarreauxBezier::display()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    qDebug() << "PTS DE CTRL";
    for (int i=0; i<_degreN; i++)
    {
        for (int j=0; j<_degreM; j++)
        {
            qDebug() << "ptCtrl[" << i << "][" << j << "] = "
                     << _coords[i][j].getX()
                     << _coords[i][j].getY()
                     << _coords[i][j].getZ();
        }
    }
    qDebug() << "COORDS CARREAUX";
    for (int i=0; i<_nbU; i++)
    {
        for (int j=0; j<_nbV; j++)
        {
            qDebug() << "coords[" << i << "][" << j << "] = "
                     << _coords[i][j].getX()
                     << _coords[i][j].getY()
                     << _coords[i][j].getZ();
        }
    }
    qDebug() << "</" << __FUNCTION__ << ">";
}













