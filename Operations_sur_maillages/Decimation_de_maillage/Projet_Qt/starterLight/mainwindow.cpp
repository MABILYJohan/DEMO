#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>

using namespace std;

reductInfos MainWindow::maj_RI(int nbEdges, int precision, int nbReduct)
{
    reductInfos ri;
    ri.nbEdges = nbEdges;
    ri.precision = precision;
    ri.nbReduct = nbReduct;
    return ri;
}

void MainWindow::showEdgeSelection(MyMesh* _mesh)
{
    // on réinitialise les couleurs de tout le maillage
    resetAllColorsAndThickness(_mesh);

    /*
     * cette fonction utilise la variables de sélection edgeSelection qui est l'ID de
     * l'arête sélectionnée et qui est égale à -1 si la sélection est vide
     */

    if (edgeSelection>=0 && edgeSelection<_mesh->n_edges()) {
        EdgeHandle eh = _mesh->edge_handle(edgeSelection);
        _mesh->set_color(eh, MyMesh::Color(0, 255, 0));
        _mesh->data(eh).thickness = 4;
    }

    // on affiche le nouveau maillage
    displayMesh(_mesh);
}

void MainWindow::collapseEdge(MyMesh* _mesh, int edgeID)
{
    /*
     * cette fonction utilise l'opérateur collapse pour supprimer l'arête d'index edgeID
     * Attention à ne pas oublier garbage_collection() !
     */

    EdgeHandle eh = _mesh->edge_handle(edgeID);
    MyMesh::HalfedgeHandle heh = _mesh->halfedge_handle(eh, 0);

    if (_mesh->is_collapse_ok(heh)) {
        MyMesh::VertexHandle vh0 = _mesh->to_vertex_handle(heh);
        MyMesh::VertexHandle vh1 = _mesh->from_vertex_handle(heh);
        MyMesh::Point newVertP = (_mesh->point(vh0) + _mesh->point(vh1))/2.f;
        _mesh->set_point(vh0, newVertP);
        _mesh->collapse(heh);
    }
    else {
        ;
        //qDebug() << "no collapse edge possible";
    }
    // permet de nettoyer le maillage et de garder la cohérence des indices après un collapse
    _mesh->garbage_collection();
}

/*-------------------------------------------------------------------
 * Renvoie un intervalle entre 0 et la valeur renvoyée, correspondant
 * à une angle.
 * @precision pourcentage de 0 à 1 de précision
 * -------------------------------------------------------------------*/
float MainWindow::param_plan(MyMesh *_mesh, float precision)
{
    float seuil=0.f;

    float min = LONG_MAX;
    float max = 0.f;

    // parcours des arêtes
    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        EdgeHandle eh = *curEdge;

        MyMesh::HalfedgeHandle heh0 = _mesh->halfedge_handle(eh, 0);
        if (!_mesh->is_collapse_ok(heh0)) {
            //qDebug() << "\t\tedge no collapse";
            continue;
        }

        MyMesh::HalfedgeHandle heh1 = _mesh->halfedge_handle(eh, 1);
        VertexHandle vh0 = _mesh->to_vertex_handle(heh0);
        VertexHandle vh1 = _mesh->to_vertex_handle(heh1);

        float angleMoyen0 = calc_moyenne_angle_point(_mesh, vh0.idx());
        //qDebug() << "\t\tangleMoyen0 = " << angleMoyen0;
        float angleMoyen1 = calc_moyenne_angle_point(_mesh, vh1.idx());
        //qDebug() << "\t\tangleMoyen1 = " << angleMoyen1;
        // Moyenne des 2 valeurs
        float currentValue = (angleMoyen0 + angleMoyen1) / 2.f;
        //qDebug() << "\t\tcurrentValue = " << currentValue;

        //qDebug() << "\t\tfabs(currentValue) =" << fabs(currentValue) << ">";

        if (fabs(currentValue) < min)
            min = currentValue;
        if (fabs(currentValue) > max)
            max = currentValue;

    }
    //qDebug() << "\t\tmax - min =" << max - min ;
    seuil = min + ((max-min) * precision);
    return seuil;
}

