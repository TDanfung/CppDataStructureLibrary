#include <iostream>
#include <math.h>
#include "DenseGraph.cpp"

using namespace std;

class DronePoint {

public:
	double x;
	double y;
	double z;
	LinkedList<string>* deliverObj;

};

class vData {

public:
	int vertexNum;
	int length;
	bool visited;

	double x;
	double y;
	double totalLength;

	vData(int vertexNum) {
		this->x = 0.0;
		this->y = 0.0;
		this->totalLength = -1.0;
		this->vertexNum = vertexNum;
	}


};


void inline setEdgeData(DenseGraph<vData*, double>* graph) {
	LRUAL<LRUAL<Edge<vData*, double>*>*>* edgeMap = graph->getEdgeMap();
	int y = edgeMap->getLastIndex();
	for (int i = 0; i <= y; i++) {

		LRUAL<Edge<vData*, double>*>* edgeList = edgeMap->get(i);

		if (edgeList == nullptr) { //���� ����
			continue;
		}


		int x = edgeList->getLastIndex();
		for (int j = 0; j <= x; j++) {

			Edge<vData*, double>* edge = edgeList->get(j);

			if (edge == nullptr) { //���� ����
				continue;
			}

			Vertex<vData*, double>* v1 = edge->getV1();
			Vertex<vData*, double>* v2 = edge->getV2();

			vData* v1Data = v1->getData();
			vData* v2Data = v2->getData();

			double x = v1Data->x - v2Data->x;
			double y = v1Data->y - v2Data->y;

			edge->setData(sqrt(x * x + y * y));


		}
	}
}


void djikstra(DenseGraph<vData*,double>* graph) {

	//��� edge�� �� �ʱ�ȭ
	setEdgeData(graph);
	
	
	LinkedList<Vertex<vData*, double>*>* list = graph->getVertices();
	
	//list->getHead();�� ���� �������� vertex��������
	LinkedNode<Vertex<vData*, double>*>* start = list->getHead();
	
	//���ͽ�Ʈ�� �˰����� ���� �켱���� ť ����
	//�������� ����
	LinkedList<Vertex<vData*, double>*>* priorityQueue = new LinkedList<Vertex<vData*, double>*>();
	Vertex<vData*, double>* startVertex = start->getData();
	startVertex->getData()->totalLength = 0.0; //���������� �Ÿ��� 0.0���� �ʱ�ȭ
	priorityQueue->enqueue(startVertex); //���������� Vertex enqueue

	//djikstra ����
	LRUAL<LRUAL<Edge<vData*, double>*>*>* edgeMap = graph->getEdgeMap();
	LRUAL<Edge<vData*, double>*>* edgeList = nullptr;
	while (!priorityQueue->isEmpty()) {
		
		//ť���� �ּ� ���ؽ� �̾ƿ���
		Vertex<vData*, double>* vertex = priorityQueue->dequeue();
		
		//�ּ� ���ؽ��� ���� ���� ����Ʈ ������
		edgeList = edgeMap->get(vertex->getData()->vertexNum);
		
		//������ ���鼭 �ּ� ���� ã�� && ���ؽ� �Ÿ� ����
		int last = edgeList->getLastIndex();
		Vertex<vData*, double>* minVertex = nullptr;
		vData* vertexData = vertex->getData();
		for (int i = 0; i < last; i++) {
			
			Edge<vData*, double>* edge = edgeList->get(i);
			if (edge == nullptr) { //�ڱ� �ڽſ� ���� ������ ��� �Ѿ��
				continue;
			}

			Vertex<vData*, double>* oppVertex = edge->getOpposite(vertex);			
			
			//������ ���� ���ϱ�
			double edgeLength = edge->getData();

			vData* minVertexData = minVertex->getData();
			vData* oppVertexData = oppVertex->getData();
			//vData* vertexData = vertex->getData();
			
			//�ʱ�ȭ�� �� �� ���¶�� �� ù ��° �ݴ��� Vertex�� minVertex�� ����
			//�Ǵ� minVertex�� totalLenght���� �� ���� oppVertexData->totalLength + edgeLength�� �߰��ߴٸ� minVertex�� oppVertex�� ���� 
			if (minVertex == nullptr || oppVertexData->totalLength + edgeLength < minVertexData->totalLength) {
				oppVertexData->totalLength = vertexData->totalLength + edgeLength;
				minVertex = oppVertex;
				priorityQueue->enqueue(minVertex);
			}
		}

		
		//minVertex�� �켱���� ť�� �� �տ� ��ť
	
	}



}





int main() {

	//LRUAL<int>* list = new LRUAL<int>(4);
	DenseGraph<vData*, double>* graph = new DenseGraph<vData*, double>();

	graph->addVertex(new vData(1));
	graph->addVertex(new vData(2));
	graph->addVertex(new vData(3));
	graph->addVertex(new vData(4));
	graph->addVertex(new vData(5));
	graph->addVertex(new vData(6));
	graph->addVertex(new vData(7));
	graph->addVertex(new vData(8));

	graph->showEdgeMap();

	return 0;
}