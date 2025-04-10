#include <iostream>
#include <string>
#include "DenseGraph.cpp"
using namespace std;
#pragma once

#define MAX_CHILD_NUM 3

//vData 객체 -> Vertex<V,E>*의 data 필드로 들어감.
class vData {

public:
	int vertexNum;		//현재 정점의 생성 순서 -> edgeMap에서 인덱싱할 때 필요
	int childNum;		//자식 정점의 개수
	bool visited;		//배송 완료 지점인지 확인
	string productName;	//배송해야하는 물건의 이름
	double x;			//배송 좌표 x
	double y;			//배송 좌표 y
	double z;			//배송 좌표 z
	double totalLength;	//현재 정점까지 도달하기 위한 최소 길이

	vData(int vertexNum, double x, double y, double z, string name) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->productName = name;
		this->totalLength = -1.0;
		this->visited = false;
		this->vertexNum = vertexNum;
		this->childNum = 0;
	}

	~vData() {
		this->x = 0.0;
		this->y = 0.0;
		this->z = 0.0;
		this->totalLength = 0.0;
		this->visited = false;
		this->vertexNum = 0;
		int childNum = 0;
	}


};


//간선 가중치(거리)를 정점들의 좌표에 기반해 초기화하는 함수
//<Time Complexity>
//	O(V^2) -> V is the number of vertices
inline void setEdgeData(DenseGraph<vData*, double>* graph) {
	//edgeMap을 얻어온다.
	CLAL<CLAL<Edge<vData*, double>*>*>* edgeMap = graph->getEdgeMap();

	int y = edgeMap->getLastIndex();
	//edgeMap이 참조하는 EdgeList들을 하나하나 순회한다.
	for (int i = 0; i <= y ; i++) {

		CLAL<Edge<vData*, double>*>* edgeList = edgeMap->get(i);

		if (edgeList == nullptr) { //예외 감지
			continue;
		}

		int x = edgeList->getLastIndex();
		//각 edge들을 순회하여 edge의 가중치를 설정한다.
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
			double z = v1Data->z - v2Data->z;

			edge->setData(sqrt(x * x + y * y + z * z));


		}
	}
}

//큐에서 최소값을 찾아 뽑아내는 함수.
//<Time Complexity>
//	O(V) -> V is the number of vertices
inline Vertex<vData*, double>* dequeueMin(LinkedList<Vertex<vData*, double>*>* priorityQueue) {

	//없다면 널포인터 반환
	if (priorityQueue->isEmpty()) {
		return nullptr;
	}

	//하나만 있다면 dequeue 호출
	if (priorityQueue->getLength() == 1) {
		return priorityQueue->dequeue();
	}

	//반복을 위해 우선순위 큐의 헤드를 얻어온다.
	LinkedNode<Vertex<vData*, double>*>* node = priorityQueue->getHead();
	LinkedNode<Vertex<vData*, double>*>* minNode = node; //일단 최소 노드는 맨 첫 번째 노드로 설정한다.

	//초기값은 Head필드에 위치한 Vertex로 초기화한다.
	Vertex<vData*, double>* minVertex = node->getData();
	node = node->getNext(); //헤더를 min으로 지정했으므로, 그 다음 노드부터 체크
	while (node) {

		Vertex<vData*, double>* curVertex = node->getData();
		if (curVertex->getData()->visited) {
			node = node->getNext();
			continue;
		}
		vData* curData = curVertex->getData();
		vData* minData = minVertex->getData();
		//최소와 비교하여 최소가 더 큰 경우 minVertex를 curVertex로 변경
		if (curData->totalLength >= 0 && minData->totalLength >= 0 && curData->childNum < MAX_CHILD_NUM){
			if (minData->totalLength > curData->totalLength) { //최소한의 거리를 이동했다면
				minVertex = curVertex;
				minNode = node;
			}
		}
		//만약 minData가 inf라면
		else if(curData->totalLength >= 0 && minData->totalLength < 0){
			minVertex = curVertex;
			minNode = node;
		}

		node = node->getNext();
	}

	LinkedNode<Vertex<vData*, double>*>* prev = minNode->getPrev();
	LinkedNode<Vertex<vData*, double>*>* next = minNode->getNext();
	
	//앞의 노드만 널포인터면 앞부분만 지우는 메소드 사용
	if (prev == nullptr) {
		priorityQueue->dequeue();
	}
	//뒤의 노드만 널포인터면 뒷부분만 지우는 메소드 사용
	else if (next == nullptr) {
		priorityQueue->pop();
	}
	//둘 다 있다면 중간을 제거
	else {
		prev->setNext(next);
		next->setPrev(prev);
		delete minNode;
		priorityQueue->updateLength();
	}


	

	//최소 버텍스를 반환
	return minVertex;

}

