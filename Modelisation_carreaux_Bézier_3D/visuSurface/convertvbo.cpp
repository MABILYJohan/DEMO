#include "convertvbo.h"

ConvertVbo::ConvertVbo()
{

}

/*-------------------------------------------------------------------------------------
 * Renvoie un ensemble de vertex sous la forme de QVector de GLfloat adapté pour un vbo
 * -----------------------------------------------------------------------------------*/
QVector<GLfloat> ConvertVbo::convert_coords_to_vbo(std::vector<float> listCoords, int dim,
                                                   std::vector<float> _colors, int flagOneColor)
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    QVector<GLfloat> vertices;
    QVector<GLfloat> colors;
    QVector<GLfloat> vertData;

    if (listCoords.size()%dim != 0) {
        qDebug() << "ERROR in " << __FUNCTION__ << " @coords doesn't match @nbPoints";
        exit (1);
    }
    int nbPoints = listCoords.size() / dim;
    //qDebug() << "nbPoints" << nbPoints;

    for (unsigned c=0; c<listCoords.size(); c++)
        vertices.append(listCoords[c]);
    for (unsigned c=0; c<_colors.size(); c++)
        colors.append(_colors[c]);

    for (int p = 0; p < nbPoints; ++p)
    {
        // coordonnées sommets
        for (int j = 0; j < dim; j++)
            vertData.append(vertices[p*dim+j]);
        // couleurs sommets
        for (int j = 0; j < 3; j++)
            if (flagOneColor)
                vertData.append(colors[j]);
            else
                vertData.append(colors[p*dim+j]);
    }
//    qDebug() << "</" << __FUNCTION__ << ">";
    return vertData;
}

/*-------------------------------------------------------------------------------------
 * Prend une liste de Points et
 * renvoie un ensemble de vertex sous la forme de QVector de GLfloat adapté pour un vbo
 * (version mono couleur pour ttes les coords. A améliorer si on veut + de couleurs)
 * -----------------------------------------------------------------------------------*/
QVector<GLfloat> ConvertVbo::convert_listPoints_to_vbo(std::vector<Point> listPoint,
                                                       float colorR, float colorG, float colorB)
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    int dim=3;
    std::vector<float> coords;
    for (unsigned p=0; p<listPoint.size(); p++) {
        coords.push_back(listPoint[p].getX());
        coords.push_back(listPoint[p].getY());
        coords.push_back(listPoint[p].getZ());
    }
    //qDebug() << "coords : \n" << coords;
    QVector<GLfloat> vertData;
    std::vector<float> colors;
    colors.push_back(colorR);
    colors.push_back(colorG);
    colors.push_back(colorB);
    vertData = ConvertVbo::convert_coords_to_vbo(coords, dim, colors, 1);
//    qDebug() << "</" << __FUNCTION__ << ">";
    return vertData;
}

/*-------------------------------------------------------------------------------------
 * - Adapte les coordonnées pour dessiner un polygone de contrôle.
 * - Double les coordonnées et inverse pour la deuxième partie l'ordre de
 *      déclaration de lignes en colonnes.
 * - (Adapté pour un polygone de controlé de degré 3 (4x4 pts de contrôle))
 * - L'ordre des points dans la liste doit au préalable être déclaré comme tel:
 *      BOUCLE:
 *          lignes.
 *              colonnes.
 * -----------------------------------------------------------------------------------*/
QVector<GLfloat> ConvertVbo::convert_to_grid(std::vector<Point> listPoint,
                                             int dim1, int dim2,
                                             float colorR, float colorG, float colorB)
{
    for (int j=0; j<dim2; j+=1)
    {
        for (int i=0; i<dim1*dim2; i+=dim2)
        {
            listPoint.push_back(listPoint[j+i]);
        }
    }

    QVector<GLfloat> vertData = convert_listPoints_to_vbo(listPoint,
                                                          colorR, colorG, colorB);
    return vertData;
}

/*-------------------------------------------------------------------------------------
 * Adapte l'ordre de déclaration des cordonnées afin de pouvoir dessiner
 * à l'aide de triangles.
 * (il suffira de dessiner 3 par 3 les coordonnées avec un LINE_LOOP).
 * Attention le nouveau nombre de coordonnées sera égal à
 *  (@dim1-1) * (@dim2-1) * 6
 * -----------------------------------------------------------------------------------*/
QVector<GLfloat> ConvertVbo::convert_triangulation(std::vector<Point> listPoint,
                                                   int dim1, int dim2,
                                                   float colorR, float colorG, float colorB)
{
//    qDebug() << "<" << __FUNCTION__ << ">";
    std::vector<Point> listPointTriangle;

    // dim1 colonnes
    // dim2 lignes

    for (int i=0; i<(dim1*dim2)-dim2; i+=dim2)
    {
//        qDebug() << "i =" << i;
        for (int j=0; j<dim2-1; j++)
        {
            int k = i+j;
            listPointTriangle.push_back(listPoint[k]);
            listPointTriangle.push_back(listPoint[k+1]);
            listPointTriangle.push_back(listPoint[k+dim2]);
            //listPointTriangle.push_back(listPoint[k]);

            listPointTriangle.push_back(listPoint[k+dim2]);
            listPointTriangle.push_back(listPoint[k+1+dim2]);
            listPointTriangle.push_back(listPoint[k+1]);
            //listPointTriangle.push_back(listPoint[k+dim2]);
        }
    }

    QVector<GLfloat> vertData = convert_listPoints_to_vbo(listPointTriangle,
                                                          colorR, colorG, colorB);
//    qDebug() << "</" << __FUNCTION__ << ">";
    return vertData;
}


