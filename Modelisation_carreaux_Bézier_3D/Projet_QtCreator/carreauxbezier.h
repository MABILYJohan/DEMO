#ifndef CARREAUXBEZIER_H
#define CARREAUXBEZIER_H


#include "surface.h"

enum en_ordre{ORDER, DISORDER};

class CarreauxBezier : public Surface
{

public:
    CarreauxBezier();
    CarreauxBezier(int degreN, int degreM);
    virtual~CarreauxBezier();

    vector<vector<Point>> coordsCarreaux();

    void generer_carreaux(int methode, float nbU, float nbV);

    Point eval(float u, float v) override;
    void display() override;


protected:
    vector<vector<Point>> _coordsCarreaux;
    float _nbU=0.f; // nb lignes
    float _nbV=0.f; // nb colonnes

    vector<vector<Point>> generer_ptsCtrl_aleatoire(bool desordre);

};

#endif // CARREAUXBEZIER_H
