//============================================================================
// Name         : lab2.cpp
// Author       : Mustafa Asif
// Version      : 1.0
// Date Created : 24 May 2024
// Date Modified: 24 May 2024
// Description  : Singly Linked-List implementation in C++
//============================================================================
//starter_code_begins
#include<iostream>
#include<string>
#include<fstream>
#include<exception>
#include<sstream>
using namespace std;
//====================================
class Node
{
	private:
		int elem; //data element 
		Node* next; //Link (pointer) to the next Node
		
	public:
		Node(int elem) : elem(elem), next(NULL)
		{}
		friend class MyLinkedList ;
};
//=====================================
class MyLinkedList
{
	private:
		Node* head; // pointer to the head of list
	public:
		MyLinkedList (); // empty list constructor
		~MyLinkedList (); // destructor to clean up all nodes
		bool empty() const; // is list empty?
		void addFront(int elem); // add a new Node at the front of the list
		void removeFront(); // remove front Node from the list
		void remove(int elem);	 // remove the first occurrence of an element of the list, if the element exists.
		unsigned int countElem(int elem) const; //count frequency of an element in the list
		int getIndexOf(int elem) const; //returns first index of an element in the list, -1 if the element is not present
		void display() const;
		void readData(string); //read a file and load it into the linked list
		void writeData(string) const; //write the linked list to a file
};
//=====================================
void listCommands()
{
	cout<<"List of available Commands:"<<endl
		<<"display            : Display the Linked List"<<endl
		<<"addFront <element> : Add <element> to the front of the Linked List"<<endl
		<<"removeFront        : Remove the front node of the Linked List"<<endl
		<<"remove        	  : Remove the first occurrence of an element of the list (if exists)"<<endl
		<<"count <element>    : count frequency of an element in the list"<<endl
		<<"indexOf <element>  : return first index of an element in the list (-1 of not present)"<<endl
		<<"read <file_name>   : Load the data from <file> and add it into the Linked List"<<endl
		<<"write <file_name>   : Dump the contents of the Linked list to <file>"<<endl
		<<"help               : Display the list of available commands"<<endl
		<<"exit               : Exit the Program"<<endl;
}
//=======================================
// main function
int main()
{
	MyLinkedList myList;

	listCommands();
	string user_input;
	string command;
	string parameter;

	do
	{
		cout<<">";
		getline(cin,user_input);
		
		// parse user input into command and parameter(s)
		stringstream sstr(user_input);
		getline(sstr,command,' ');
		getline(sstr,parameter);
		
		if(command=="display") 				myList.display();
		else if(command=="addFront" or command=="af")				myList.addFront(stoi(parameter)),myList.display();
		else if(command=="removeFront" or command=="rf")			myList.removeFront(),myList.display();
		else if(command=="remove" or command=="r")         			myList.remove(stoi(parameter)),myList.display();
		else if(command=="count")			cout<<parameter<<" occurs "<<myList.countElem(stoi(parameter))<<" time(s) in the list"<<endl;
		else if(command=="indexOf")         cout<<"First index of "<<parameter<<" in the list is: "<<myList.getIndexOf(stoi(parameter))<<endl;
		else if(command=="read")			myList.readData(parameter),myList.display();
		else if(command == "write")			myList.writeData(parameter);
		else if(command == "help")			listCommands();
		else if(command == "exit")			break;
		else 								cout<<"Invalid Command !!"<<endl;
	}while(command!="exit");
	return EXIT_SUCCESS;
}
//====================================
// constructor
MyLinkedList::MyLinkedList ()
{
	this->head = NULL;
}
//starter_code_ends
//====================================
// destructor to clean up all nodes
MyLinkedList::~MyLinkedList () 
{
	while (!empty()) removeFront();//if list is not empty, removes front until it is
}
//====================================
// Check if the list is empty or not
bool MyLinkedList::empty() const 
{
	return head == NULL; //returns true if head points to null, meaning when list is empty
}
//====================================
// add a node at the front of list
void MyLinkedList::addFront(int elem)
{
	Node* v = new Node(elem); //Creates a pointer which points to a new node
	v-> next = head; //The next of the node points to where the head was pointing to.
	head = v; //The head points to the new node.
}
//====================================
// remove the first node from the list
void MyLinkedList::removeFront()
{
	if (!empty()){ //only allows removal if list has no nodes
		Node* tmp = head; //creates a temporary pointer
		head = tmp->next; //makes head point to the second node in the list
		delete tmp; //deletes the first node
	}
	else cout<<"Error: List is empty!"<<endl; //Error message
}
//=====================================
// remove the first occurrence of an element of the list, if the element exists
void MyLinkedList::remove(int elem)
{
	if (empty()) {cout<<"Error: Element not Found!"<<endl; return;} //If list is empty
	if (head->elem == elem) {removeFront(); return;} //If the elem is at the head then we use removeFront()
	Node* tmp = head; //creates a pointer initially pointing to the head
	while(tmp->next != NULL){ //loop runs if the node after our temporary node is not null
		if (tmp->next->elem == elem){ //conditional checks if the next node contains the element
			Node* v=tmp->next; //assigns a pointer to the next node
			tmp->next=v->next; //the next node of tmp becomes the node which occurs 2 nodes after tmp
			delete v; //deletes the node after tmp which contains the element
			return;
		}
		tmp = tmp->next; //if element is not found, tmp now points to its next node
	}
cout<<"Error: Element not Found!"<<endl; //error message
}
//====================================
// count frequency of an element in the list
unsigned int MyLinkedList::countElem(int elem) const
{
	Node* tmp = head; 
	int count = 0; //integer which stores the number of elements of "elem" we find
	while (tmp != NULL){ //makes sure that our pointer is not pointing to a null value
		if (tmp->elem == elem) count++; //if pointer is pointing to a node which contains element, we add 1 in count
		tmp=tmp->next; //pointer now points to its next valu
	}
	return count; //returns our int value
}
//==============================================
// get first index of an element in the the list.
// returns -1 if the element is not present
int MyLinkedList::getIndexOf(int elem) const
{
	Node* tmp = head;
	int index = 0; //integer which stores the value of the index
	while (tmp != NULL){
		if (tmp->elem == elem) return index; //if element is found the index value is returned.
		tmp=tmp->next; 
		index++; //if element is not found, adds 1 to the index value.
	}
	
	return -1; //returns -1 if element is not found.
}
//==============================================
// display all nodes of the linked list
void MyLinkedList::display() const
{
	cout<<"Head->";
	Node* tmp = head;
	while (tmp != NULL){ //iterates through the full list and prints the values
		cout<<tmp->elem<<"->";
		tmp=tmp->next;
	}
	cout<<"NULL \n"; //prints NULL
}
//==============================================
//Load data from a file and add it to the Linked List
void MyLinkedList::readData(string path){
	ifstream fin(path); //opens a file to read from
	int elem; 
	if (fin.is_open()){ //checks if file is open
		while(fin >> elem) addFront(elem); //while we can read integers from our file, we add them to the front.
	}
	else cout<<"Error: File not Found!"<<endl; //error occurs if file is not found.
	fin.close(); //closes the file.
}
//=============================================
//Dump/write the contents of the list to a file
void MyLinkedList::writeData(string path) const{ 
	ofstream fout(path); //opens an output file
	if (fout.is_open()){ 
		Node* tmp = head;
		int records = 0; //records the number of records that are present in the list.
		while (tmp!=NULL){
			fout<<tmp->elem<<endl; //outputs the element that our pointer points to into the file
			tmp=tmp->next;
			records++; //increases records by 1 for every element outputted.
		}
		cout<<records <<" records have been exported to "<<path<<endl; //prints a statement for user.
	}
	else cout<<"File Error!"<<endl; //Error statement

	fout.close(); //closes file
}
//==================================================

