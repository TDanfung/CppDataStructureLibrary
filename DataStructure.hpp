#pragma once



//LinkedNode<T>
//LinkedList<T>의 부속 객체, 이중연결리스트의 형태를 띄고 있다.
//<Warning>
//	함부로 이 객체에 대한 소멸자를 호출하지 말 것. 댕글링 포인터를 만들어낼 수 있음.
//	이 객체는 LinkedList<T>에 의해 다뤄지는 것이 자연스러움.
//	Read Only로 사용할 것을 권장.
template <typename T> class LinkedNode {
private:
	LinkedNode<T>* next;
	LinkedNode<T>* prev;
	T data;

public:
	LinkedNode(T data);
	~LinkedNode();

	//현재 노드의 다음 노드를 반환한다.
	//<example>
	//	LinkedNode<T>* n = list->getHead(); // LinkedList<T>객체
	//	while(n){ . . .
	//		n = n->getPrev();}
	void setNext(LinkedNode<T>* next);

	//현재 노드의 이전 노드를 반환한다.
	//<example>
	//	LinkedNode<T>* n = list->getHead(); // LinkedList<T>객체
	//	while(n){ . . .
	//		n = n->getNext();}
	void setPrev(LinkedNode<T>* next);
	
	//현재 노드가 가리키는 다음 노드를 next로 설정한다.
	//<Warning>
	//	next 포인터가 이미 다른 곳을 가리키고 있는 경우, 가비지 생성의 위험이 있음.
	//	또한 head노드인 경우, LinkedList의 삽입 삭제 메소드들에 영향을 줄 수 있음.
	//	외부에서는 사용하지 않는 것을 권장.
	LinkedNode<T>* getNext();
	
	//현재 노드가 가리키는 다음 노드를 prev로 설정한다.
	//<Warning>
	//	prev 포인터가 이미 다른 곳을 가리키고 있는 경우, 가비지 생성의 위험이 있음.
	//	또한 head노드인 경우, LinkedList의 삽입 삭제 메소드들에 영향을 줄 수 있음.
	//	외부에서는 사용하지 않는 것을 권장.
	LinkedNode<T>* getPrev();

	//현재 노드가 가지고 있는 데이터를 얻어온다.
	T getData();

	//현재 노드가 가지고 있는 데이터를 수정한다.
	//<Warning>
	//	T가 포인터 타입인 경우, 가비지 생성의 위험이 있음.
	//	가비지 생성을 방지하기 위한 방법 중 하나는 다음과 같음 :
	//		T* t = node->getData();
	//		node->setData(new T());
	//		delete t; // t를 삭제해도 좋고, 다른 객체에 삽입하여도 좋음.
	void setData(T data);

};

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
template <typename T> class LinkedList{
private:
	LinkedNode<T>* head;
	LinkedNode<T>* tail;
	int length;

public:
	LinkedList();
	~LinkedList();

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
	LinkedNode<T>* getHead();

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
	LinkedNode<T>* getTail();

	//Tail 노드와 연계되어 사용되는 메소드들.

	//맨 끝에 원소를 추가한다.
	void append(T data);

	//append와 그 의미가 같다.
	void enqueue(T data);

	//맨 뒤에서 노드를 제거하여 원소를 반환한다.
	//<Exception>
	//	리스트의 길이가 0인 경우, 0(nullptr)을 반환한다.
	//<Warning>
	//	의도치 않게 리스트의 Tail필드가 nullptr이 된 경우엔 길이가 0이라고 판단해 삭제를 진행하지 않는다.
	T pop();
	
	//Head 노드와 연계되어 사용되는 메소드들

	//append와 그 의미가 같다.
	void add(T data);

	//맨 앞에 원소를 추가한다.
	void prepend(T data);

	//맨 앞에서 노드를 제거하여 원소를 반환한다.
	//<Exception>
	//	리스트의 길이가 0인 경우, 0(nullptr)을 반환한다.
	//<Warning>
	//	의도치 않게 리스트의 Head필드가 nullptr이 된 경우엔 길이가 0이라고 판단해 삭제를 진행하지 않는다.
	T dequeue();
	
	//댕글링 포인터 제거 함수
	//Tail 필드와 Head 필드를 모두 nullptr으로 할당한다
	//길이가 0인 경우에만 동작한다.
	void setHeadTailNull();

	//외부에서 링크드리스트를 수정할 일이 있을 때, 수정 후 호출할 것.
	//링크드리스트의 길이를 갱신한다. -> O(n)
	void updateLength();
	
	//리스트에서 특정 원소를 제거한다.
	//<Exception>
	//	원소를 찾지 못한 경우, 0(nullptr)을 반환한다.
	T remove(T elem);


	//링크드리스트의 길이를 가져온다.
	int getLength();

	//링크드리스트가 비어있는 지 확인한다.
	bool isEmpty();

};

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
template <typename T> class CLALBlock {

public:
	T* list;
	int startIndex;
	int size;
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
	CLALBlock(int size, int startIndex);
	~CLALBlock();
	//현재 매개변수로 입력된 인덱스와 자신의 배열의 시작 인덱스를 통한 상대 인덱스로 (index - this->startIndex)
	//자신의 배열의 원소를 수정.
	void setCLALBlockElem(int index, T data);
	//현재 매개변수로 입력된 인덱스와 자신의 배열의 시작 인덱스를 통한 상대 인덱스로 (index - this->startIndex)
	//자신의 배열의 원소를 얻어오기.
	T getCLALBlockElem(int index);
};



