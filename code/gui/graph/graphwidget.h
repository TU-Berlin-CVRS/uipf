#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include "../../framework/Configuration.hpp"

namespace uipf{
namespace gui{

class Node;

//A Class that displays the uipf::Configuration as a directed graph.
//layout is generated automatically with fruchtermann_reingold algorithm.
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);

   void renderConfig(uipf::Configuration& config);

};

}//gui
}//uipf

#endif // GRAPHWIDGET_H
