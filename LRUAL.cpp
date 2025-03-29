#include "DataStructure.hpp"
#include "LinkedNode.cpp"
#pragma once

template <typename T> inline LinkedNode<LRUBlock<T>*>* traverseToPrevNFind(LinkedNode<LRUBlock<T>*>* current, int index) {
	LinkedNode<LRUBlock<T>*>* currNode = current;
	LRUBlock<T>* currBlock;
	int startIndex, endIndex;

	//역방향 순회
	while (currNode) {
		currBlock = currNode->getData();
		startIndex = currBlock->startIndex;
		endIndex = currBlock->startIndex + currBlock->size - 1;
		if (startIndex <= index && index <= endIndex) {
			return currNode;
		}
		currNode = currNode->getPrev();
	}
}

template <typename T> inline LinkedNode<LRUBlock<T>*>* traverseToNextNFind(LinkedNode<LRUBlock<T>*>* current, int index) {
	LinkedNode<LRUBlock<T>*>* currNode = current;
	LRUBlock<T>* currBlock;
	int startIndex, endIndex;

	//역방향 순회
	while (currNode) {
		currBlock = currNode->getData();
		startIndex = currBlock->startIndex;
		endIndex = currBlock->startIndex + currBlock->size - 1;
		if (startIndex <= index && index <= endIndex) {
			return currNode;
		}
		currNode = currNode->getNext();
	}
	return nullptr;
}

template <typename T> LRUBlock<T>::LRUBlock(int size, int startIndex) {
	this->list = new T[size];
	this->startIndex = startIndex;
	this->size = size;
}

template <typename T> LRUBlock<T>::~LRUBlock() {
	this->size = 0;
	this->startIndex = 0;
	T* list = this->list;
	delete list;
}

template <typename T> void	LRUBlock<T>::setLRUBlockElem(int index, T data) {
	this->list[index - this->startIndex] = data;
}

template <typename T> T		LRUBlock<T>::getLRUBlockElem(int index) {
	return this->list[index - this->startIndex];
}



//Constructor
template <typename T> LRUAL<T>::LRUAL(int initCapacity) {
	this->head = new LinkedNode<LRUBlock<T>*>(new LRUBlock<T>(initCapacity, 0));
	this->tail = this->head;
	this->lruRefered = nullptr;
	this->index = -1;
}

template <typename T> LRUAL<T>::~LRUAL() {

	LinkedNode<LRUBlock<T>*>* currentNode = this->head;

	while (currentNode) {
		LinkedNode<LRUBlock<T>*>* nextNode = currentNode->getNext();
		LRUBlock<T>* dataToRemove = currentNode->getData();
		delete dataToRemove;
		delete currentNode;
		currentNode = nextNode;
	}
	this->head = nullptr;
	this->tail = nullptr;
	this->lruRefered = nullptr;
	this->index = 0;
}


//getter
template <typename T> T		LRUAL<T>::get(int index) {

	if (index < 0 || this->index < index) {
		//errmsg
		return 0;
	}

	if (this->lruRefered == nullptr) {
		this->lruRefered = this->head;
	}

	LinkedNode<LRUBlock<T>*>* node = getLinkedNode(index);
	LRUBlock<T>* block = node->getData();
	return block->getLRUBlockElem(index);

}

template <typename T> LinkedNode<LRUBlock<T>*>*	LRUAL<T>::getLinkedNode(int index) {

	if (index < 0 || this->index < index) {
		//errmsg
		return 0;
	}

	if (this->lruRefered == nullptr) {
		this->lruRefered = this->head;
	}

	LRUBlock<T>* block = this->lruRefered->getData();

	int currentStartIndex = block->startIndex;
	int currentEndIndex = block->startIndex + block->size - 1;

	if (currentStartIndex > index) { // -> to prev
		return this->lruRefered = traverseToPrevNFind(this->lruRefered, index);

	}
	else if (currentEndIndex < index) {// -> to next
		return this->lruRefered = traverseToNextNFind(this->lruRefered, index);
	}
	else { //아니면 그냥 받아오기
		return this->lruRefered;
	}
}

template <typename T> T		LRUAL<T>::remove(int index) {

	if (index < 0 || this->index < index) {
		//errmsg
		return 0;
	}

	if (this->lruRefered == nullptr) {
		this->lruRefered = this->head;
	}

	
	T toRemove = this->get(index);
	int i = index;
	for (i = index; i < this->index; i++) { // i는 0부터 this->index - 1까지 변함. 그리고 마지막에는 this->index가 됨.
		this->set(i, this->get(i + 1));
	}
	this->set(i, 0); //this->index 부분의 원소를 0으로 변경
	this->index--; //전체 인덱스 수 차감

	//만약 tail노드에 원소가 하나도 남아있지 않다면 제거.
	LRUBlock<T>* tailBlock = this->tail->getData();
	if (tailBlock->startIndex > i) {
		this->deleteTail();
	}

	return toRemove;
}

template <typename T> void LRUAL<T>::deleteTail() {

	if (this->tail == nullptr) {
		return;
	}

	LinkedNode<LRUBlock<T>*>* tail = this->tail;
	LinkedNode<LRUBlock<T>*>* newTail = tail->getPrev();

	if (newTail) {
		newTail->setNext(nullptr);
	}
	this->tail = newTail;

	LRUBlock<T>* data = tail->getData();
	delete data;
	delete tail;
}

//setter
template <typename T> void	LRUAL<T>::set(int index, T data) {

	if (index < 0 || this->index < index) {
		//errmsg
		return;
	}

	if (this->lruRefered == nullptr) {
		this->lruRefered = this->head;
	}

	LinkedNode<LRUBlock<T>*>* node = getLinkedNode(index);
	LRUBlock<T>* block = node->getData();
	block->setLRUBlockElem(index, data);
}

template <typename T> void	LRUAL<T>::add(T data) {
	if (this->lruRefered == nullptr) {
		this->lruRefered = this->tail;
	}

	LRUBlock<T>* block = this->tail->getData();

	if (this->index >= block->startIndex + block->size - 1) {

		int newSize = block->size * 2;
		int newIndex = block->startIndex + block->size;

		LinkedNode<LRUBlock<T>*>* newTail = new LinkedNode<LRUBlock<T>*>(new LRUBlock<T>(newSize, newIndex));
		LinkedNode<LRUBlock<T>*>* currTail = this->tail;

		currTail->setNext(newTail);
		newTail->setPrev(currTail);
		
		this->lruRefered = newTail;
		this->tail = newTail;

		block = newTail->getData();
	}

	block->setLRUBlockElem(++this->index, data);
}

