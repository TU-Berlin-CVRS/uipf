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

   void selectNodeByName(const QString name);

   void triggerNodeSelected(const uipf::gui::Node*);

   signals: //for QT to connect
   		void nodeSelected(const uipf::gui::Node*);


private:
   void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

   void scaleView(qreal scaleFactor);

   qreal currentScale_;
};

}//gui
}//uipf

#endif // GRAPHWIDGET_H