/*-------------------------------------------------------------------
 * Renvoie un intervalle entre 0 et la valeur renvoyée, correspondant
 * à une angle.
 * @precision pourcentage de 0 à 1 de précision
 * -------------------------------------------------------------------*/
float MainWindow::param_angle(MyMesh *_mesh, float precision)
{
    float seuil=0.f;

    float min = LONG_MAX;
    float max = 0.f;

    // parcours des arêtes
    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        EdgeHandle eh = *curEdge;

        MyMesh::HalfedgeHandle heh0 = _mesh->halfedge_handle(eh, 0);
        if (!_mesh->is_collapse_ok(heh0)) continue;

        if (!_mesh->is_boundary(eh))
        {
            MyMesh::HalfedgeHandle heh1 = _mesh->halfedge_handle(eh, 1);
            VertexHandle vh0 = _mesh->to_vertex_handle(heh0);
            VertexHandle vh1 = _mesh->to_vertex_handle(heh1);
            FaceHandle fh0 = _mesh->face_handle(heh0);
            FaceHandle fh1 = _mesh->face_handle(heh1);
            if (fh0.idx() < 0 || fh1.idx() < 0) {
                continue;
            }

            float angle = fabs(angleFF(_mesh,fh0.idx(), fh1.idx(), vh0.idx(), vh1.idx()));
            //qDebug() << "\t\tangle = " << angle;

            if (angle < min)
                min = angle;
            if (angle > max)
                max = angle;
        }
    }

    //qDebug() << "\t\tmax - min =" << max - min ;
    seuil = min + ((max-min) * precision);

    return seuil;
}

/*-------------------------------------------------------------------
 * Renvoie un intervalle entre 0 et la valeur renvoyée, correspondant
 * à une taille d'arete.
 * @precision pourcentage de 0 à 1 de précision
 * -------------------------------------------------------------------*/
float MainWindow::param_taille(MyMesh *_mesh, float precision)
{
    float seuil=0.f;
    float edgeMoyenne=0.f;

    float min = LONG_MAX;
    float max = 0.f;

    // parcours des arêtes
    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        EdgeHandle eh = *curEdge;
        float edgeLength = _mesh->calc_edge_length(eh);
        edgeMoyenne += edgeLength;
        if (edgeLength < min)   min = edgeLength;
        if (edgeLength > max)   max = edgeLength;
    }
    //qDebug() << "\t\tmin =" << min ;
    //qDebug() << "\t\tmax =" << max ;
    //qDebug() << "\t\tmax - min =" << max - min ;
    seuil = min + ((max-min) * precision);
    return seuil;
}

/*-------------------------------------------------------------------
 * @precision pourcentage de 0 à 1 de précision du paramètre @param
 * -------------------------------------------------------------------*/
float MainWindow::param_seuil(MyMesh *_mesh, int param, float precision)
{
    float seuil=0.f;
    if (precision > 1.f) precision = 1.f;
    if (precision < 0.f) precision = 0.f;

    if (param == PARAM_TAILLE)
    {
        seuil = param_taille(_mesh, precision);
    }
    else if (param == PARAM_ANGLE)
    {
        seuil = param_angle(_mesh, precision);
    }
    else
    {
        seuil = param_plan(_mesh, precision);
    }
    return seuil;
}

// fonction pratique pour faire des tirages aléatoires
int randInt(int low, int high){return qrand() % ((high + 1) - low) + low;}

void MainWindow::decimationAleatoire(MyMesh *_mesh, int percent)
{

    int nbEdgesBase = _mesh->n_edges();
    float limPercent = (float(percent)/100.f);
    limPercent = 1.f - limPercent;
    float currentPercent = 1.f;
    while (currentPercent>limPercent)
    {
        collapseEdge(_mesh, randInt(0, currentPercent*_mesh->n_edges()));
        currentPercent = float(_mesh->n_edges()) / float(nbEdgesBase);
    }
    _nbAreteAleatoire = _mesh->n_edges();
}