//버텍스 초기화(리셋)
inline void resetVertex(LinkedList<Vertex<vData*, double>*>* priorityQueue, Vertex<vData*, double>* v) {
	vData* data = v->getData();
	data->totalLength = -1.0;
	data->visited = false;

	//유지하던 인접리스트 제거
	LinkedList<Edge<vData*, double>*>* eList = v->getEdgeList();
	while (!eList->isEmpty()) { 
		eList->pop();
	}

	v->setParent(nullptr);
	if (data->visited) {
		priorityQueue->enqueue(v);
	}
}

//트리를 깊이우선으로 탐색하여 우선순위 큐에 모두 넣는 함수
inline void dfsForInitializeChildNodes(LinkedList<Vertex<vData*, double>*>* priorityQueue, Vertex<vData*, double>* v) {

	LinkedList<Edge<vData*, double>*>* eList = v->getEdgeList();
	LinkedNode<Edge<vData*, double>*>* eNode = eList->getHead();

	while (eNode) {
		Edge<vData*, double>* e = eNode->getData();
		Vertex<vData*, double>* vOpp = e->getOpposite(v);
		dfsForInitializeChildNodes(priorityQueue, vOpp);
		eNode = eNode->getNext();
	}

	resetVertex(priorityQueue, v);

}

//최대 totalLength를 가진 간선 제거 -> 정점 당 자식 정점의 개수를 유지하기 위해 사용됨.
inline void removeMaxLengthEdge(LinkedList<Vertex<vData*, double>*>* priorityQueue, Vertex<vData*, double>* vertex) {
	LinkedNode<Edge<vData*, double>*>* eNode = nullptr;

	eNode = vertex->getEdgeList()->getHead();
	Edge<vData*, double>* maxLenEdge = eNode->getData();
	eNode = eNode->getNext();
	while (eNode) {
		Edge<vData*, double>* e = eNode->getData();
		if (e->getData() > maxLenEdge->getData()) {
			maxLenEdge = e;
		}
		eNode = eNode->getNext();
	}

	Vertex<vData*, double>* vOpp = maxLenEdge->getOpposite(vertex);

	//깊이우선탐색으로 노드들 제거
	dfsForInitializeChildNodes(priorityQueue, vOpp);
	vertex->removeEdge(maxLenEdge);
}

