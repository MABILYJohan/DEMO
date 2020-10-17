#ifndef CONVERTVBO_H
#define CONVERTVBO_H

#include <QObject>
#include <QWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QDebug>
#include <vector>

using namespace std;

class ConvertVbo
{
public:
    ConvertVbo();

    static void convert_coords_to_vbo(QVector<GLfloat> &vertData,
                                 std::vector<float> listCoords,
                                 int dim1, std::vector<float> color, int dim2, bool unicolor = false);

};

#endif // CONVERTVBO_H