//CLAL<T>
//
//가변 배열로써 구현되었음. LinkedList<T>와 구조가 유사하며, 배열의 크기가 변할 때, 복사 연산을 전혀 하지 않음.
//논리적 크기보다 물리적 크기가 작아지는 순간에 새 CLALBlock를 append하고, 그 CLALBlock의 배열의 크기는 앞선 노드의 두배임.
//	삽입 add() : O(1)
//	삭제 remove() : O(n) (사실상 O(log(n)) + n * O(1))
//	탐색 get() : O(log(n)), cachedRefered 필드가 정확히 찾았다면 O(1)
//	해제 delete : O(log(n))
//	해제(2차원 정방행렬) : O(log(n))*n + O(log(n)) = O(nlog(n))
//
template <typename T> class CLAL{
private:
	LinkedNode<CLALBlock<T>*>* head;
	LinkedNode<CLALBlock<T>*>* tail;
	LinkedNode<CLALBlock<T>*>* cachedRefered;
	int index; // Last index

	//원소 삭제 후 맨 마지막 블록의 배열이 비어있다면 삭제
	//의도치 않은 삭제를 막기 위해 private 액세스를 부여함
	void deleteTail();

	//인덱스에 해당하는 LinkedNode<T>를 반환
	//의도치 않은 값의 변경을 막기 위해 private 액세스를 부여함.
	LinkedNode<CLALBlock<T>*>* getLinkedNode(int index);

	//private
	//현재 참조하고 있는 노드의 인덱스 범위와 매개변수로 전달된 index값과 비교하여 prev 노드로 이동하는 함수.
	LinkedNode<CLALBlock<T>*>* traverseToPrevNFind(LinkedNode<CLALBlock<T>*>* current, int index);

	//private
	//현재 참조하고 있는 노드의 인덱스 범위와 매개변수로 전달된 index값과 비교하여 next 노드로 이동하는 함수.
	LinkedNode<CLALBlock<T>*>* traverseToNextNFind(LinkedNode<CLALBlock<T>*>* current, int index);
	
public:
	CLAL(int initCapacity);
	~CLAL();

	//반복문을 위한 함수. 현재까지 늘어난 길이에 해당하는 마지막 인덱스를 가져온다.
	//<Warning>
	//	for (int i = 0 ; i <= clal->getLastIndex() ; i ++ ){...} 와 같이 <= 를 이용해 계산하여야 한다.
	//	그렇지 않으면 맨 마지막 원소가 참조되지 않을 수 있음.
	int getLastIndex();

	

	//data를 뒤에서 삽입
	void add(T data);

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
	T get(int index);

	//해당하는 인덱스의 원소를 얻어오고, 제거한다.
	//<Exception>
	//인덱스 범위를 벗어날 경우, 0(nullptr)을 반환한다.
	//<Time Complexity>
	//최악시간복잡도 : O(n)
	T remove(int index);

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
	void set(int index, T data);

	

	

	


};

