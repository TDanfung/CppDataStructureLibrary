#include <iostream>
#include "DataStructure.hpp"
#include "Graph.cpp"
#include "CLAL.cpp"
#include "LinkedList.cpp"
#define INITIAL_CAPACITY 4
#pragma once



//DenseGraph<V,E>
//밀집그래프를 표현하기 위한 객체이다.
//V는 정점의 데이터의 자료형, E는 간선의 데이터의 자료형이다.
//<Methodes>
//	addVertex() : O(V) -> V is the number of vertices
//  removeEdgeMap() : O(V^2) -> V is the number of vertices
//	getEdgeMap() : O(1)
//	getVertices() : O(1)
//<Warning>
//	소멸자 호출 시 가비지 생성의 위험이 있으므로, 먼저 getVertices()와 getEdges()를 통해 LinkedNode를 모두 제거하고 호출을 권장.
template<typename V, typename E> DenseGraph<V, E>::DenseGraph() {
	this->edgeMap = new CLAL<CLAL<Edge<V,E>*>*>(INITIAL_CAPACITY);
	this->vertexList = new LinkedList<Vertex<V, E>*>();
	this->edgeList = new LinkedList<Edge<V, E>*>();
	this->vertexNumber = 0;
}

template<typename V, typename E> DenseGraph<V, E>::~DenseGraph() {
	delete this->edgeList;
	this->edgeList = nullptr;

	delete this->vertexList;
	this->vertexList = nullptr;
	
	delete this->edgeMap;
	this->edgeMap = nullptr;
}

//그래프에 정점을 추가하는 함수.
//그래프에 정점이 추가될 때마다, edge를 표현하는 공간이 2V + 1만큼 늘어나게 됨.
template<typename V, typename E> void DenseGraph<V, E>::addVertex(V v1) {
	
	//매개변수로 들어온 값에 대한 새로운 버텍스 생성
	Vertex<V, E>* newV = new Vertex<V, E>(v1);

	CLAL<Edge<V, E>*>* currEdgeRow = nullptr;
	LinkedNode<Vertex<V, E>*>* currVertexNode = this->vertexList->getHead();

	//버텍스가 하나도 없을 때, 사실상 초기화
	if (currVertexNode == nullptr) {

		this->vertexList->append(newV);

		CLAL<Edge<V, E>*>* newEdgeRow = new CLAL<Edge<V, E>*>(INITIAL_CAPACITY);
		newEdgeRow->add(nullptr); //순환그래프이기에 nullptr
		this->edgeMap->add(newEdgeRow);
		this->vertexNumber++;

		return;
	}


	//새로 생성되는 edge들을 담을 큐.
	LinkedList<Edge<V, E>*>* edgeQueue = new LinkedList<Edge<V, E>*>();
	
	//각 EdgeMap의 row마다 새로운 vertex newV에 대한 관계 edge를 하나씩 할당
	//currVertexNode = this->vertexList->getHead();
	for (int i = 0; i < this->vertexNumber; i++) {

		currEdgeRow = this->edgeMap->get(i); //edgemap row얻어오기
	
		Vertex<V, E>* currV = currVertexNode->getData();
		Edge<V, E>* newEdge = new Edge<V, E>(currV, newV);
		
		if (currV == newV) {
			currEdgeRow->add(nullptr); //순환그래프는 null로 처리
			edgeQueue->enqueue(nullptr);
		}
		else {
			currEdgeRow->add(newEdge);
			edgeQueue->enqueue(newEdge);
			this->edgeList->add(newEdge);
		}


		currVertexNode = currVertexNode->getNext();
	}

	//새로운 EdgeMap Row를 생성하여 새로운 버텍스에 대한 나머지 버텍스의 관계를 정의함.
	CLAL<Edge<V, E>*>* newEdgeRow = new CLAL<Edge<V, E>*>(INITIAL_CAPACITY);
	this->vertexList->append(newV);
	this->vertexNumber++;

	currVertexNode = this->vertexList->getHead();
	for (int i = 0; i < this->vertexNumber; i++) {
		
		Vertex<V, E>* currV = currVertexNode->getData();

		newEdgeRow->add(edgeQueue->dequeue());
		currVertexNode = currVertexNode->getNext();
	}
	
	delete edgeQueue;

	this->edgeMap->add(newEdgeRow);
}

//edgeMap 필드의 포인터를 반환하는 함수.
//<Warning>
//	객체를 직접 수정하는 것이 아닌 Read Only로 사용할 것을 권장.
//	edgeMap 객체를 직접 수정하는 것은 예상치 못한 버그를 야기할 수 있음.
template<typename V, typename E> CLAL<CLAL<Edge<V, E>*>*>* DenseGraph<V, E>::getEdgeMap() {
	return this->edgeMap;
}

//현재까지 생성된 모든 edge들을 담고 있는 LinkedList의 포인터를 반환한다.
//<Warning>
//	객체를 직접 수정하는 것이 아닌 Read Only로 사용할 것을 권장.
//	LinkedList<Edge<V,E>*> 객체를 직접 수정하는 것은 예상치 못한 버그를 야기할 수 있음.
template<typename V, typename E> LinkedList<Edge<V, E>*>* DenseGraph<V, E>::getEdges() {
	return this->edgeList;
}

//현재까지 추가된 모든 vertex들을 담고 있는 LinkedList의 포인터를 반환한다.
//<Warning>
//	객체를 직접 수정하는 것이 아닌 Read Only로 사용할 것을 권장.
//	LinkedList<Vertex<V,E>*> 객체를 직접 수정하는 것은 예상치 못한 버그를 야기할 수 있음.
template<typename V, typename E> LinkedList<Vertex<V, E>*>* DenseGraph<V, E>::getVertices() {
	return this->vertexList;
}

//edgeMap을 삭제하는 함수
//<Warning>
//	EdgeMap을 삭제 후 addVertex()를 즉시 호출하는 것은 권장하지 않음.
//	이는 edgeMap필드에 새로운 간선을 추가하게 되며, 예상치 못한 메모리 사용을 하게 될 수 있음.
template<typename V, typename E> void DenseGraph<V, E>::removeEdgeMap() {

	int edgeMapLastIndex = this->edgeMap->getLastIndex();
	for (int i = 0; i < edgeMapLastIndex; i++) {
		delete edgeMap->get(i);
		edgeMap->set(i, nullptr);
	}
	delete edgeMap;
	this->edgeMap = new CLAL<CLAL<Edge<V, E>*>*>(INITIAL_CAPACITY);

}