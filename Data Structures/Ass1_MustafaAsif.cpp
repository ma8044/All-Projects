/**
 * Assignment 1
 * Mustafa Asif 
 * Doubly LinkedList implementation in C++
 * 2023-03-22
 */
//startercode_begins
#include<iostream>
#include<string>
#include<sstream>
#include<iomanip>
using namespace std;


class Node
{
	private:
		string elem; //data element (type string in this case)
		Node* next; //Link (pointer) to the next Node
		Node* prev; //Link (pointer) to the previous Node
		friend class DoublyLinkedList;
		
	public:
		Node(): next(NULL), prev(NULL)
		{}
		Node(string elem) : elem(elem),next(NULL), prev(NULL)
		{}
};
//==============================================================
class DoublyLinkedList
{
	private:
		Node* head; 	// pointer to the head of List
		Node* tail; 	// pointer to the tail of List
	public:
		DoublyLinkedList (); // constructor to create dummy head and tail nodes
		~DoublyLinkedList (); // destructor to clean up all nodes
		bool isEmpty() const; // return true if the list is empty
		const string& front() const; // returns the const reference of the element from the first valid Node of list, throw an exception if the list is empty
		const string& back() const;  // returns the const reference of the element from last valid Node of the list, throw an exception if the list is empty
		void addFront(const string& elem); // add a new node to the front of list (after head)
		void addBack(const string & elem); //add a new node to the back of the list (before tail)
		void removeFront(); // remove front node from list, throw an exception if the list is empty
		void removeBack();  // remove last node from list, throw an exception if the list is empty
		void reverseList();	// reverse the list
		bool isPalindrome() const; //return true if the list is Palindrome, false otherwise
		void display() const;	// display all element of the list
};
//===================================
void listCommands()
{
	cout<<"List of available Commands:"<<endl
		<<"display            : Display the Doubly Linked List"<<endl
		<<"front              : Display the first element of the List"<<endl
		<<"back               : Display the last element of the List"<<endl
		<<"addFront <elem>    : Add a new Node at the beginning of the List"<<endl
		<<"addBack  <elem>    : Add a new Node at the end of the List"<<endl
		<<"removeFront        : Remove the first Node of the Linked List"<<endl
		<<"removeBack         : Remove the last Node of the Linked List"<<endl	
		<<"isEmpty            : Check if the List is empty?"<<endl
		<<"reverse            : Reverse the Linked List"<<endl
		<<"isPalindrome       : Check if the List is Palindrome or not?"<<endl
		<<"help               : Display the list of available commands"<<endl
		<<"exit               : Exit the Program"<<endl;
}
//===================================
int main()
{
	DoublyLinkedList myList;
	listCommands();
	string user_input;
	string command;
	string parameter;


	do
	{
		cout<<">";
		getline(cin,user_input);

		// parse user-input into command and parameter(s)
		stringstream sstr(user_input);
		getline(sstr,command,' ');
		getline(sstr,parameter);

		try
		{
			if(command=="display" or command=="d") 				myList.display();
			else if(command=="front" or command=="f")			cout<<"Front = "<<myList.front()<<endl;
			else if(command=="back" or command=="b")			cout<<"Back = "<<myList.back()<<endl;
			else if(command=="addFront" or command=="af")		myList.addFront(parameter),myList.display();
			else if(command=="addBack"	or command=="ab")		myList.addBack(parameter),myList.display();
			else if(command=="removeFront" or command=="rf")	myList.removeFront(),myList.display();
			else if(command=="removeBack" or command=="rb")		myList.removeBack(),myList.display();
			else if(command=="isEmpty"	or command=="ise")		cout<<"List is "<<(myList.isEmpty()? "Empty":"Not Empty")<<endl;
			else if(command=="reverse"	or command=="r")		myList.reverseList(),myList.display();
			else if(command=="isPalindrome" or command=="isp")	cout<<"List is "<<(myList.isPalindrome()? "Palindrome":"Not Palindrome")<<endl;
			else if(command == "help" or command=="h")			listCommands();
			else if(command == "exit" or command=="e")			break;
			else 												cout<<"Invalid Commad !!"<<endl;
		}
		catch(exception &e)
		{
			cout<<"Exception: "<<e.what()<<endl;
		}

	}while(command!="exit" and command!="quit");

	return EXIT_SUCCESS;
}
//============================================================
#ifdef _WIN32
void DoublyLinkedList::display() const
{
	Node *curr=head;
	while(curr!=NULL)
	{
		cout<<"+------";
		curr=curr->next;
	}
	cout<<"+"<<endl<<"|";
	curr=head;
	while(curr!=NULL)
	{
		if (curr == head)		cout<<" Head |";
		else if(curr == tail)	cout<<" Tail |";
		else 					cout<<setw(5)<<curr->elem<<" |";
	
		curr=curr->next;
	}
	curr=head;
	cout<<endl;
	while(curr!=NULL)
	{
		cout<<"+------";
		curr=curr->next;
	}
	cout<<"+"<<endl;
}
//====================================================================
#else
void DoublyLinkedList::display() const
{
	Node *curr=head;
	while(curr!=NULL)
	{
		cout<<"╔══════╗    ";
		curr=curr->next;
	}
	cout<<endl<<"║";
	curr=head;
	while(curr!=NULL)
	{
		if (curr == head)		cout<<" Head ║<══>║";
		else if(curr == tail)	cout<<" Tail ║";
		else 					cout<<setw(6)<<curr->elem<<"║<══>║";
	
		curr=curr->next;
	}
	curr=head;
	cout<<endl;
	while(curr!=NULL)
	{
		cout<<"╚══════╝    ";
		curr=curr->next;
	}
	cout<<endl;
}
#endif
//startercode_ends
//===========================================================
//Constructor to create Sentinel (dummy) nodes
DoublyLinkedList::DoublyLinkedList ()
{
	head = new Node; //head is a pointer which points to an empty Node
	tail = new Node; //tail is a pointer which points to an empty Node
	head->next = tail; //ensures that head and tail are linked in an empty list
	tail->prev = head;
}
//============================================================
DoublyLinkedList::~DoublyLinkedList () 
{
	while (!isEmpty()) removeFront(); //deletes Nodes until the list is empty
}
//============================================================
bool DoublyLinkedList::isEmpty() const 
{
	return (head -> next == tail); //returns true if head points to tail, which would mean that list is empty.
}
//============================================================
const string& DoublyLinkedList::front() const 
{
	if (isEmpty()) throw logic_error("List is Empty!"); //handles exception (list being empty)
	return head -> next -> elem; //returns the element of the Node that is after the Null Node that head points to.

}
//============================================================
const string& DoublyLinkedList::back() const 
{
	if (isEmpty()) throw logic_error("List is Empty!"); //handles exception (list being empty)
	return tail -> prev -> elem; //returns the element of the Node that is before the Null Node that tail points to.
}
//============================================================
void DoublyLinkedList::addFront(const string& elem) 
{
	Node* v = new Node(elem); //New node "v" which contains the element
	v->prev = head; //The previous pointer of "v" now points to where head points to.
	v -> next = head -> next; //The next pointer of v points to where the next of the head points.
	head -> next = v; //The next of head points to v.
	v -> next -> prev = v; //The prev of the node after v points to v.
}
//============================================================
void DoublyLinkedList::addBack(const string& elem) 
{
	Node* v = new Node(elem); //New node "v" which contains the elem
	v->next = tail; //The next of "v" points to tail
	v->prev = tail -> prev; //The prev of v, points to the 2nd last Node in the list.
	tail -> prev = v; //prev of tail points to v
	v -> prev -> next = v; //next of Node behind v points to v.
}
//============================================================
void DoublyLinkedList::removeFront() 
{
	if (isEmpty()) throw logic_error("List is Empty!"); //handles exception (list being empty)
	Node* tmp = head -> next; //Creates a pointer that points to the first Node in the list
	head -> next = tmp -> next; //The next pointer of head now points to 2nd Node
	head -> next -> prev = head; //The prev of 2nd Node now points to head.
	delete tmp; //we delete the 1st Node, making the 2nd Node the new 1st Node.
}
//============================================================
void DoublyLinkedList::removeBack() 
{
	if (isEmpty()) throw logic_error("List is Empty!"); //handles exception (list being empty)
	Node* tmp = tail -> prev; //creates a pointer pointing to last Node in the list
	tail -> prev = tmp -> prev; //prev of tail now points to 2nd last Node.
	tmp -> prev -> next = tail; //next of 2nd last Node points to tail
	delete tmp; //Deleting last node.
}
//============================================================
void DoublyLinkedList::reverseList() 
{
	if (isEmpty()) throw logic_error("List is Empty!"); //handles exception (list being empty)
	Node* v = head -> next; //creates a pointer "v" initially pointing to the first Node
	Node* tmp; //Creates another pointer "tmp"
	while (v != tail){ //ensures that loop stops when entire list has been iterated through
		//The While loop swaps positions of next and prev pointers to switch around
		tmp = v -> next; //tmp points to the next node of "v"
		v -> next = v -> prev; //the next node of v now points to where the prev is pointing
		v -> prev = tmp; //The prev now points to where the next was pointing
		v = v -> prev; //takes us to the next element (which is now the prev elem) in the List
	}
	//Since now head will be pointing to "last" element and tail to "first", we can switch them
	tmp = head -> next; //tmp points to where the Node after head was pointing to
	head -> next = tail -> prev; //head now points to the Node behind tail
	tail -> prev = tmp; //tail now points to where head was initially pointing

	//Since the prev of new first Node is still pointing to tail
	//and next of initial last Node is still pointing to head, we switch those
	head -> next -> prev = head; //makes it so that the prev of new first node points to head
	tail -> prev -> next = tail; //makes it so that the next of new last node points to tail
}
//============================================================
bool DoublyLinkedList::isPalindrome() const
{
	if (isEmpty()) throw logic_error("List is Empty"); //handles exception (list being empty)

	Node* mright = head -> next; //pointer which traverses list to the right starting from first Node
	Node* mleft = tail -> prev; //pointer which traverses list to the left starting from last Node

	//First while condition works if list is odd number of Nodes, both pointers will meet in the middle and point to the same Node, after which loop breaks
	//Second while condition works if list is even number of Nodes, if the two nodes cross each other, the loop breaks
	while ((mright != mleft) && (mleft -> next != mright)){
		if (mright->elem != mleft->elem) return false; //if any two elems are not same, the function returns false
		mright = mright -> next; //moves right pointer to the right
		mleft = mleft -> prev; //moves left pointer to the left
	}
	return true; //returns true if while loop was successfully broken without two elems being different.
}