std::vector<EdgeHandle> tri_insertion(MyMesh *_mesh, std::vector<EdgeHandle> tab)
{
    if (tab.empty()) return tab;

    unsigned i, j;
    for (i = 1; i < tab.size(); ++i)
    {
        EdgeHandle eh = tab[i];
        float elem = _mesh->calc_edge_length(eh);

        for (j = i; j > 0 && _mesh->calc_edge_length(tab[j-1]) > elem; j--) {
            tab[j] = tab[j-1];
        }
        tab[j] = eh;
    }
    return tab;
}

/*-------------------------------------------------------------
 * Renvoi la plus petite arete (taille) pouvant être supprimée
 * -----------------------------------------------------------*/
int plusPetiteEdgeCollapse(MyMesh *_mesh, float limite)
{
    if (_mesh->n_edges() <= 0)  return -1;
    float min = LONG_MAX;
    int idEdge=-1;
    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        EdgeHandle eh = *curEdge;
        MyMesh::HalfedgeHandle heh = _mesh->halfedge_handle(eh, 0);
        if (_mesh->is_collapse_ok(heh))
        {
            float edgeLength = _mesh->calc_edge_length(eh);
            if (edgeLength < min) {
                min = edgeLength;
                idEdge = eh.idx();
            }
            //qDebug() << "\t\tedgeLength = " << edgeLength;
            // si arete plus petite que la limite, on a pas besoin d'en trouver une plus petite
            if (edgeLength <= limite) break;
        }
    }
    return idEdge;
}



void MainWindow::decimationTaille(MyMesh *_mesh, int percent)
{

    int nbAretesSupr = _mesh->n_edges() * (float(percent)/100.f);
    nbAretesSupr = _mesh->n_edges() - nbAretesSupr;

    float seuil = param_seuil(_mesh, PARAM_TAILLE, _precision);

    _progress = new QProgressDialog("simplification...", "Annuler", 0, nbAretesSupr, this);
    _progress->setWindowModality(Qt::WindowModal);

    int base_nb_edges = _mesh->n_edges();
    int iter_progress=0;

    for (int i=0; i<nbAretesSupr && _mesh->n_edges()>nbAretesSupr; i++)
    {
        if (_progress->wasCanceled())
            break;
        int idEdge = plusPetiteEdgeCollapse(_mesh, seuil);
        if (idEdge<0)  {
            _progress->cancel();
            break;
        }
        collapseEdge(_mesh, idEdge);
        //if (i!=0 && i%100==0) qDebug() << "i" << i;
        iter_progress = base_nb_edges - _mesh->n_edges();
        _progress->setValue(iter_progress);
    }
    _progress->setValue(nbAretesSupr);
    _riTaille = maj_RI(_mesh->n_edges(), 100-(_precision*100), _nbReduct);

}

void MainWindow::decimationTailleQS(MyMesh *_mesh, int percent)
{

    int nbAretesSupr = _mesh->n_edges() * (float(percent)/100.f);

    std::vector<EdgeHandle> tabEdgeLenght;
    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        EdgeHandle eh = *curEdge;
        tabEdgeLenght.push_back(eh);
    }

    tabEdgeLenght = tri_insertion(_mesh, tabEdgeLenght);

    for (int i=0; i<tabEdgeLenght.size(); i++)
    {
        EdgeHandle eh = tabEdgeLenght[i];
        MyMesh::HalfedgeHandle heh = _mesh->halfedge_handle(eh, 0);
        if (_mesh->is_collapse_ok(heh))
        {
            collapseEdge(_mesh, eh.idx());
        }
    }
    //_nbAreteTaille = _mesh->n_edges();
}


