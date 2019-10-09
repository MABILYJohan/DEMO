#ifndef CONVERTVBO_H
#define CONVERTVBO_H

#include <QObject>
#include <QWidget>
#include <QOpenGLFunctions>
#include <QDebug>

#include "point.h"

using namespace std;

class ConvertVbo
{
public:
    ConvertVbo();

    static QVector<GLfloat> convert_coords_to_vbo(std::vector<float> listCoords, int dim, std::vector<float> _colors, int flagOneColor);
    static QVector<GLfloat> convert_listPoints_to_vbo(std::vector<Point> listPoint, float colorR, float colorG, float colorB);
    static QVector<GLfloat> convert_tabPoints_to_vbo(vector<vector<Point> > tab, int dim1, int dim2, float colorR, float colorG, float colorB);

    static QVector<GLfloat> convert_to_grid(std::vector<Point> listPoint, int dim1, int dim2, float colorR, float colorG, float colorB);
    static QVector<GLfloat> convert_triangulation(std::vector<Point> listPoint, int dim1, int dim2, float colorR, float colorG, float colorB);
};

#endif // CONVERTVBO_H
