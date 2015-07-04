
#include "edge.h"
#include "node.h"
#include "graphwidget.h"
#include "../../framework/Logger.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QTextItem>

namespace uipf{
namespace gui{


Node::Node(GraphWidget *graphWidget,QString name)
    : graph(graphWidget),name_(name)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    QFont font("times", 24);
    font.setBold(false);
    font.setPointSize(14);
    QFontMetrics fm(font);
    int pixelsWide = fm.width(name_)+3;
    int pixelsHigh = fm.height();
    boundingRect_ = QRectF (-15, -15, pixelsWide, pixelsHigh);
}


void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

QRectF Node::boundingRect() const
{
    return boundingRect_;
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 500, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    //painter->setBrush(Qt::darkGray);

    painter->setPen(QPen(Qt::black, 0));

    QFont font("times", 24);
    font.setBold(false);
    font.setPointSize(14);


    painter->drawRect(boundingRect_);
    painter->setFont(font);
    //painter->setPen(Qt::lightGray);
   // painter->drawText(boundingRect_.translated(2, 2), name_);
    painter->setPen(Qt::black);
    painter->drawText(boundingRect_.translated(2, 0), name_);

}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{

    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}



}//gui
}//uipf
