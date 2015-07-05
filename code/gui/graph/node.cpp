
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


Node::Node(GraphWidget *graphWidget,QString name,uipf::ProcessingStep processingStep)
    : graph(graphWidget),processingStep_(processingStep),name_(name),pinned_(false)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    pen_ = QPen(Qt::black, 0);

    //build a simple view in html
    std::stringstream simple;
    simple << "<table border='1'> "
    		<< "<tr>    <th>"<<processingStep_.name <<"</th>    </tr>  "
			<< "</table>";


    //build a complex view in html
    std::stringstream complex;
    complex << "<h2> "<< processingStep_.name <<" </h2> <h3>["<< processingStep_.module <<"]</h3>"
    		<< "<hr />";
    if (processingStep_.params.size() > 0)
    {
    	complex	<<  "<table border='1'> "
    			<< "<tr>    <th>Parameter</th>    <th>Value</th>  </tr>  ";
    	for (auto param : processingStep_.params)
    		complex << "<tr>    <td>"<< param.first<<"</td> <td>"<< param.second<<"</td>   </tr>  ";

    	complex << "</table>";
    }

    simpleHtml_ = QString(simple.str().c_str());
    complexHtml_ = QString(complex.str().c_str());


    setHtml(simpleHtml_);
    adjustSize();
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

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	//Maybe do something
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	//Maybe do something
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	if (pinned_)
		setHtml(simpleHtml_);
	else
		setHtml(complexHtml_);

	adjustSize();
	update();

	updateEdges();

	pinned_ = !pinned_;
}

//void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
//{
//	if (complexView_)
//	{
//		painter->setPen(pen_);
//
//		QFont font("times", 24);
//		font.setBold(false);
//		font.setPointSize(14);
//
//		QString qstrStepName("");
//
//		if (!processingStep_.name.empty())
//			qstrStepName = QString(processingStep_.name.c_str());
//
//
//		//painter->drawRect(boundingRect_);
//		painter->setFont(font);
//		//painter->setPen(Qt::lightGray);
//		// painter->drawText(boundingRect_.translated(2, 2), name_);
//		painter->setPen(Qt::black);
//		painter->drawText(boundingRect_.translated(2, 0),qstrStepName);
//
//		if (!processingStep_.module.empty())
//		{
//			QString qstrModName(processingStep_.module.c_str());
//
//			QRectF moduleRect = measureString(qstrModName);
//
//			painter->drawText(moduleRect.translated(0, -30),qstrModName);
//		}
//	}
//	else
//	{
//		painter->setPen(pen_);
//
//		QFont font("times", 24);
//		font.setBold(false);
//		font.setPointSize(14);
//
//
//		painter->drawRect(boundingRect_);
//		painter->setFont(font);
//		//painter->setPen(Qt::lightGray);
//		// painter->drawText(boundingRect_.translated(2, 2), name_);
//		painter->setPen(Qt::black);
//		painter->drawText(boundingRect_.translated(2, 0), name_);
//	}
//}

void Node::updateEdges()
{
	foreach (Edge *edge, edgeList)
			edge->adjust();

}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{

    switch (change) {
    case ItemPositionHasChanged:
    	updateEdges();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}



}//gui
}//uipf
