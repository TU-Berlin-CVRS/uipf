#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include "../../framework/StdIncl.hpp"
#include "../../framework/Logger.hpp"
#include <math.h>
#include <random>
#include <QKeyEvent>
#include <QSizePolicy>

#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/progress.hpp>
#include <boost/shared_ptr.hpp>


namespace uipf{
namespace gui{

using namespace boost;

typedef boost::rectangle_topology<> topology_type;
typedef topology_type::point_type point_type;

typedef adjacency_list<listS, vecS, undirectedS,
                       property<vertex_name_t, std::string> > Graph;

typedef graph_traits<Graph>::vertex_descriptor Vertex;

typedef std::map<std::string, Vertex> NameToVertex;

Vertex get_vertex(const std::string& name, Graph& g, NameToVertex& names)
{
  NameToVertex::iterator i = names.find(name);
  if (i == names.end())
    i = names.insert(std::make_pair(name, add_vertex(name, g))).first;
  return i->second;
}


GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent),currentScale_(0.7f)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-100, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(currentScale_,currentScale_);
    setMinimumSize(400, 400);

}

//takes a configuration and outputs a graph layout
void GraphWidget::renderConfig(uipf::Configuration& config)
{
	QGraphicsScene* scene = this->scene();
	scene->clear();
	using namespace std;
	using namespace uipf;
	map<string, ProcessingStep> chain = config.getProcessingChain();

	//create all nodes first
	map<string,Node*> nodes;
	for (auto it = chain.begin(); it!=chain.end(); ++it)
	{
		  Node *node = new Node(this,QString(it->first.c_str()),it->second);

		  scene->addItem(node);
		  nodes.insert(std::pair<string,Node*>(it->first,node));
	}

	Graph g;
	NameToVertex names;

	//connect them in second pass:
	for (auto it = chain.begin(); it!=chain.end(); ++it)
	{
		auto inputs = it->second.inputs;
		std::string thisname = it->first;
		for (auto inputIt = inputs.begin();inputIt!=inputs.end();++inputIt)
		{
			std::string othername = inputIt->second.first;
			if (nodes.count(othername) != 0) { // only create edge if referenced node exists
				auto edge =new Edge(nodes[it->first], nodes[othername]);
				scene->addItem(edge);
				//add to calculation graph
				add_edge(get_vertex(thisname, g, names), get_vertex(othername, g, names), g);
			}
		}
	}

	//calculate graph layout with boosts fruchterman_reingold algo
	double width = this->width();//scene->width();
	double height = this->height();//scene->height();

	typedef std::vector<point_type> PositionVec;
	PositionVec position_vec(num_vertices(g));
	typedef iterator_property_map<PositionVec::iterator,
			property_map<Graph, vertex_index_t>::type>
	PositionMap;
	PositionMap position(position_vec.begin(), get(vertex_index, g));

	boost::minstd_rand gen;
	topology_type topo(gen, -width/2, -height/2, width/2, height/2);
	random_graph_layout(g, position, topo);
	fruchterman_reingold_force_directed_layout	(g, position, topo);

	//apply calculated positions to nodes
	graph_traits<Graph>::vertex_iterator vi, vi_end;
	for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
		std::string strName = get(vertex_name, g, *vi) ;

		nodes[strName]->setPos(position[*vi][0],  position[*vi][1]);
	}


	//QGraphicsView::fitInView( scene->itemsBoundingRect(), Qt::KeepAspectRatio );

}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);

    currentScale_ = scaleFactor;
}

}//gui
}//uipf
