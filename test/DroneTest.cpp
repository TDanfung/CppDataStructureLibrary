#include <iostream>
#include "DenseGraph.cpp"




int main() {

	//LRUAL<int>* list = new LRUAL<int>(4);
	DenseGraph<int, int>* graph = new DenseGraph<int, int>();

	graph->addVertex(1);
	graph->addVertex(2);
	graph->addVertex(3);
	graph->addVertex(4);

	graph->showEdgeMap();

	return 0;
}