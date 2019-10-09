#ifndef DISCRETISATION_H
#define DISCRETISATION_H

#include "surface.h"

enum en_paramtype{HOMOGENE, DESORDRE};

class Discretisation
{
public:
    Discretisation();

    static std::vector<Point> discretiser_surface(Surface *surf, int type,
                                           int nbParam1, int nbParam2);
};

#endif // DISCRETISATION_H
