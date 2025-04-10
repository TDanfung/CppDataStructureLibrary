#include "LinkedNode.cpp"
#pragma once

//LinkedList<T>
//이중연결리스트로써, 헤드와 테일을 모두 유지한다.
//큐와 스택의 역할을 동시에 수행할 수 있도록 설계되었다.
//<Methodes>
//	삽입 : 
//		prepend() : O(1), Head 노드에 접근한다.
//		append(), enqueue() add(): O(1), Tail 노드에 접근한다.
//	삭제 :
//		dequeue() : O(1), Head 노드에 접근한다.
//		pop() : O(1), Tail 노드에 접근한다.
//		remove() : O(n)
//	갱신 :
//		updateLength() : O(n)
//<Warning>
//	소멸자는 반드시 포함된 LinkedNode<T>를 모두 소멸시킨 뒤 호출할 것. 메모리 누수 위험이 있음.
template <typename T> LinkedList<T>::LinkedList() {
	this->head = nullptr;
	this->tail = nullptr;
	this->length = 0;
}

template <typename T> LinkedList<T>::~LinkedList() {
	this->head = nullptr;
	this->tail = nullptr;
	this->length = 0;
}

//맨 앞 노드를 얻어온다. dequeue(), prepend(), add()의 대상이 된다.
//<Warning>
//	얻어온 객체는 LinkedList<T>*가 아닌 LinkedNode<T>*이며, 정말 필요한 경우가 아닌 이상
//	setPrev(), setNext()함수를 호출하여 그 값을 바꾸지 말 것. 메모리 누수 위험과 댕글링 포인터 생성 위험이 있음.
//	Read-Only 객체로써 사용할 것을 권장
//<Example>
//	LinkedNode<T>* node = list->getHead();
//	while(node){
//		...
//		node = node->getNext();
//	}
template <typename T> LinkedNode<T>* LinkedList<T>::getHead() {
	return this->head;
}

//맨 끝 노드를 얻어온다. pop(), append(), enqueue()의 대상이 된다.
//<Warning>
//	얻어온 객체는 LinkedList<T>*가 아닌 LinkedNode<T>*이며, 정말 필요한 경우가 아닌 이상
//	setPrev(), setNext()함수를 호출하여 그 값을 바꾸지 말 것. 메모리 누수 위험과 댕글링 포인터 생성 위험이 있음.
//	Read-Only 객체로써 사용할 것을 권장
//<Example>
//	LinkedNode<T>* node = list->getHead();
//	while(node){
//		...
//		node = node->getPrev();
//	}
template <typename T> LinkedNode<T>* LinkedList<T>::getTail() {
	return this->tail;
}

//setter

//댕글링 포인터 제거 함수
//Tail 필드와 Head 필드를 모두 nullptr으로 할당한다
//길이가 0인 경우에만 동작한다.
template <typename T> void LinkedList<T>::setHeadTailNull() {
	if (this->length == 0) {
		this->head = nullptr;
		this->tail = nullptr;
	}
}

//외부에서 이 LinkedList<T>가 소유한 LinkedNode<T>*를 삭제한 경우에
//리스트의 길이를 갱신할 때 사용
//<Time Complexity>
//	O(n)
template <typename T> void LinkedList<T>::updateLength() {
	
	int length = 0;

	LinkedNode<T>* currentNode = this->head;
	while (currentNode) {
		length++;
		currentNode = currentNode->getNext();
	}

	this->length = length;
}

//append

//맨 앞에 원소를 추가한다.
template <typename T> void LinkedList<T>::prepend(T data) {

	if (this->head == nullptr) {
		this->head = new LinkedNode<T>(data);
		this->tail = this->head;
		this->length++;
		return;
	}

	LinkedNode<T>* currentHead = this->head;
	LinkedNode<T>* newNode = new LinkedNode<T>(data);
	this->head = newNode;
	newNode->setNext(currentHead);
	currentHead->setPrev(newNode);

	this->length++;

}

//맨 끝에 원소를 추가한다.
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

	this->length++;
}

//append와 그 의미가 같다.
template <typename T> void inline LinkedList<T>::enqueue(T data) {
	this->append(data);
}

//append와 그 의미가 같다.
template <typename T> void inline LinkedList<T>::add(T data) {
	this->append(data);
}

//getter

//리스트에서 특정 원소를 제거한다.
//<Exception>
//	원소를 찾지 못한 경우, 0(nullptr)을 반환한다.
template <typename T> T LinkedList<T>::remove(T data) {

	LinkedNode<T>* n = this->head;
	LinkedNode<T>* toRemove = nullptr;
	while (n) {

		if (n->getData() == data) {
			toRemove = n;
			break;
		}

		n = n->getNext();
	}

	if (toRemove == nullptr) {
		return 0;
	}

	if (n->getNext() == nullptr) {
		return this->pop();
	}
	else if (n->getPrev() == nullptr) {
		return this->dequeue();
	}
	else {
		LinkedNode<T>* prev = toRemove->getPrev();
		LinkedNode<T>* next = toRemove->getNext();

		prev->setNext(next);
		next->setPrev(prev);

		T data = toRemove->getData();
		delete toRemove;

		this->length--;

		return data;

	}

}

//맨 앞에서 노드를 제거하여 원소를 반환한다.
//<Exception>
//	리스트의 길이가 0인 경우, 0(nullptr)을 반환한다.
//<Warning>
//	의도치 않게 리스트의 Head필드가 nullptr이 된 경우엔 길이가 0이라고 판단해 삭제를 진행하지 않는다.
template <typename T> T LinkedList<T>::dequeue() {

	if (this->head == nullptr) {
		return 0;
	}

	LinkedNode<T>* toDeq = this->head;
	LinkedNode<T>* newHead = toDeq->getNext();

	if (newHead) {
		newHead->setPrev(nullptr);
	}
	else {
		this->tail = nullptr;
	}
	this->head = newHead;
	
	T dequeuedData = toDeq->getData();
	delete toDeq;

	this->length--;

	return dequeuedData;

}

//맨 뒤에서 노드를 제거하여 원소를 반환한다.
//<Exception>
//	리스트의 길이가 0인 경우, 0(nullptr)을 반환한다.
//<Warning>
//	의도치 않게 리스트의 Tail필드가 nullptr이 된 경우엔 길이가 0이라고 판단해 삭제를 진행하지 않는다.
template <typename T> T LinkedList<T>::pop() {

	if (this->tail == nullptr) {
		return 0;
	}

	LinkedNode<T>* toPop = this->tail;
	LinkedNode<T>* newTail = toPop->getPrev();

	if (newTail) {
		newTail->setNext(nullptr);
	}
	else {
		this->head = nullptr;
	}
	this->tail = newTail;

	T popedData = toPop->getData();
	delete toPop;


	this->length--;


	return popedData;

}

//리스트가 비어 있는지 확인한다.
//<Warning>
//	의도치 않게 리스트의 Head필드가 nullptr이 된 경우엔 길이가 0이라고 판단해 true를 반환한다.
template <typename T> bool LinkedList<T>::isEmpty() {
	return this->head == nullptr;
}

//리스트의 원소 개수를 얻어온다.
template <typename T> int LinkedList<T>::getLength() {
	return this->length;
}