float MainWindow::angleFF(MyMesh* _mesh, int faceID0, int faceID1, int vertID0, int vertID1)
{
    /* **** à compléter ! **** */

    //qDebug() << "<" << __FUNCTION__ << ">";

    FaceHandle fh0 = _mesh->face_handle(faceID0);
    FaceHandle fh1 = _mesh->face_handle(faceID1);
    MyMesh::Point vNFace0 = _mesh->calc_face_normal(fh0);
    MyMesh::Point vNFace1 = _mesh->calc_face_normal(fh1);

    vNFace0.normalize();
    vNFace1.normalize();

    VertexHandle vh1 = _mesh->vertex_handle(vertID0);
    VertexHandle vh2 = _mesh->vertex_handle(vertID1);

    float angle=0.f;
    if ( ((vNFace0 % vNFace1) | (_mesh->point(vh2) - _mesh->point(vh1))) >=0)
    {
        //qDebug() << "\t+";
        angle = acos( (vNFace0|vNFace1) );
    }
    else {
        //qDebug() << "\t-";
        angle = -acos( (vNFace0|vNFace1) );
    }

    //qDebug() << "</" << __FUNCTION__ << ">";
    return angle;;
}

/*-------------------------------------------------------------
 * Renvoi l'arete dont les faces voisines sont le plus proche
 * d'un plan
 * @precision valeur de 0 à 1 (0 étant le plus précis)
 * -----------------------------------------------------------*/
int MainWindow::get_edge_with_min_angle_between_neighb_face(MyMesh *_mesh, float limite)
{
    //qDebug() << "\t<" << __FUNCTION__ << ">";

    int edgeID = -1;
    float minAngle = 361.f;
    float angle;

    // parcours des arêtes
    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        EdgeHandle eh = *curEdge;

        MyMesh::HalfedgeHandle heh0 = _mesh->halfedge_handle(eh, 0);
        if (!_mesh->is_collapse_ok(heh0)) continue;

        if (!_mesh->is_boundary(eh))
        {
            MyMesh::HalfedgeHandle heh1 = _mesh->halfedge_handle(eh, 1);
            VertexHandle vh0 = _mesh->to_vertex_handle(heh0);
            VertexHandle vh1 = _mesh->to_vertex_handle(heh1);
            FaceHandle fh0 = _mesh->face_handle(heh0);
            FaceHandle fh1 = _mesh->face_handle(heh1);

            if (fh0.idx() < 0 || fh1.idx() < 0) {
                continue;
            }

            angle = fabs(angleFF(_mesh,fh0.idx(), fh1.idx(), vh0.idx(), vh1.idx()));
            //qDebug() << "\t\tangle = " << angle;

            if (angle < minAngle) {
                minAngle = angle;
                edgeID = eh.idx();
            }
            // TEST TMP
            if (angle <= limite) break;
        }
        //qDebug() << "\t\t<" << "edgeID =" << edgeID;
        //qDebug() << "\t\t<" << "angle =" << angle;
    }

    //qDebug() << "\t</" << __FUNCTION__ << ">";
    return edgeID;
}

void MainWindow::decimationAngle(MyMesh *_mesh, int percent)
{
    qDebug() << "<" << __FUNCTION__ << ">";

    int nbAretesSupr = _mesh->n_edges() * (float(percent)/100.f);

    float seuil = param_seuil(_mesh, PARAM_ANGLE, _precision);

    _progress = new QProgressDialog("simplification...", "Annuler", 0, nbAretesSupr, this);
    _progress->setWindowModality(Qt::WindowModal);
    int base_nb_edges = _mesh->n_edges();
    int iter_progress=0;

    for (int i=0; i<nbAretesSupr && _mesh->n_edges()>nbAretesSupr; i++)
    {
        if (_progress->wasCanceled())
            break;
        int edgeSuppr = get_edge_with_min_angle_between_neighb_face(_mesh, seuil);
        //qDebug() << "\tedgeSuppr = " << edgeSuppr;
        if (edgeSuppr<0) {
            _progress->cancel();
            break;
        }
        collapseEdge(_mesh, edgeSuppr);
        //if (i!=0 && i%100==0) qDebug() << "i" << i;
        //qDebug() << "i" << i;
        iter_progress = base_nb_edges - _mesh->n_edges();
        _progress->setValue(iter_progress);
    }
    _progress->setValue(nbAretesSupr);   
    _riAngle = maj_RI(_mesh->n_edges(), 100-(_precision*100), _nbReduct);

}


