#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QList>
#include <QPen>
#include "../../framework/ProcessingStep.hpp"
#include "../framework/GUIEventDispatcher.hpp"

namespace uipf{
namespace gui{


class Edge;
class GraphWidget;
/*QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE*/


class Node : public QGraphicsTextItem
{
public:
    Node(GraphWidget *graphWidget,QString name, ProcessingStep processingStep);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE { return Type; }

    //QRectF boundingRect() const Q_DECL_OVERRIDE;
    //QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

    QString name_;

    ProcessingStep processingStep_;

    void select(uipf::gui::GraphViewSelectionType eType);
    void unselect();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;

private:
    void updateEdges();

    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;


    QRectF boundingRect_;
    QPen pen_;
    bool pinned_;
    QString simpleHtml_;
    QString complexHtml_;
    QString inputsHtml_;
    uipf::gui::GraphViewSelectionType eSelectionType_;

};

}//gui
}//uipf

#endif // NODE_H
