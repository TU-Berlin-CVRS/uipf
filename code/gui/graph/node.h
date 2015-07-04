#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

namespace uipf{
namespace gui{


class Edge;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE


class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget,QString name);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE { return Type; }

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
    QString name_;
    QRectF boundingRect_;
};

}//gui
}//uipf

#endif // NODE_H
