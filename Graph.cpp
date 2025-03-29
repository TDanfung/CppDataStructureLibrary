#include "DataStructure.hpp"
#pragma once

//vertex
template<typename V, typename E> Vertex<V, E>::Vertex(V data) {
	this->data = data;
	this->edge = nullptr;
}

template<typename V, typename E> Vertex<V, E>::~Vertex() {
	this->edge = nullptr;
	this->data = 0;
}

template<typename V, typename E> void Vertex<V, E>::setEdge(Edge<V, E>* edge) {
	this->edge = edge;
}

template<typename V, typename E> V Vertex<V, E>::getData() {
	return this->data;
}

//edge
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

template<typename V, typename E> E Edge<V, E>::getData() {
	return this->data;
}

template<typename V, typename E> void Edge<V, E>::setData(E data) {
	this->data = data;
}

template<typename V, typename E> Vertex<V, E>* Edge<V, E>::getV1() {
	return this->v1;
}

template<typename V, typename E> Vertex<V, E>* Edge<V, E>::getV2() {
	return this->v2;
}