float MainWindow::calc_moyenne_angle_point(MyMesh *_mesh, int vertID)
{
    //qDebug() << "\t\t<" << __FUNCTION__ << ">";

    VertexHandle vh0 = _mesh->vertex_handle(vertID);
    int nbEdges = 0;
    float angleMoyen=0.f;
    for (MyMesh::VertexEdgeCWIter ve_it = _mesh->ve_cwiter(vh0); ve_it.is_valid(); ve_it++)
    {
        EdgeHandle eh = *ve_it;

        MyMesh::HalfedgeHandle hehc1 = _mesh->halfedge_handle(eh, 0);
        MyMesh::HalfedgeHandle hehc2 = _mesh->halfedge_handle(eh, 1);
        VertexHandle vhhe1 = _mesh->to_vertex_handle(hehc1);
        VertexHandle vhhe2 = _mesh->to_vertex_handle(hehc2);
        VertexHandle vh1;
        if (vhhe1 != vh0)
            vh1 = vhhe1;
        else
            vh1 = vhhe2;

        FaceHandle fh0;
        FaceHandle fh1;
        if (vh1 == vhhe1) {
            fh0 = _mesh->face_handle(hehc1);
            fh1 = _mesh->face_handle(hehc2);
        }
        else {
            fh0 = _mesh->face_handle(hehc2);
            fh1 = _mesh->face_handle(hehc1);
        }
        float angle = fabs(angleFF(_mesh, fh0.idx(), fh1.idx(), vh0.idx(), vh1.idx()));
        //qDebug() << "\t\t\tangle = " << angle;
        angleMoyen += angle;
        nbEdges++;
    }

    //qDebug() << "\t\t</" << __FUNCTION__ << ">";
    return angleMoyen/float(nbEdges);
}


int MainWindow::min_edge_extremites_plan(MyMesh *_mesh, float limite)
{
    //qDebug() << "\t<" << __FUNCTION__ << ">";

    int edgeID = -1;
    float min = float(INT_MAX);

    // parcours des arêtes
    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        EdgeHandle eh = *curEdge;

        MyMesh::HalfedgeHandle heh0 = _mesh->halfedge_handle(eh, 0);
        if (!_mesh->is_collapse_ok(heh0)) {
            //qDebug() << "\t\tedge no collapse";
            continue;
        }

        MyMesh::HalfedgeHandle heh1 = _mesh->halfedge_handle(eh, 1);
        VertexHandle vh0 = _mesh->to_vertex_handle(heh0);
        VertexHandle vh1 = _mesh->to_vertex_handle(heh1);
        //FaceHandle fh0 = _mesh->face_handle(heh0);
        //FaceHandle fh1 = _mesh->face_handle(heh1);

        // TMP
        /*
        resetAllColorsAndThickness(_mesh);
        _mesh->set_color(vh0, MyMesh::Color(100, 0, 0));
        _mesh->data(vh0).thickness = 12;
        _mesh->set_color(vh1, MyMesh::Color(100, 0, 0));
        _mesh->data(vh1).thickness = 12;
        displayMesh(_mesh);
        */
        //

        float angleMoyen0 = calc_moyenne_angle_point(_mesh, vh0.idx());
        //qDebug() << "\t\tangleMoyen0 = " << angleMoyen0;
        float angleMoyen1 = calc_moyenne_angle_point(_mesh, vh1.idx());
        //qDebug() << "\t\tangleMoyen1 = " << angleMoyen1;
        // Moyenne des 2 valeurs
        float currentValue = (angleMoyen0 + angleMoyen1) / 2.f;
        //qDebug() << "\t\tcurrentValue = " << currentValue;

        //qDebug() << "\t\tfabs(currentValue) =" << fabs(currentValue) << ">";

        if (fabs(currentValue) < min) {
            min = currentValue;
            edgeID = eh.idx();
        }
        // TMP
        if (min <= limite) break;
    }

    //qDebug() << "\t\tmin =" << min << ">";
    //qDebug() << "\t</" << __FUNCTION__ << ">";

    return edgeID;
}

