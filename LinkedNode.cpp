#include "DataStructure.hpp"
#pragma once

template <typename T> LinkedNode<T>::LinkedNode(T data) {
	this->next = nullptr;
	this->prev = nullptr;
	this->data = data;
}

template <typename T> LinkedNode<T>::~LinkedNode() {
	this->next = nullptr;
	this->prev = nullptr;
	this->data = 0;
}

template <typename T> LinkedNode<T>* LinkedNode<T>::getPrev() {
	return this->prev;
}

template <typename T> LinkedNode<T>* LinkedNode<T>::getNext() {
	return this->next;
}

template <typename T> void LinkedNode<T>::setPrev(LinkedNode<T>* prev) {
	this->prev = prev;
}

template <typename T> void LinkedNode<T>::setNext(LinkedNode<T>* next) {
	this->next = next;
}

template <typename T> void LinkedNode<T>::setData(T data) {
	this->data = data;
}

template <typename T> T LinkedNode<T>::getData() {
	return this->data;
}