//다익스트라를 통해 최소신장트리를 구하는 함수. 한 정점 당 자식 노드는 세개로 한정한다.
inline void djikstra(DenseGraph<vData*, double>* graph) {

	//모든 edge의 값 초기화
	setEdgeData(graph);
	
	
	LinkedList<Vertex<vData*, double>*>* list = graph->getVertices();
	
	//list->getHead();를 통해 시작지점 vertex가져오기
	LinkedNode<Vertex<vData*, double>*>* start = list->getHead();
	
	//다익스트라 알고리즘을 위한 우선순위 큐 삽입
	//시작지점 삽입
	LinkedList<Vertex<vData*, double>*>* priorityQueue = new LinkedList<Vertex<vData*, double>*>();
	Vertex<vData*, double>* startVertex = start->getData();
	vData* startVertexData = startVertex->getData();
	startVertexData->totalLength = 0.0; //시작지점의 거리는 0.0으로 초기화

	//모든 버텍스를 우선순위 큐에 삽입
	LinkedNode<Vertex<vData*, double>*>* currentNode = list->getHead();
	while (currentNode) {
		priorityQueue->enqueue(currentNode->getData());
		currentNode = currentNode->getNext();
	}

	//djikstra 실행
	CLAL<CLAL<Edge<vData*, double>*>*>* edgeMap = graph->getEdgeMap();
	CLAL<Edge<vData*, double>*>* edgeList = nullptr;
	while (!priorityQueue->isEmpty()) {
		
		//큐에서 최소 버텍스 뽑아오기
		Vertex<vData*, double>* vertex = dequeueMin(priorityQueue);
		

		//totalLength가 최소가 되는 버텍스 얻어오고 visited = true;로 하기
		vData* vertexData = vertex->getData();
		vertexData->visited = true;
		int index = vertexData->vertexNum;
		edgeList = edgeMap->get(index);
		
		//간선을 돌면서 최소 간선 찾기 && 버텍스 거리 갱신
		int last = edgeList->getLastIndex();
		for (int i = 0; i <= last; i++) {
			
			Edge<vData*, double>* edge = edgeList->get(i);
			if (edge == nullptr) { //자기 자신에 대한 간선인 경우 넘어가기
				continue;
			}

			//반대편 버텍스 구해오기
			Vertex<vData*, double>* oppVertex = edge->getOpposite(vertex);			
			
			//간선의 길이 구하기
			double edgeLength = edge->getData();

			//반대편 정점의 데이터 가져오기
			vData* oppVertexData = oppVertex->getData();
			if (!oppVertexData->visited) {
				//초기화되지 않았다면
				if (oppVertexData->totalLength < 0){

					//만약 개수를 넘었다면, 최대 길이의 간선을 우선적으로 제거한다.
					//그 후 반대편 버텍스의 상태를 초기화한다.
					//이후 우선순위 큐에 넣는다.
					if (vertex->getEdgeNum() == MAX_CHILD_NUM) {
						removeMaxLengthEdge(priorityQueue, vertex);
					}

					oppVertexData->totalLength = vertexData->totalLength + edgeLength;
					
					//parent 설정을 통해 최소신장트리 제작 가능.
					oppVertex->setParent(vertex);
					vertex->addEdge(edge); //인접리스트 구현
				}
				else if ((vertexData->totalLength + edgeLength) < (oppVertexData->totalLength)) { 
					
					//만약 개수를 넘었다면, 최대 길이의 간선을 우선적으로 제거한다.
					//그 후 반대편 버텍스의 상태를 초기화한다.
					//이후 우선순위 큐에 넣는다
					if (vertex->getEdgeNum() == MAX_CHILD_NUM) {
						removeMaxLengthEdge(priorityQueue, vertex);
					}

					oppVertexData->totalLength = vertexData->totalLength + edgeLength;

					//이전의 부모 노드로부터 독립
					Vertex<vData*, double>* parentVertex = oppVertex->getParent();
					parentVertex->removeEdge(oppVertex);

					//새로운 부모의 자식으로 변경
					oppVertex->setParent(vertex);
					vertex->addEdge(edge); //인접리스트 구현
				}
			}
			
		}
		//minVertex를 우선순위 큐의 맨 앞에 엔큐
	}

	delete priorityQueue;

	/*
	LinkedNode<Vertex<vData*, double>*>* curVnode = list->getHead()->getNext();
	while (curVnode) {
		Vertex<vData*, double>* currV = curVnode->getData();

		while (currV) {

			vData* data = currV->getData();

			cout << "( " << data->x << " , " << data->y << " , " << data->z << " )";

			currV = currV->getParent();
		}
		cout << endl;
		curVnode = curVnode->getNext();
	}
	*/


}

