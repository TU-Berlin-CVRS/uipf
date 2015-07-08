
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
    : name_(name),processingStep_(processingStep),graph(graphWidget),pinned_(false),eSelectionType_(uipf::gui::NONE)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    pen_ = QPen(Qt::black, 0);

    std::stringstream style;
    style << "<style>"
    		<< "white-space: nowrap"
			<< "padding: 15px;"
    	 <<	"    table {"

        << "border-style: groove;"

		<<"}"
		<<"</style>";
    //build a simple view in html
    std::stringstream simple;
    simple << style.str() << "<table border='0'> "
    		<< "<tr>    <th>"<<processingStep_.name <<"</th>    </tr>  "
			<< "</table>";


    //build a complex view in html
    std::stringstream complex;
    complex << style.str() << "<table border='0'> "
    		<< "<tr>    <th width='100%' >"<<processingStep_.name <<" ["<< processingStep_.module <<"]</th>    </tr> ";

    		//<< "<h3> "<< processingStep_.name <<" </h3> <h4>["<< processingStep_.module <<"]</h4>";
    std::stringstream sParams;
    bool bHaveNonEmptyParams = false;
    if (processingStep_.params.size() > 0)
    {
    	sParams	<< "<hr/> <tr><td><table border='0'> "
    			<< "<tr>    <th width='30%' align='left'>Parameter</th>    <th width='70%' align='left'>Value</th>  </tr>  ";
    	for (auto param : processingStep_.params)
    	{
    		if (param.second.empty())continue;
    		sParams << "<tr>    <td>"<< param.first<<"</td> <td>"<< param.second<<"</td>   </tr>  ";
    		bHaveNonEmptyParams = true;
    	}
    	sParams << "</table></td>   </tr>";
    }
    if (bHaveNonEmptyParams)
    	complex << sParams.str();

    auto inputs = processingStep_.inputs;
    std::stringstream ss;
    if (inputs.size()>0)
    {
    	ss	<<  "<tr><td><table border='0'> "
    		<< "<tr><th width='30%' align='left'>From</th>    <th width='70%' align='left'>Output</th>  </tr>  ";

    	for (auto inputIt = inputs.begin();inputIt!=inputs.end();++inputIt)
    	{
    		if (inputIt->second.first.empty() || inputIt->second.second.empty())
    			continue;

    		ss << "<tr>    <td>"<< inputIt->second.first<<"</td> <td>"<< inputIt->second.second<<"</td>   </tr>  ";

    	}
    	ss << "</table></td>   </tr>";
    }
    inputsHtml_= QString(ss.str().c_str());

    complex << ss.str() << "</table>";
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

void Node::unselect()
{
	eSelectionType_ = uipf::gui::NONE;

	update();

	updateEdges();
}

void Node::select(uipf::gui::GraphViewSelectionType eType)
{
	eSelectionType_ = eType;

	update();

	updateEdges();
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	graph->triggerNodeSelected(this);
	select(uipf::gui::CURRENT);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	unselect();
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	graph->triggerNodeSelected(this);
	//always call parent. otherwise event is swallowed
	QGraphicsTextItem::mousePressEvent(event);
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

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* w)
{
	//draw a surrounding coloured rectangle
		QRectF boundingBox = this->boundingRect();
		//painter->drawRect(boundingBox);

		QPainterPath path;
		path.addRoundedRect(boundingBox, 10, 10);
		QPen pen(Qt::black, 2);
		painter->setPen(pen);

	switch (eSelectionType_)
	{
	case uipf::gui::CURRENT:
		//painter->setPen(Qt::blue);
		painter->fillPath(path, QColor(0, 0, 255, 127));//use alpha to get some transparency
		break;
	case uipf::gui::ERROR:
		//painter->setPen(Qt::red);
		painter->fillPath(path, QColor(255, 0, 0, 127));
		break;
	case uipf::gui::GOOD:
		//painter->setPen(Qt::green);
		painter->fillPath(path, QColor(0, 255, 0, 127));
		break;
	case uipf::gui::NONE:
		//painter->setPen(Qt::black);
		//painter.fillPath(path, Qt::blue);
		break;
	}

	//draw a surrounding coloured rectangle
		//QRectF boundingBox = this->boundingRect();
		//painter->drawRect(boundingBox);

	painter->drawPath(path);

	//call parent to draw its content
	QGraphicsTextItem::paint(painter,option,w);
}

void Node::updateEdges()
{
	//tell our edges we changed so they can arrange
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
