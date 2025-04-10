#include "DataStructure.hpp"
#include "LinkedNode.cpp"
#pragma once

//private
//현재 참조하고 있는 노드의 인덱스 범위와 매개변수로 전달된 index값과 비교하여 prev 노드로 이동하는 함수.
template <typename T> inline LinkedNode<CLALBlock<T>*>* CLAL<T>::traverseToPrevNFind(LinkedNode<CLALBlock<T>*>* current, int index) {
	LinkedNode<CLALBlock<T>*>* currNode = current;
	CLALBlock<T>* currBlock;
	int startIndex, endIndex;

	//역방향 traverse
	while (currNode) {
		currBlock = currNode->getData();
		startIndex = currBlock->startIndex;
		endIndex = currBlock->startIndex + currBlock->size - 1;
		if (startIndex <= index && index <= endIndex) {//그 사이에 있다면 현재 노드 반환
			return currNode;
		}
		currNode = currNode->getPrev();
	}
	return nullptr;
}

//private
//현재 참조하고 있는 노드의 인덱스 범위와 매개변수로 전달된 index값과 비교하여 next 노드로 이동하는 함수.
template <typename T> inline LinkedNode<CLALBlock<T>*>* CLAL<T>::traverseToNextNFind(LinkedNode<CLALBlock<T>*>* current, int index) {
	LinkedNode<CLALBlock<T>*>* currNode = current;
	CLALBlock<T>* currBlock;
	int startIndex, endIndex;

	//순방향 traverse
	while (currNode) {
		currBlock = currNode->getData();
		startIndex = currBlock->startIndex;
		endIndex = currBlock->startIndex + currBlock->size - 1;
		if (startIndex <= index && index <= endIndex) {//그 사이에 있다면 현재 노드 반환
			return currNode;
		}
		currNode = currNode->getNext();
	}
	return nullptr;
}

//CLALBlock<T>
//CLAL<T>함수의 부속 객체
//이 객체는 시작 인덱스를 따로 유지하며, 이 시작 인덱스는 전체를 기준으로 할당된다.
//ex)앞선 노드의 마지막 인덱스가 n이었다면, 이 객체의 시작 인덱스는 n+1이 된다.
//T객체에 대한 배열 포인터를 필드로 가지며
//다른 CLALBlock<T>와 LinkedNode객체를 통해 유기적으로 사용되고 할당된다.
//<Warning>
//	직접 수정하지는 말 것을 권유함.
//	이 클래스는 CLAL<T>의 메소드 호출에 의해 수정되는 것이 자연스러우며,
//	그 외의 용도로 사용하는 경우 예상치 못한 동작을 할 수 있음.
template <typename T> CLALBlock<T>::CLALBlock(int size, int startIndex) {
	this->list = new T[size];
	this->startIndex = startIndex;
	this->size = size;
}

template <typename T> CLALBlock<T>::~CLALBlock() {
	this->size = 0;
	this->startIndex = 0;
	T* list = this->list;
	delete list;
}

//현재 매개변수로 입력된 인덱스와 자신의 배열의 시작 인덱스를 통한 상대 인덱스로 (index - this->startIndex)
//자신의 배열의 원소를 수정.
template <typename T> void	CLALBlock<T>::setCLALBlockElem(int index, T data) {
	this->list[index - this->startIndex] = data;
}

//현재 매개변수로 입력된 인덱스와 자신의 배열의 시작 인덱스를 통한 상대 인덱스로 (index - this->startIndex)
//자신의 배열의 원소를 얻어오기.
template <typename T> T		CLALBlock<T>::getCLALBlockElem(int index) {
	return this->list[index - this->startIndex];
}



