#include <iostream>

using namespace std;

class Node
{
public:
	Node(int value, Node* nextptr = NULL, Node* prevptr = NULL, int currentpriority = 0);

	int getVal(void);

	Node* getNext(void);

	Node* getPrev(void);

	void setVal(int value);

	void setPrev(Node* prevptr);

	void setNext(Node* nextptr);

	int getPriority(void);

	void setPriority(int priority);

private:
	Node* next;
	Node* prev;
	int priority;
	int value;
};

class Stack
{
public:
	Stack(void);

	~Stack(void);

	void Push(int value);

	Node* NodePop(void);

	int Pop(void);

private:

	Node* top;
};

class Queue
{
public:
	Queue(void);

	~Queue(void);

	void Enqueue(int i, int priority = 0);

	int Dequeue(void);

protected:

	Node* back;
	Node* front;

private:

	virtual Node* NodeDequeue(void);
};

class Scheduler : public Queue
{
public:
	void Enqueue(int i, int priority = 0); //overloading queue enqueue
private:
	Node* NodeDequeue(void);
};

//setting the pointers and and assigning the process id and priority to the node
Node::Node(int value, Node* nextptr, Node* prevptr, int currentpriority)
{
	this->value = value;
	next = nextptr;
	prev = prevptr;
	priority = currentpriority;
}

//returns the process id of the node
int Node::getVal(void)
{
	return value;
}

//returns the position of the next node
Node * Node::getNext(void)
{
	return next;
}

//returns the position of the prevoius node
Node * Node::getPrev(void)
{
	return prev;
}

//sets process id/value of the node
void Node::setVal(int value)
{
	this->value = value;
}

//sets the previous position to the prev node
void Node::setPrev(Node* prevptr)
{
	prev = prevptr;
}

//sets the next position to the next node 
void Node::setNext(Node* nextptr)
{
	next = nextptr;
}

//returns the priority 
int Node::getPriority(void)
{
	return priority;
}

//sets the priority of node 
void Node::setPriority(int priority)
{
	this->priority = priority;
}

Stack::Stack(void)
{
	top = 0;
}

//goes through the stack and deleting all pointers
Stack::~Stack(void)
{
	while (top != nullptr) {
		delete NodePop();
	}
}

//adds tha value to the top of the stack
void Stack::Push(int value)
{
	Node* tmp = new Node(value, top);
	top = tmp;
}

//returns the pointer pointing to the top node
Node* Stack::NodePop(void)
{
	Node* tmp = top;
	if (top != nullptr) top = top->getNext();
	return tmp;
}

//pops the top number on the stack
int Stack::Pop(void)
{
	int ret = 0;
	if (top != nullptr) //check to make sure there is something in the stack
	{
		ret = top->getVal();
		Node* tmp = top;
		top = top->getNext();
		delete tmp;
	}
	else throw "stack empty"; //exception is thrown when stack is empty
	return ret;
}

Queue::Queue(void)
{
	front = back = nullptr;
}

//Queue destructor...goes through the queue deleting all pointers
Queue::~Queue(void)
{
	while (front != nullptr) {
		delete NodeDequeue();
	}
}

//adds data to the node 
void Queue::Enqueue(int i, int priority)
{
	Node* tmp = new Node(i, back);
	back = tmp;
	if (front == nullptr) front = back; //if the queue is empty front and back point to the same node
	else
	{
		tmp = back->getNext();
		tmp->setPrev(back);
	}
}

//add data to the node and sorts by priority
void Scheduler::Enqueue(int i, int priority)
{
	Node* tmp = new Node(i, nullptr, back, priority); 
	int maxPriority = 10, minPriority = 1;

	if (priority > maxPriority || priority < minPriority) //checks if the node is between 1 and 10
	{ 
		cout << "Priority for " << i << " should be between 1 and 10. Not:" << priority << endl;
	}
	else {
		if (front != nullptr) //checks the queue is not empty
		{
			if (tmp->getPriority() > front->getPriority()) { tmp->setNext(front); front = tmp;  }// compares the priority of the recently added node to the top node
			else
			{
				Node* prevTemp = front;
				Node* temp = front->getNext(); //next node after the fron node because we have already compared it to the top
				while (temp != nullptr && temp->getPriority() > tmp->getPriority()) //checks if the new node has a higher priority that next node
				{
					prevTemp = temp; // sets prevTemp to the node that was just compared 
					temp = temp->getNext(); //points to the next node so it can compare it next to tmp 
				}
				if (temp == nullptr) { prevTemp->setNext(tmp); back = tmp; } //if we get to the end of the list then add the new node to the end
				else { tmp->setNext(temp); prevTemp->setNext(tmp);  }//add the new node between temp and prevTemp because its priority is bigger than temp
			}
		}
		else
		{
			// if the queue is empty then add the new node.. since its the only node front and back point to it
			front = tmp;
			back = tmp;
		}
	}
	
}

//this dequeues the node
int Queue::Dequeue(void)
{
	Node* tmp = NodeDequeue(); //takes the tmp returned from NodeDequeue
	int ret = 0; 
	
		if (tmp != nullptr) { //checks there is something to dequeue
			ret = tmp->getVal();
		}
		else
		{
			cout << "queue empty"; //if there is nothing to dequeue inform user
		}
		
	if (front == nullptr) back = front; //if there is only 1 item left to dequeue then back and front point to it
	delete tmp;
	return ret;
}

//returns the pointer pointing to the front node
Node* Queue::NodeDequeue(void)
{
	Node* tmp = front;
	if (front != nullptr)
	{
		front = front->getPrev();
		if (front != nullptr) front->setNext(nullptr);
	}
	return tmp; //tmp is pointing to the node thats going to be dequeued next
}

//returns the pointer pointing to the front node
Node* Scheduler::NodeDequeue(void)
{
	Node* tmp = front;
	if (front != nullptr)
	{
		front = front->getNext();
		if (front != nullptr) front->setPrev(nullptr);
	}
	return tmp; //tmp is pointing to the node thats going to be dequeued next
}

