#include "LinkedNode.cpp"
#pragma once

template <typename T> LinkedList<T>::LinkedList() {
	this->head = nullptr;
	this->tail = nullptr;
}

template <typename T> LinkedList<T>::~LinkedList() {
	this->head = nullptr;
	this->tail = nullptr;
}


template <typename T> LinkedNode<T>* LinkedList<T>::getHead() {
	return this->head;
}

template <typename T> LinkedNode<T>* LinkedList<T>::getTail() {
	return this->tail;
}

//setter
template <typename T> void LinkedList<T>::prepend(T data) {

	if (this->head == nullptr) {
		this->head = new LinkedNode<T>(data);
		this->tail = this->head;
		return;
	}

	LinkedNode<T>* currentHead = this->head;
	LinkedNode<T>* newNode = new LinkedNode<T>(data);
	this->head = newNode;
	newNode->setNext(currentHead);
	currentHead->setPrev(newNode);

}

template <typename T> void inline LinkedList<T>::enqueue(T data) {
	this->prepend(data);
}

template <typename T> void LinkedList<T>::append(T data) {
	
	if (this->head == nullptr) {
		this->prepend(data);
		return;
	}

	LinkedNode<T>* currentTail = this->tail;
	LinkedNode<T>* newTail = new LinkedNode<T>(data);
	this->tail = newTail;
	newTail->setPrev(currentTail);
	currentTail->setNext(newTail);
}

template <typename T> void LinkedList<T>::add(T data) {
	this->append(data);
}

//getter
template <typename T> T LinkedList<T>::dequeue() {

	if (this->head == nullptr) {
		return 0;
	}

	LinkedNode<T>* toPop = this->head;
	LinkedNode<T>* newHead = toPop->getNext();

	this->head = newHead;
	if (newHead) {
		newHead->setPrev(nullptr);
	}
	
	T popedData = toPop->getData();
	delete toPop;

	return popedData;

}

template <typename T> T LinkedList<T>::pop() {

	if (this->tail == nullptr) {
		return 0;
	}

	LinkedNode<T>* toDeque = this->tail;
	LinkedNode<T>* newTail = toDeque->getPrev();

	this->tail = newTail;
	if (newTail) {
		newTail->setNext(nullptr);
	}

	T dequeuedData = toDeque->getData();
	delete toDeque;

	return dequeuedData;

}

template <typename T> bool LinkedList<T>::isEmpty() {
	return this->head == nullptr;
}