//전방선언
template <typename V, typename E> class Edge;
template <typename V, typename E> class Vertex;

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
template <typename V, typename E> class Vertex {
private:
	bool visited;
	V data;
	Vertex<V, E>* parent; //Minimum Spanning Tree를 구성하기 위한 포인터.
	LinkedList<Edge<V, E>*>* edgeList;

public:
	Vertex(V data);
	~Vertex();
	
	//setter
	
	//버텍스에 인접리스트 형식으로 edge를 추가한다.
	void addEdge(Edge<V, E>* edge);

	//버텍스의 부모를 설정한다.
	//<warning>
	//	최소신장트리를 구하기 위해 사용되는 함수이다. 그래프 알고리즘 외부에서의 사용은 예기치 못한 버그를 만들어낼 수 있다.
	void setParent(Vertex<V, E>* vParent);

	//getter

	//버텍스의 부모를 얻어온다.
	//<warning>
	//	최소신장트리를 구하기 위해 사용되는 함수이다.
	Vertex<V, E>* getParent();

	//정점과 연결된 간선의 개수를 얻어온다.
	int getEdgeNum();

	//정점의 방문 여부를 확인한다.
	//<warning>
	//	그래프 알고리즘을 구현할 때 사용한다.
	bool isVisited();

	//정점의 방문 여부를 '방문'으로 설정한다.
	//<warning>
	//	그래프 알고리즘을 구현할 때 사용한다. 그래프 알고리즘 외부에서의 사용은 예기치 못한 버그를 만들어낼 수 있다.
	void visit();

	//인접리스트 방식의 정점에서 사용 가능하다.
	//반대편 정점이 v인 간선을 인접 리스트에서 제거한다.
	//<Exception>
	//	반대편 정점이 v인 간선이 없다면 nullptr을 반환
	//<Warning>
	//	간선을 제거 후 delete를 섣불리 호출하지 말 것. 이미 해당 Edge<V,E>객체는 Graph의 간선 관리 리스트에 저장되어 있을 것.
	//	간선 제거 후 delete 호출은 댕글링 포인터 생성을 야기할 수 있음.
	Edge<V, E>* removeEdge(Vertex<V, E>* v);

	//인접리스트 방식의 정점에서 사용 가능하다.
	//해당 간선을 인접 리스트에서 제거한다.
	//<Exception>
	//	반대편 정점이 v인 간선이 없다면 nullptr을 반환
	//<Warning>
	//	간선을 제거 후 delete를 섣불리 호출하지 말 것. 이미 해당 Edge<V,E>객체는 Graph의 간선 관리 리스트에 저장되어 있을 것.
	//	간선 제거 후 delete 호출은 댕글링 포인터 생성을 야기할 수 있음.
	Edge<V, E>* removeEdge(Edge<V, E>* e);

	//인접리스트 방식의 정점에서 사용 가능하다.
	//다른 정점들과 연결된 간선들이 저장된 LinkedList를 반환한다.
	//<Warning>
	//	Read Only로 사용하길 권장
	//	edgeList를 수정하고자 한다면 addEdge(), removeEdge() 등의 메소드를 호출하여 수정할 것.
	LinkedList<Edge<V, E>*>* getEdgeList();

	//정점의 데이터를 얻어온다.
	V getData();
};

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
template <typename V, typename E> class Edge {
private :
	Vertex<V,E>* v1;
	Vertex<V,E>* v2;
	E data;

public:
	Edge(Vertex<V, E>* v1, Vertex<V, E>* v2);
	~Edge();

	//간선의 가중치(데이터)를 설정한다.
	//<Warning>
	//	간선의 가중치 E가 포인터 타입의 경우, 메모리 누수의 가능성이 있음.
	//	메모리 누수를 예방하기 위한 방법 중 하나는 다음과 같음 :
	//		TestEdgeData* d = edge->getData();
	//		edge->setData(new TestEdgeData());
	//		delete d; // delete를 하여도 좋고, 다른 객체에 할당해도 좋다.
	void setData(E data);
	
	//간선의 가중치(데이터)를 얻어온다.
	E getData();
	
	//간선의 첫 번째 정점을 가져온다.
	//사용하지 않을 것을 권장.
	//<warning>
	//	두 정점 중 어떤 정점 값이 나올 지 모름. 만약 반대편 정점을 얻고자 한다면 이 메소드 대신
	//	getOpposite(Vertex<V,E>* v)메소드를 이용해 얻어올 것.
	Vertex<V, E>* getV1();
	
	//간선의 두 번째 정점을 가져온다.
	//사용하지 않을 것을 권장.
	//<warning>
	//	두 정점 중 어떤 정점 값이 나올 지 모름. 만약 반대편 정점을 얻고자 한다면 이 메소드 대신
	//	getOpposite(Vertex<V,E>* v)메소드를 이용해 얻어올 것.
	Vertex<V, E>* getV2();
	
	//간선이 가진 두 정점 중 한 정점에 대한 반대편의 정점을 가져온다.
	//<Exception>
	//	두 정점 중 어느 정점에도 해당하지 않는 정점이 매개변수로 들어간 경우에는 nullptr를 반환한다.
	Vertex<V, E>* getOpposite(Vertex<V,E>* v);

};