//CLAL<T>
//
//가변 배열로써 구현되었음. LinkedList<T>와 구조가 유사하며, 배열의 크기가 변할 때, 복사 연산을 전혀 하지 않음.
//논리적 크기보다 물리적 크기가 작아지는 순간에 새 CLALBlock를 append하고, 그 CLALBlock의 배열의 크기는 앞선 노드의 두배임.
//<Methodes>
//	삽입 add() : O(1)
//	삭제 remove() : O(n) (사실상 O(log(n)) + n * O(1))
//	탐색 get() : O(log(n)), cachedRefered 필드가 정확히 찾았다면 O(1)
//	해제 delete : O(log(n))
//	해제(2차원 정방행렬) : O(log(n))*n + O(log(n)) = O(nlog(n))
template <typename T> CLAL<T>::CLAL(int initCapacity) {
	this->head = new LinkedNode<CLALBlock<T>*>(new CLALBlock<T>(initCapacity, 0));
	this->tail = this->head;
	this->cachedRefered = nullptr;
	this->index = -1;
}

template <typename T> CLAL<T>::~CLAL() {

	LinkedNode<CLALBlock<T>*>* currentNode = this->head;

	while (currentNode) {
		LinkedNode<CLALBlock<T>*>* nextNode = currentNode->getNext();
		CLALBlock<T>* dataToRemove = currentNode->getData();
		delete dataToRemove;
		delete currentNode;
		currentNode = nextNode;
	}
	this->head = nullptr;
	this->tail = nullptr;
	this->cachedRefered = nullptr;
	this->index = 0;
}


//getter

//private
//인덱스에 해당하는 LinkedNode<T>를 반환
//의도치 않은 값의 변경을 막기 위해 private 액세스를 부여함.
template <typename T> LinkedNode<CLALBlock<T>*>* CLAL<T>::getLinkedNode(int index) {

	if (index < 0 || this->index < index) {
		//errmsg
		return 0;
	}

	if (this->cachedRefered == nullptr) {
		this->cachedRefered = this->head;
	}

	CLALBlock<T>* block = this->cachedRefered->getData();

	int currentStartIndex = block->startIndex;
	int currentEndIndex = block->startIndex + block->size - 1;

	if (currentStartIndex > index) { // -> to prev
		return this->cachedRefered = traverseToPrevNFind(this->cachedRefered, index);

	}
	else if (currentEndIndex < index) {// -> to next
		return this->cachedRefered = traverseToNextNFind(this->cachedRefered, index);
	}
	else { //아니면 그냥 받아오기
		return this->cachedRefered;
	}
}

//반복문을 위한 함수. 현재까지 늘어난 길이에 해당하는 마지막 인덱스를 가져온다.
//<Warning>
//	for (int i = 0 ; i <= clal->getLastIndex() ; i ++ ){...} 와 같이 <= 를 이용해 계산하여야 한다.
//	그렇지 않으면 맨 마지막 원소가 참조되지 않을 수 있음.
template <typename T> int	CLAL<T>::getLastIndex() {
	return this->index;
}

//해당하는 인덱스의 원소를 얻어와 반환한다.
//<Exception>
//	인덱스 범위를 벗어날 경우, 0(nullptr)을 반환한다.
//<Time Complexity>
//	최악시간복잡도 : O(log(n))
//	캐시 적중 시 : O(1)
//<Warning>
//	T가 포인터인 경우, 포인터를 복사하여 반환하기 때문에, get을 이용해 얻어온 객체를
//	delete하지 않는 것을 권장. 이는 댕글링 포인터를 생성한다.
//	포인터 복사를 막고자 한다면 remove(int) 메소드를 이용할 것. (시간복잡도 : O(n) )
//	만약 delete를 해야 하는 경우 댕글링 포인터 생성을 피하기 위해 다음과 같이 이용할 것 :
//		T* data = clal->get(i);
//		delete data;
//		clal->set(i,nullptr);
template <typename T> T		CLAL<T>::get(int index) {

	if (index < 0 || this->index < index) {
		//errmsg
		return 0;
	}

	if (this->cachedRefered == nullptr) {
		this->cachedRefered = this->head;
	}

	this->cachedRefered = getLinkedNode(index);
	CLALBlock<T>* block = this->cachedRefered->getData();
	return block->getCLALBlockElem(index);

}

