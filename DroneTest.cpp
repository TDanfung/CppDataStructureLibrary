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

		if (edgeList == nullptr) { //예외 감지
			continue;
		}


		int x = edgeList->getLastIndex();
		for (int j = 0; j <= x; j++) {

			Edge<vData*, double>* edge = edgeList->get(j);

			if (edge == nullptr) { //예외 감지
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

	//모든 edge의 값 초기화
	setEdgeData(graph);
	
	
	LinkedList<Vertex<vData*, double>*>* list = graph->getVertices();
	
	//list->getHead();를 통해 시작지점 vertex가져오기
	LinkedNode<Vertex<vData*, double>*>* start = list->getHead();
	
	//다익스트라 알고리즘을 위한 우선순위 큐 삽입
	//시작지점 삽입
	LinkedList<Vertex<vData*, double>*>* priorityQueue = new LinkedList<Vertex<vData*, double>*>();
	Vertex<vData*, double>* startVertex = start->getData();
	startVertex->getData()->totalLength = 0.0; //시작지점의 거리는 0.0으로 초기화
	priorityQueue->enqueue(startVertex); //시작지점의 Vertex enqueue

	//djikstra 실행
	LRUAL<LRUAL<Edge<vData*, double>*>*>* edgeMap = graph->getEdgeMap();
	LRUAL<Edge<vData*, double>*>* edgeList = nullptr;
	while (!priorityQueue->isEmpty()) {
		
		//큐에서 최소 버텍스 뽑아오기
		Vertex<vData*, double>* vertex = priorityQueue->dequeue();
		
		//최소 버텍스에 대한 간선 리스트 얻어오기
		edgeList = edgeMap->get(vertex->getData()->vertexNum);
		
		//간선을 돌면서 최소 간선 찾기 && 버텍스 거리 갱신
		int last = edgeList->getLastIndex();
		Vertex<vData*, double>* minVertex = nullptr;
		vData* vertexData = vertex->getData();
		for (int i = 0; i < last; i++) {
			
			Edge<vData*, double>* edge = edgeList->get(i);
			if (edge == nullptr) { //자기 자신에 대한 간선인 경우 넘어가기
				continue;
			}

			Vertex<vData*, double>* oppVertex = edge->getOpposite(vertex);			
			
			//간선의 길이 구하기
			double edgeLength = edge->getData();

			vData* minVertexData = minVertex->getData();
			vData* oppVertexData = oppVertex->getData();
			//vData* vertexData = vertex->getData();
			
			//초기화가 안 된 상태라면 맨 첫 번째 반대편 Vertex를 minVertex로 설정
			//또는 minVertex의 totalLenght보다 더 작은 oppVertexData->totalLength + edgeLength를 발견했다면 minVertex를 oppVertex로 변경 
			if (minVertex == nullptr || oppVertexData->totalLength + edgeLength < minVertexData->totalLength) {
				oppVertexData->totalLength = vertexData->totalLength + edgeLength;
				minVertex = oppVertex;
				priorityQueue->enqueue(minVertex);
			}
		}

		
		//minVertex를 우선순위 큐의 맨 앞에 엔큐
	
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