//DenseGraph<V,E>
//밀집그래프를 표현하기 위한 객체이다.
//V는 정점의 데이터의 자료형, E는 간선의 데이터의 자료형이다.
//<Methodes>
//	addVertex() : O(V) -> V is the number of vertices
//  removeEdgeMap() : O(V^2) -> V is the number of vertices
//	getEdgeMap() : O(1)
//	getVertices() : O(1)
template <typename V, typename E>
class DenseGraph {
private:
	CLAL<CLAL<Edge<V, E>*>*>* edgeMap;
	LinkedList<Vertex<V, E>*>* vertexList;
	LinkedList<Edge<V, E>*>* edgeList;
	int vertexNumber;

public:
	DenseGraph();
	~DenseGraph();
	
	//그래프에 정점을 추가하는 함수.
	//그래프에 정점이 추가될 때마다, edge를 표현하는 공간이 2V + 1만큼 늘어나게 됨.
	void addVertex(V v1);
	
	//edgeMap을 삭제하는 함수
	//<Warning>
	//	EdgeMap을 삭제 후 addVertex()를 즉시 호출하는 것은 권장하지 않음.
	//	이는 edgeMap필드에 새로운 간선을 추가하게 되며, 예상치 못한 메모리 사용을 하게 될 수 있음.
	void removeEdgeMap();
	
	//edgeMap 필드의 포인터를 반환하는 함수.
	//<Warning>
	//	객체를 직접 수정하는 것이 아닌 Read Only로 사용할 것을 권장.
	//	edgeMap 객체를 직접 수정하는 것은 예상치 못한 버그를 야기할 수 있음.
	CLAL<CLAL<Edge<V, E>*>*>* getEdgeMap();
	
	//현재까지 추가된 모든 vertex들을 담고 있는 LinkedList의 포인터를 반환한다.
	//<Warning>
	//	객체를 직접 수정하는 것이 아닌 Read Only로 사용할 것을 권장.
	//	LinkedList<Vertex<V,E>*> 객체를 직접 수정하는 것은 예상치 못한 버그를 야기할 수 있음.
	LinkedList<Vertex<V, E>*>* getVertices();
	
	//현재까지 생성된 모든 edge들을 담고 있는 LinkedList의 포인터를 반환한다.
	//<Warning>
	//	객체를 직접 수정하는 것이 아닌 Read Only로 사용할 것을 권장.
	//	LinkedList<Edge<V,E>*> 객체를 직접 수정하는 것은 예상치 못한 버그를 야기할 수 있음.
	LinkedList<Edge<V, E>*>* getEdges();

};




