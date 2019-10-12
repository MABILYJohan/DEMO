#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include "point.h"
#include "utils.h"
#include "math.h"


using namespace std;

class Surface
{
public:
    // Constructeurs
    Surface();
    Surface(int degreN, int degreM);
    Surface(vector<vector<Point> > tab, int degreN, int degreM);
    virtual~Surface();

    // Accesseurs
    vector<vector<Point>> coords();
    int degre_n();
    int degre_m();

    // fonctions
    vector<vector<Point>> set_coords(vector<vector<Point>> tab, int n, int m);
    float polynome_Berstein(int n, int i, float t);

    virtual void display();
    virtual Point eval(float u, float v);


protected:
    vector<vector<Point>> _coords;
    int _degreN=0;
    int _degreM=0;

};

#endif // SURFACE_H
