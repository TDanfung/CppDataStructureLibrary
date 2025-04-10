#include "DataStructure.hpp"
#pragma once

//LinkedNode<T>
//LinkedList<T>의 부속 객체, 이중연결리스트의 형태를 띄고 있다.
//<Warning>
//	함부로 이 객체에 대한 소멸자를 호출하지 말 것. 댕글링 포인터를 만들어낼 수 있음.
//	이 객체는 LinkedList<T>에 의해 다뤄지는 것이 자연스러움.
//	Read Only로 사용할 것을 권장.
template <typename T> LinkedNode<T>::LinkedNode(T data) {
	this->next = nullptr;
	this->prev = nullptr;
	this->data = data;
}

template <typename T> LinkedNode<T>::~LinkedNode() {
	this->next = nullptr;
	this->prev = nullptr;
}

//현재 노드의 이전 노드를 반환한다.
//<example>
//	LinkedNode<T>* n = list->getHead(); // LinkedList<T>객체
//	while(n){ . . .
//		n = n->getNext();}
template <typename T> LinkedNode<T>* LinkedNode<T>::getPrev() {
	return this->prev;
}

//현재 노드의 다음 노드를 반환한다.
//<example>
//	LinkedNode<T>* n = list->getHead(); // LinkedList<T>객체
//	while(n){ . . .
//		n = n->getPrev();}
template <typename T> LinkedNode<T>* LinkedNode<T>::getNext() {
	return this->next;
}

//현재 노드가 가리키는 다음 노드를 prev로 설정한다.
//<Warning>
//	prev 포인터가 이미 다른 곳을 가리키고 있는 경우, 가비지 생성의 위험이 있음.
//	또한 head노드인 경우, LinkedList의 삽입 삭제 메소드들에 영향을 줄 수 있음.
//	외부에서는 사용하지 않는 것을 권장.
template <typename T> void LinkedNode<T>::setPrev(LinkedNode<T>* prev) {
	this->prev = prev;
}

//현재 노드가 가리키는 다음 노드를 next로 설정한다.
//<Warning>
//	next 포인터가 이미 다른 곳을 가리키고 있는 경우, 가비지 생성의 위험이 있음.
//	또한 head노드인 경우, LinkedList의 삽입 삭제 메소드들에 영향을 줄 수 있음.
//	외부에서는 사용하지 않는 것을 권장.
template <typename T> void LinkedNode<T>::setNext(LinkedNode<T>* next) {
	this->next = next;
}

//현재 노드가 가지고 있는 데이터를 수정한다.
//<Warning>
//	T가 포인터 타입인 경우, 가비지 생성의 위험이 있음.
//	가비지 생성을 방지하기 위한 방법 중 하나는 다음과 같음 :
//		T* t = node->getData();
//		node->setData(new T());
//		delete t; // t를 삭제해도 좋고, 다른 객체에 삽입하여도 좋음.
template <typename T> void LinkedNode<T>::setData(T data) {
	this->data = data;
}

//현재 노드가 가지고 있는 데이터를 얻어온다.
template <typename T> T LinkedNode<T>::getData() {
	return this->data;
}