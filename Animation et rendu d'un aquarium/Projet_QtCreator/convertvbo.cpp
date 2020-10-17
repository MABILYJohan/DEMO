#include "convertvbo.h"

ConvertVbo::ConvertVbo()
{

}

/*-------------------------------------------------------------------------------------
 * Renvoie un ensemble de vertex sous la forme de QVector de GLfloat adapté pour un vbo
 * -----------------------------------------------------------------------------------*/
void ConvertVbo::convert_coords_to_vbo(QVector<GLfloat> &vertData,
                                                   std::vector<float> listCoords, int dim1,
                                                   std::vector<float> color, int dim2, bool unicolor)
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    QVector<GLfloat> vertices_coords;
    QVector<GLfloat> color_coords;

    if (listCoords.size()%dim1 != 0) {
        qDebug() << "ERROR in " << __FUNCTION__ << " @listCoords doesn't match @nbPoints";
        exit (1);
    }if (color.size()%dim2 != 0) {
        qDebug() << "ERROR in " << __FUNCTION__ << " @color doesn't match @nbcolors";
        exit (1);
    }
    int nbPoints = listCoords.size() / dim1;
    //qDebug() << "nbPoints" << nbPoints;

    for (unsigned c=0; c<listCoords.size(); c++) {
        vertices_coords.append(listCoords[c]);
    }
    for (unsigned c=0; c<color.size(); c++) {
        color_coords.append(color[c]);
    }

    for (int p = 0; p < nbPoints; ++p)
    {
        // coordonnées sommets
        for (int j = 0; j < dim1; j++)
            vertData.append(vertices_coords[p*dim1+j]);
        // couleurs/textures sommets
        for (int j = 0; j < dim2; j++) {
          if (unicolor) {
            vertData.append(color_coords[j]);
            }
            else {
                vertData.append(color_coords[p*dim2+j]);
            }
        }
    }

//    vbo.create();
//    vbo.bind();
//    vbo.allocate(vertData.constData(), vertData.count() * int(sizeof(GLfloat)));
//    qDebug() << "</" << __FUNCTION__ << ">";
}