void MainWindow::decimationPlaneite(MyMesh *_mesh, int percent)
{
    qDebug() << "<" << __FUNCTION__ << ">";

    int nbAretesSupr = _mesh->n_edges() * (float(percent)/100.f);

    float seuil = param_seuil(_mesh, PARAM_PLAN, _precision);

    _progress = new QProgressDialog("simplification...", "Annuler", 0, nbAretesSupr, this);
    _progress->setWindowModality(Qt::WindowModal);
    int base_nb_edges = _mesh->n_edges();
    int iter_progress=0;

    for (int i=0; i<nbAretesSupr && _mesh->n_edges()>nbAretesSupr; i++)
    {
        if (_progress->wasCanceled())
            break;
        int edgeSuppr = min_edge_extremites_plan(_mesh, _precision);
        //qDebug() << "\tedgeSuppr = " << edgeSuppr;
        if (edgeSuppr<0) {
            _progress->cancel();
            break;
        }
        collapseEdge(_mesh, edgeSuppr);
        //if (i!=0 && i%100==0) qDebug() << "i" << i;
        //qDebug() << "i" << i;
        iter_progress = base_nb_edges - _mesh->n_edges();
        _progress->setValue(iter_progress);
    }
    _progress->setValue(nbAretesSupr);
    _riPlan = maj_RI(_mesh->n_edges(), 100-(_precision*100), _nbReduct);

}

void MainWindow::decimation(MyMesh* _mesh, int percent, QString method)
{
    /* **** à compléter ! (Partie 2 et 3) ****
     * Cette fonction supprime des arêtes jusqu'à atteindre un pourcentage d'arêtes restantes, selon un critère donné
     * percent : pourcentage de l'objet à garder
     * method  : la méthode à utiliser parmis : "Aléatoire", "Par taille", "Par angle", "Par planéité"
     */

    qDebug() << "<" << __FUNCTION__ << ">";

    qDebug() << "\tnbEdgesBase =" << _mesh->n_edges();
    qDebug() << "\tpercent =" << percent;

    if(method == "Aléatoire")
    {
        decimationAleatoire(_mesh, percent);
    }
    else if(method == "Par taille")
    {
        decimationTaille(_mesh, percent);
        //decimationTailleQS(_mesh, percent);
    }
    else if(method == "Par angle")
    {
        decimationAngle(_mesh, percent);
    }
    else if(method == "Par planéité")
    {
        decimationPlaneite(_mesh, percent);
    }
    else
    {
        qDebug() << "\tMéthode inconnue !!!";
    }

    qDebug() << "\tnouveau nb d'aretes =" << _mesh->n_edges();
    updateEdgeSelectionIHM();

    qDebug() << "</" << __FUNCTION__ << ">";
}

