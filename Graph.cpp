#include "DataStructure.hpp"
#pragma once

//Vertex<V,E> 
//DenseGraph<V,E> 클래스의 부속 객체
//Minimum Spanning Tree구현을 위해 parent field를 추가하였음.
//그 자체로 인접리스트로써 사용할 수 있도록 edgeList필드가 있으며, 이는 getEdgeList();함수로 반환 가능
//V는 Vertex의 자료형, E는 edge의 자료형을 의미.
//<Methodes>
//	addEdge(Edge<V,E>*);	: O(1)
//	setParent(Vertex<V,E>*); : O(1)
//	isVisited(); : O(1)
//	visit(); : O(1)
//	getParent(); : O(1)
//	getData(V); : O(1)
//	getEdgeNum(); : O(1)
//	removeEdge(Vertex<V,E>*); : O(E) -> E is the number of edge
//	removeEdge(Edge<V,E>*); : O(E) -> E is the number of edge
//<Warning>
//	클래스, 소멸자 자체에 데이터 소멸 기능은 없음. getData() 호출 후 delete해야 함.
template<typename V, typename E> Vertex<V, E>::Vertex(V data) {
	this->data = data;
	this->visited = false;
	this->edgeList = new LinkedList<Edge<V,E>*>();
	this->parent = nullptr;
}


template<typename V, typename E> Vertex<V, E>::~Vertex() {
	this->data = data;
	this->visited = false;

	while (!this->edgeList->isEmpty()) {
		this->edgeList->pop();
	}

	delete this->edgeList;
	this->edgeList = nullptr;
	
	this->parent = nullptr;
}

//버텍스에 인접리스트 형식으로 edge를 추가한다.
template<typename V, typename E> void Vertex<V, E>::addEdge(Edge<V, E>* edge) {
	this->edgeList->append(edge);
}


//버텍스의 부모를 설정한다.
//<warning>
//	최소신장트리를 구하기 위해 사용되는 함수이다. 그래프 알고리즘 외부에서의 사용은 예기치 못한 버그를 만들어낼 수 있다.
template<typename V, typename E> void Vertex<V, E>::setParent(Vertex<V, E>* vParent) {
	this->parent = vParent;
}

//정점의 방문 여부를 확인한다.
//<warning>
//	그래프 알고리즘을 구현할 때 사용한다.
template<typename V, typename E> bool Vertex<V, E>::isVisited() {
	return this->visited;
}

//정점의 방문 여부를 '방문'으로 설정한다.
//<warning>
//	그래프 알고리즘을 구현할 때 사용한다. 그래프 알고리즘 외부에서의 사용은 예기치 못한 버그를 만들어낼 수 있다.
template<typename V, typename E> void Vertex<V, E>::visit(){
	this->visited = true;
}

//버텍스의 부모를 얻어온다.
//<warning>
//	최소신장트리를 구하기 위해 사용되는 함수이다.
template<typename V, typename E> Vertex<V, E>* Vertex<V, E>::getParent() {
	return this->parent;
}

//인접리스트 방식의 정점에서 사용 가능하다.
//다른 정점들과 연결된 간선들이 저장된 LinkedList를 반환한다.
//<Warning>
//	Read Only로 사용하길 권장
//	edgeList를 수정하고자 한다면 addEdge(), removeEdge() 등의 메소드를 호출하여 수정할 것.
template<typename V, typename E> LinkedList<Edge<V, E>*>* Vertex<V, E>::getEdgeList() {
	return this->edgeList;
}

//정점의 데이터를 얻어온다.
template<typename V, typename E> V Vertex<V, E>::getData() {
	return this->data;
}

//정점과 연결된 간선의 개수를 얻어온다.
template<typename V, typename E> int Vertex<V, E>::getEdgeNum() {
	return this->edgeList->getLength();
}

