#include <iostream>
#include "DataStructure.hpp"
#include "Graph.cpp"
#include "LRUAL.cpp"
#include "LinkedList.cpp"
#define INITIAL_CAPACITY 4
#pragma once



//densegraph
template<typename V, typename E> DenseGraph<V, E>::DenseGraph() {
	this->edgeMap = new LRUAL<LRUAL<Edge<V,E>*>*>(INITIAL_CAPACITY);
	this->vertexList = new LinkedList<Vertex<V, E>*>();
	this->edgeList = new LinkedList<Edge<V, E>*>();
	this->vertexNumber = 0;
}



template<typename V, typename E> void DenseGraph<V, E>::addVertex(V v1) {
	
	//매개변수로 들어온 값에 대한 새로운 버텍스 생성
	Vertex<V, E>* newV = new Vertex<V, E>(v1);

	LRUAL<Edge<V, E>*>* currEdgeRow = nullptr;
	LinkedNode<Vertex<V, E>*>* currVertexNode = this->vertexList->getHead();

	//버텍스가 하나도 없을 때, 사실상 초기화
	if (currVertexNode == nullptr) {

		this->vertexList->append(newV);

		LRUAL<Edge<V, E>*>* newEdgeRow = new LRUAL<Edge<V, E>*>(INITIAL_CAPACITY);
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
			edgeQueue->prepend(nullptr);
		}
		else {
			currEdgeRow->add(newEdge);
			edgeQueue->prepend(newEdge);
		}


		currVertexNode = currVertexNode->getNext();
	}

	//새로운 EdgeMap Row를 생성하여 새로운 버텍스에 대한 나머지 버텍스의 관계를 정의함.
	LRUAL<Edge<V, E>*>* newEdgeRow = new LRUAL<Edge<V, E>*>(INITIAL_CAPACITY);
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

template<typename V, typename E> void DenseGraph<V, E>::showEdgeMap() {
	for (int i = 0; i < this->vertexNumber; i++) {
		
		LRUAL<Edge<V, E>*>* edgeRow = this->edgeMap->get(i);

		for (int j = 0; j < this->vertexNumber; j++) {

			Edge<V, E>* edge = edgeRow->get(j);
			if (edge == nullptr) {
				std::cout << "( V , V )";
				continue;
			}


			Vertex<V, E>* v1 = edge->getV1();
			Vertex<V, E>* v2 = edge->getV2();

			std::cout << "( " << v1->getData() << " , " << v2->getData() << ")";
		}
		std::cout << "" << std::endl;
	}
}


