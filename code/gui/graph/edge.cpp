#include "edge.h"
#include "node.h"

#include <math.h>
#include <QDebug>
#include <QPainter>
#include <sstream>
#include <QGraphicsTextItem>

namespace uipf{
namespace gui{


static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(Node *sourceNode, Node *destNode)
    : arrowSize(10)
{
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
}

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

void Edge::adjust()
{
    if (!source || !dest)
        return;

    QPointF pStart = mapFromItem(source, 0, 0);
    QPointF pEnd = mapFromItem(dest, 0, 0);
    QRectF sourceBounds = source->boundingRect();
    QRectF destBounds = dest->boundingRect();

    //move points to middle of boundingboxes
    pStart.setX(pStart.x() + source->boundingRect().width()/2);
    pEnd.setX(pEnd.x() + dest->boundingRect().width()/2);
    pStart.setY(pStart.y() + source->boundingRect().height()/2);
    pEnd.setY(pEnd.y() + dest->boundingRect().height()/2);

    //adjust docking edges relative to point positions
    if (abs(pStart.y() - pEnd.y()) < destBounds.height()+30.f)//start and end are approximately at same height
    {
    	if (pStart.x() > pEnd.x()) //start is to the right of end
    	{
    		pStart.setX(pStart.x()-sourceBounds.width()/2);
    		pEnd.setX(pEnd.x()+destBounds.width()/2);
    	}
    	else //start is to the left of end
    	{
    		pStart.setX(pStart.x()+sourceBounds.width()/2);
    		pEnd.setX(pEnd.x()-destBounds.width()/2);
    	}
    }
    else if (pStart.y() > pEnd.y())//start is over end
    {
    	pStart.setY(pStart.y()-sourceBounds.height()/2);
    	pEnd.setY(pEnd.y()+destBounds.height()/2);
    }
    else //start is under end
    {
    	pStart.setY(pStart.y()+sourceBounds.height()/2);
    	pEnd.setY(pEnd.y()-destBounds.height()/2);
    }

    QLineF line(pStart, pEnd);
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
    	QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
    	sourcePoint = line.p1() + edgeOffset;
    	destPoint = line.p2() - edgeOffset;
    } else {
    	sourcePoint = destPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{

}

void Edge::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{

}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                  cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                  cos(angle + Pi - Pi / 3) * arrowSize);


    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);

}

}//gui
}//uipf