//인접리스트 방식의 정점에서 사용 가능하다.
//반대편 정점이 v인 간선을 인접 리스트에서 제거한다.
//<Exception>
//	반대편 정점이 v인 간선이 없다면 nullptr을 반환
//<Warning>
//	간선을 제거 후 delete를 섣불리 호출하지 말 것. 이미 해당 Edge<V,E>객체는 Graph의 간선 관리 리스트에 저장되어 있을 것.
//	간선 제거 후 delete 호출은 댕글링 포인터 생성을 야기할 수 있음.
template<typename V, typename E> Edge<V, E>* Vertex<V, E>::removeEdge(Vertex<V, E>* v) {
	LinkedNode<Edge<V, E>*>* eNode = this->edgeList->getHead();
	
	Edge<V, E>* eToRemove = nullptr;
	while (eNode) {
		Edge<V, E>* e = eNode->getData();
		if (e->getOpposite(v) == this) {
			eToRemove = e;
			break;
		}
		eNode = eNode->getNext();
	}

	if (eToRemove == nullptr) {
		return nullptr;
	}

	return this->edgeList->remove(eToRemove);
}

//인접리스트 방식의 정점에서 사용 가능하다.
//해당 간선을 인접 리스트에서 제거한다.
//<Exception>
//	반대편 정점이 v인 간선이 없다면 nullptr을 반환
//<Warning>
//	간선을 제거 후 delete를 섣불리 호출하지 말 것. 이미 해당 Edge<V,E>객체는 Graph의 간선 관리 리스트에 저장되어 있을 것.
//	간선 제거 후 delete 호출은 댕글링 포인터 생성을 야기할 수 있음.
template<typename V, typename E> Edge<V, E>* Vertex<V, E>::removeEdge(Edge<V, E>* e) {
	return this->edgeList->remove(e);
}


//Edge<V,E>
//DenseGraph<V,E> 클래스의 부속 객체
//두 개의 Vertex 정보와 가중치(데이터)를 가지고 있음.
//<Methodes>
//	getData(); : O(1)
//	setData(E); : O(1)
//	getV1(); : O(1)
//	getV2(); : O(1)
//	getOpposite(Vertex<V,E>* v); : O(1)
//<Warning>
//	Edge 클래스, 소멸자 자체에 가중치(데이터) 소멸 기능은 없음. getData로 따로 빼서 delete해야 함.
template<typename V, typename E> Edge<V, E>::Edge(Vertex<V, E>* v1, Vertex<V, E>* v2) {
	this->v1 = v1;
	this->v2 = v2;
	this->data = 0;
}

template<typename V, typename E> Edge<V, E>::~Edge() {
	this->v1 = nullptr;
	this->v2 = nullptr;
	this->data = 0;
}

//간선의 가중치(데이터)를 얻어온다.
template<typename V, typename E> E Edge<V, E>::getData() {
	return this->data;
}

//간선의 가중치(데이터)를 설정한다.
//<Warning>
//	간선의 가중치 E가 포인터 타입의 경우, 메모리 누수의 가능성이 있음.
//	메모리 누수를 예방하기 위한 방법 중 하나는 다음과 같음 :
//		TestEdgeData* d = edge->getData();
//		edge->setData(new TestEdgeData());
//		delete d; // delete를 하여도 좋고, 다른 객체에 할당해도 좋다.
template<typename V, typename E> void Edge<V, E>::setData(E data) {
	this->data = data;
}

//간선의 첫 번째 정점을 가져온다.
//사용하지 않을 것을 권장.
//<warning>
//	두 정점 중 어떤 정점 값이 나올 지 모름. 만약 반대편 정점을 얻고자 한다면 이 메소드 대신
//	getOpposite(Vertex<V,E>* v)메소드를 이용해 얻어올 것.
template<typename V, typename E> Vertex<V, E>* Edge<V, E>::getV1() {
	return this->v1;
}

//간선의 두 번째 정점을 가져온다.
//사용하지 않을 것을 권장.
//<warning>
//	두 정점 중 어떤 정점 값이 나올 지 모름. 만약 반대편 정점을 얻고자 한다면 이 메소드 대신
//	getOpposite(Vertex<V,E>* v)메소드를 이용해 얻어올 것.
template<typename V, typename E> Vertex<V, E>* Edge<V, E>::getV2() {
	return this->v2;
}

//간선이 가진 두 정점 중 한 정점에 대한 반대편의 정점을 가져온다.
//<Exception>
//	두 정점 중 어느 정점에도 해당하지 않는 정점이 매개변수로 들어간 경우에는 nullptr를 반환한다.
template<typename V, typename E> Vertex<V, E>* Edge<V, E>::getOpposite(Vertex<V,E>* v) {
	if (this->v1 == v) {
		return this->v2;
	}
	else if (this->v2 == v) {
		return this->v1;
	}
	else {
		return nullptr;
	}
}