//해당하는 인덱스의 원소를 얻어오고, 제거한다.
//<Exception>
//인덱스 범위를 벗어날 경우, 0(nullptr)을 반환한다.
//<Time Complexity>
//최악시간복잡도 : O(n)
template <typename T> T		CLAL<T>::remove(int index) {

	if (index < 0 || this->index < index) {
		//errmsg
		return 0;
	}

	if (this->cachedRefered == nullptr) {
		this->cachedRefered = this->head;
	}


	T toRemove = this->get(index);
	int i = index;
	for (i = index; i < this->index; i++) { // i는 0부터 this->index - 1까지 변함. 그리고 마지막에는 this->index가 됨.
		this->set(i, this->get(i + 1));
	}
	this->set(i, 0); //this->index 부분의 원소를 0으로 변경
	this->index--; //전체 인덱스 수 차감

	//만약 tail노드에 원소가 하나도 남아있지 않다면 제거.
	CLALBlock<T>* tailBlock = this->tail->getData();
	if (tailBlock->startIndex > i) {
		this->deleteTail();
	}

	this->cachedRefered = traverseToPrevNFind(this->cachedRefered, index);

	return toRemove;
}

//remove(int)호출 후 맨 마지막 블록의 배열이 비어있다면 삭제
//의도치 않은 삭제를 막기 위해 private 액세스를 부여함
template <typename T> void CLAL<T>::deleteTail() {

	if (this->tail == nullptr) {
		return;
	}

	LinkedNode<CLALBlock<T>*>* tail = this->tail;
	LinkedNode<CLALBlock<T>*>* newTail = tail->getPrev();

	if (newTail) {
		newTail->setNext(nullptr);
	}
	this->tail = newTail;

	CLALBlock<T>* data = tail->getData();
	delete data;
	delete tail;
}

//setter

//해당하는 인덱스의 원소를 설정한다.
//<Exception>
//	인덱스 범위를 벗어날 경우, 아무 것도 수정하지 않고 함수를 즉시 종료한다.
//<Time Complexity>
//	최악시간복잡도 : O(log(n))
//	캐시 적중 시 : O(1)
//<Warning>
//	T가 포인터이고, data가 가리키는 객체가 이 객체에 의해서만 참조되고 있었다면
//	그 가리키는 객체는 가비지가 될 가능성이 있음.
//	가비지가 되지 않도록 하려면 다음의 방법을 이용할 것 :
//		TestObject* t = clal->get(i);
//		clal->set(i,new TestObject());
//		...
//		delete t;
//	  
template <typename T> void	CLAL<T>::set(int index, T data) {

	if (index < 0 || this->index < index) {
		//errmsg
		return;
	}

	if (this->cachedRefered == nullptr) {
		this->cachedRefered = this->head;
	}

	LinkedNode<CLALBlock<T>*>* node = getLinkedNode(index);
	CLALBlock<T>* block = node->getData();
	block->setCLALBlockElem(index, data);
}

//원소를 리스트의 맨 뒤에 삽입한다.
//논리적 크기가 넘어가면 마지막 노드의 두 배의 크기를 가진 배열을 가진 노드를 할당한다.
//<Time Complexity>
//	최악시간복잡도 : O(1)
template <typename T> void	CLAL<T>::add(T data) {
	if (this->cachedRefered == nullptr) {
		this->cachedRefered = this->tail;
	}

	CLALBlock<T>* block = this->tail->getData();

	if (this->index >= block->startIndex + block->size - 1) {

		int newSize = block->size * 2;
		int newIndex = block->startIndex + block->size;

		LinkedNode<CLALBlock<T>*>* newTail = new LinkedNode<CLALBlock<T>*>(new CLALBlock<T>(newSize, newIndex));
		LinkedNode<CLALBlock<T>*>* currTail = this->tail;

		currTail->setNext(newTail);
		newTail->setPrev(currTail);
		
		this->cachedRefered = newTail;
		this->tail = newTail;

		block = newTail->getData();
	}

	block->setCLALBlockElem(++this->index, data);
}

