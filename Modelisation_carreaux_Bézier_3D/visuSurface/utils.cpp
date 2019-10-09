#include "utils.h"

#include <QObject>
#include <QDebug>
#include <ctime>

Utils::Utils()
{

}

int Utils::factorielle(int nb)
{
    int val=1;
    if(nb==0) return 1;
    for (int i=1; i<=nb; i++)
    {
       val*=i;
    }
    return val;
}

int Utils::randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

/*---------------------------------------------------------------------------------
 * Convertit une liste vector<Point> en un tableau de
 * vector<vector<Point>>
 *------------------------------------------------------------------------------- */
vector<vector<Point>> Utils::list_to_tab(std::vector<Point> liste, int n, int m, int begin)
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    vector<vector<Point>> tab(n, vector<Point>(m));

    int k=begin;
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            tab[i][j] = liste[k];
            k++;
//            qDebug() << "\ttab[" << i << "][" << j << "] = "
//                     << tab[i][j].getX()
//                     << tab[i][j].getY()
//                     << tab[i][j].getZ();
        }
    }
//    qDebug() << "</" << __FUNCTION__ << ">";
    return tab;
}

/*---------------------------------------------------------------------------------
 * Convertit un tableau vector<vector<Point>> en liste vector<Point>
 *------------------------------------------------------------------------------- */
std::vector<Point> Utils::tab_to_list(vector<vector<Point>> tab, int n, int m)
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    std::vector<Point> liste;

    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            liste.push_back(tab[i][j]);
        }
    }

//    qDebug() << "</" << __FUNCTION__ << ">";
    return liste;
}