/* **** début de la partie boutons et IHM **** */
void MainWindow::updateEdgeSelectionIHM()
{
    /* **** à compléter ! (Partie 3) ****
     * Cette fonction met à jour l'interface, les critères pourrons être affichés dans la zone de texte pour les vérifier
     */

    QString infos = "";
    infos = infos + "Surface : " + QString::number(0) + "\n";
    infos = infos + "\nC1 \nnbEdges=" + QString::number(_riTaille.nbEdges) +
            "\t\tprecision " + QString::number(_riTaille.precision)+
            "%\t\tnb réductions=" + QString::number(_riTaille.nbReduct)
            + "\n";
    infos = infos + "\nC2 \nnbEdges " + QString::number(_riAngle.nbEdges) +
            "\t\tprecision " + QString::number(_riAngle.precision)+
            "%\t\tnb réductions " + QString::number(_riAngle.nbReduct)
            + "\n";
    infos = infos + "\nC3 \nnbEdges " + QString::number(_riPlan.nbEdges) +
            "\t\tprecision " + QString::number(_riPlan.precision)+
            "%\t\tnb réductions " + QString::number(_riPlan.nbReduct)
            + "\n";
    ui->infoEdgeSelection->setPlainText(infos);

    ui->labelEdge->setText(QString::number(edgeSelection));

    // on montre la nouvelle sélection
    showEdgeSelection(&mesh);
}
/* **** fin de la partie à compléter **** */

void MainWindow::on_spinBox_precision(int value)
{
    _precision = value/100.f;
    _precision = 1.f - _precision;
}

void MainWindow::on_pushButton_edgeMoins_clicked()
{
    // mise à jour de l'interface
    edgeSelection = edgeSelection - 1;
    updateEdgeSelectionIHM();
}

void MainWindow::on_pushButton_edgePlus_clicked()
{
    // mise à jour de l'interface
    edgeSelection = edgeSelection + 1;
    updateEdgeSelectionIHM();
}

void MainWindow::on_pushButton_delSelEdge_clicked()
{
    // on supprime l'arête d'indice edgeSelection
    collapseEdge(&mesh, edgeSelection);

    // on actualise la sélection
    showEdgeSelection(&mesh);
}

void MainWindow::on_pushButton_chargement_clicked()
{
    // fenêtre de sélection des fichiers
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "", tr("Mesh Files (*.obj)"));

    // chargement du fichier .obj dans la variable globale "mesh"
    OpenMesh::IO::read_mesh(mesh, fileName.toUtf8().constData());

    mesh.update_normals();

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&mesh);

    // on affiche le maillage
    displayMesh(&mesh);
    _nbReduct = 0;
}

void MainWindow::on_pushButton_decimate_clicked()
{
    _nbReduct++;
    decimation(&mesh, ui->horizontalSlider->value(), ui->comboBox->currentText());
    displayMesh(&mesh);
}
/* **** fin de la partie boutons et IHM **** */



/* **** fonctions supplémentaires **** */
// permet d'initialiser les couleurs et les épaisseurs des élements du maillage
void MainWindow::resetAllColorsAndThickness(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        _mesh->data(*curVert).thickness = 1;
        _mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
    }

    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++)
    {
        _mesh->set_color(*curFace, MyMesh::Color(150, 150, 150));
    }

    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        _mesh->data(*curEdge).thickness = 1;
        _mesh->set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
}

