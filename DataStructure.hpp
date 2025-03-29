#pragma once


template <typename T>
class List {
private:
	T data;
public:
	void add(T data);
	void remove(T data);
};

template <typename T>
class LinkedNode {
private:
	LinkedNode<T>* next;
	LinkedNode<T>* prev;
	T data;

public:
	LinkedNode(T data);
	~LinkedNode();

	void setNext(LinkedNode<T>* next);
	void setPrev(LinkedNode<T>* next);
	LinkedNode<T>* getPrev();
	LinkedNode<T>* getNext();

	T getData();
	void setData(T data);

};

template <typename T>
class LinkedList : public List<T> {
private:
	LinkedNode<T>* head;
	LinkedNode<T>* tail;

public:
	LinkedList();
	~LinkedList();

	LinkedNode<T>* getHead();
	LinkedNode<T>* getTail();

	void add(T data);
	void append(T data);
	void prepend(T data);
	void queue(T data);
	
	T pop();
	T dequeue();
	T remove(T elem);

};

template <typename T>
class LRUBlock {
public:
	T* list;
	int startIndex;
	int size;
	LRUBlock(int size, int startIndex);
	~LRUBlock();
	void setLRUBlockElem(int index, T data);
	T getLRUBlockElem(int index);
};

template <typename T>
class LRUAL : public List<T>{

private:
	LinkedNode<LRUBlock<T>*>* head;
	LinkedNode<LRUBlock<T>*>* tail;
	LinkedNode<LRUBlock<T>*>* lruRefered;
	int index; // Last index

	void deleteTail();
	LinkedNode<LRUBlock<T>*>* getLinkedNode(int index);
	
public:
	LRUAL(int initCapacity);
	~LRUAL();
	void set(int index, T data);
	void add(T data);
	T remove(int index);
	T get(int index);
	//void show();

};

//전방선언
template <typename V, typename E> class Edge;
template <typename V, typename E> class Vertex;

template <typename V, typename E> //Vertex 클래스, 소멸자 자체에 데이터 소멸 기능은 없음. getData로 따로 빼서 delete해야 함.
class Vertex {
private:
	V data;
	Edge<V, E>* edge;

public:
	Vertex(V data);
	~Vertex();
	void setEdge(Edge<V, E>* edge);
	V getData();
};


template <typename V, typename E> //Edge 클래스, 소멸자 자체에 데이터 소멸 기능은 없음. getData로 따로 빼서 delete해야 함.
class Edge {
private :
	Vertex<V,E>* v1;
	Vertex<V,E>* v2;
	E data;

public:
	Edge(Vertex<V, E>* v1, Vertex<V, E>* v2);
	~Edge();
	void setData(E data);
	E getData();
	Vertex<V, E>* getV1();
	Vertex<V, E>* getV2();
};


template <typename V, typename E>
class DenseGraph {
private:
	LRUAL<LRUAL<Edge<V, E>*>*>* edgeMap;
	LinkedList<Vertex<V, E>*>* vertexList;
	LinkedList<Edge<V, E>*>* edgeList;
	int vertexNumber;

public:
	DenseGraph();
	~DenseGraph();
	void addVertex(V v1);
	void showEdgeMap();


};




