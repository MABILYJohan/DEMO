#ifndef UTILS_H
#define UTILS_H

#include <cstdarg>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDebug>

class Utils
{
public:
    Utils();

    static void add_int_to_vector(QVector<int> *v, int nbValues, ...);
    static void add_float_to_vector(QVector<GLfloat> *v, int nbPoints, ...);
};

#endif // UTILS_H