// charge un objet MyMesh dans l'environnement OpenGL
void MainWindow::displayMesh(MyMesh* _mesh, bool isTemperatureMap, float mapRange)
{
    GLuint* triIndiceArray = new GLuint[_mesh->n_faces() * 3];
    GLfloat* triCols = new GLfloat[_mesh->n_faces() * 3 * 3];
    GLfloat* triVerts = new GLfloat[_mesh->n_faces() * 3 * 3];

    int i = 0;

    if(isTemperatureMap)
    {
        QVector<float> values;

        if(mapRange == -1)
        {
            for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
                values.append(fabs(_mesh->data(*curVert).value));
            qSort(values);
            mapRange = values.at(values.size()*0.8);
            qDebug() << "mapRange" << mapRange;
        }

        float range = mapRange;
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;

        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }
    else
    {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }


    ui->displayWidget->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);

    delete[] triIndiceArray;
    delete[] triCols;
    delete[] triVerts;

    GLuint* linesIndiceArray = new GLuint[_mesh->n_edges() * 2];
    GLfloat* linesCols = new GLfloat[_mesh->n_edges() * 2 * 3];
    GLfloat* linesVerts = new GLfloat[_mesh->n_edges() * 2 * 3];

    i = 0;
    QHash<float, QList<int> > edgesIDbyThickness;
    for (MyMesh::EdgeIter eit = _mesh->edges_begin(); eit != _mesh->edges_end(); ++eit)
    {
        float t = _mesh->data(*eit).thickness;
        if(t > 0)
        {
            if(!edgesIDbyThickness.contains(t))
                edgesIDbyThickness[t] = QList<int>();
            edgesIDbyThickness[t].append((*eit).idx());
        }
    }
    QHashIterator<float, QList<int> > it(edgesIDbyThickness);
    QList<QPair<float, int> > edgeSizes;
    while (it.hasNext())
    {
        it.next();

        for(int e = 0; e < it.value().size(); e++)
        {
            int eidx = it.value().at(e);

            MyMesh::VertexHandle vh1 = _mesh->to_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh1)[0];
            linesVerts[3*i+1] = _mesh->point(vh1)[1];
            linesVerts[3*i+2] = _mesh->point(vh1)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;

            MyMesh::VertexHandle vh2 = _mesh->from_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh2)[0];
            linesVerts[3*i+1] = _mesh->point(vh2)[1];
            linesVerts[3*i+2] = _mesh->point(vh2)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;
        }
        edgeSizes.append(qMakePair(it.key(), it.value().size()));
    }

    ui->displayWidget->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);

    delete[] linesIndiceArray;
    delete[] linesCols;
    delete[] linesVerts;

    GLuint* pointsIndiceArray = new GLuint[_mesh->n_vertices()];
    GLfloat* pointsCols = new GLfloat[_mesh->n_vertices() * 3];
    GLfloat* pointsVerts = new GLfloat[_mesh->n_vertices() * 3];

    i = 0;
    QHash<float, QList<int> > vertsIDbyThickness;
    for (MyMesh::VertexIter vit = _mesh->vertices_begin(); vit != _mesh->vertices_end(); ++vit)
    {
        float t = _mesh->data(*vit).thickness;
        if(t > 0)
        {
            if(!vertsIDbyThickness.contains(t))
                vertsIDbyThickness[t] = QList<int>();
            vertsIDbyThickness[t].append((*vit).idx());
        }
    }
    QHashIterator<float, QList<int> > vitt(vertsIDbyThickness);
    QList<QPair<float, int> > vertsSizes;

    while (vitt.hasNext())
    {
        vitt.next();

        for(int v = 0; v < vitt.value().size(); v++)
        {
            int vidx = vitt.value().at(v);

            pointsVerts[3*i+0] = _mesh->point(_mesh->vertex_handle(vidx))[0];
            pointsVerts[3*i+1] = _mesh->point(_mesh->vertex_handle(vidx))[1];
            pointsVerts[3*i+2] = _mesh->point(_mesh->vertex_handle(vidx))[2];
            pointsCols[3*i+0] = _mesh->color(_mesh->vertex_handle(vidx))[0];
            pointsCols[3*i+1] = _mesh->color(_mesh->vertex_handle(vidx))[1];
            pointsCols[3*i+2] = _mesh->color(_mesh->vertex_handle(vidx))[2];
            pointsIndiceArray[i] = i;
            i++;
        }
        vertsSizes.append(qMakePair(vitt.key(), vitt.value().size()));
    }

    ui->displayWidget->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);

    delete[] pointsIndiceArray;
    delete[] pointsCols;
    delete[] pointsVerts;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    vertexSelection = -1;
    edgeSelection = -1;
    faceSelection = -1;

    _riTaille = maj_RI(0, 0, 0);
    _riAngle = maj_RI(0, 0, 0);
    _riPlan = maj_RI(0, 0, 0);

    modevoisinage = false;

    ui->setupUi(this);

    connect(ui->spinBoxPrecision, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_precision(int)));
    ui->spinBoxPrecision->setValue(90);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _progress;
}

