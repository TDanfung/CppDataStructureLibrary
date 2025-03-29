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
	
	//�Ű������� ���� ���� ���� ���ο� ���ؽ� ����
	Vertex<V, E>* newV = new Vertex<V, E>(v1);

	LRUAL<Edge<V, E>*>* currEdgeRow = nullptr;
	LinkedNode<Vertex<V, E>*>* currVertexNode = this->vertexList->getHead();

	//���ؽ��� �ϳ��� ���� ��, ��ǻ� �ʱ�ȭ
	if (currVertexNode == nullptr) {

		this->vertexList->append(newV);

		LRUAL<Edge<V, E>*>* newEdgeRow = new LRUAL<Edge<V, E>*>(INITIAL_CAPACITY);
		newEdgeRow->add(nullptr); //��ȯ�׷����̱⿡ nullptr
		this->edgeMap->add(newEdgeRow);
		this->vertexNumber++;

		return;
	}


	//���� �����Ǵ� edge���� ���� ť.
	LinkedList<Edge<V, E>*>* edgeQueue = new LinkedList<Edge<V, E>*>();
	
	//�� EdgeMap�� row���� ���ο� vertex newV�� ���� ���� edge�� �ϳ��� �Ҵ�
	//currVertexNode = this->vertexList->getHead();
	for (int i = 0; i < this->vertexNumber; i++) {

		currEdgeRow = this->edgeMap->get(i); //edgemap row������
	
		Vertex<V, E>* currV = currVertexNode->getData();
		Edge<V, E>* newEdge = new Edge<V, E>(currV, newV);
		
		if (currV == newV) {
			currEdgeRow->add(nullptr); //��ȯ�׷����� null�� ó��
			edgeQueue->prepend(nullptr);
		}
		else {
			currEdgeRow->add(newEdge);
			edgeQueue->prepend(newEdge);
		}


		currVertexNode = currVertexNode->getNext();
	}

	//���ο� EdgeMap Row�� �����Ͽ� ���ο� ���ؽ��� ���� ������ ���ؽ��� ���踦 ������.
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


