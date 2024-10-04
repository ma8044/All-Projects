//============================================================================
// Name         : MyVector.cpp
// Author       : Mustafa Asif
// Version      : 1.0
// Date Created : 5 Jun 2024
// Date Modified: 5 Jun 2024
// Description  : Vector implmentation in C++
//============================================================================
// start_code+begins
#include<iostream>
#include<cstdlib>
#include<iomanip>
#include <stdexcept>
#include<sstream>

using namespace std;

template <typename T>
class MyVector
{
	private:
		T *data;						//pointer to int(array) to store elements
		int v_size;						//current size of vector (number of elements in vector)
		int v_capacity;					//capacity of vector
	public:
		MyVector(int cap=0);			//Constructor
		~MyVector();					//Destructor
		int size() const;				//Return current size of vector
		int capacity() const;			//Return capacity of vector
		bool isEmpty() const; 			//Rturn true if the vector is empty, False otherwise
		const T& front();				//Returns reference of the first element in the vector
		const T& back();				//Returns reference of the Last element in the vector
		void push_back(T element);		//Add an element at the end of vector
		void insert(int index, T element); //Add an element at the index 
		void erase(int index);			//Removes an element from the index
		T& operator[](int index);			//Returns the reference of an element at given index
		T& at(int index); 				//return reference of the element at given index
		void shrink_to_fit();			//Reduce vector capacity to fit its size
		void display();
};

void listCommands()
{
	cout<<"List of available Commands:"<<endl
		<<"display                  : Display the Vector"<<endl
		<<"push_back <element>      : Add an element to the end of the vector"<<endl
		<<"insert <index element>   : Insert an element at location index"<<endl
		<<"erase <index>            : Remove the element from index"<<endl
		<<"[index]                  : Returns a reference to an element at a specified index"<<endl
		<<"at <index>               : Returns a reference to an element at a specified index"<<endl
		<<"front                    : Return the (reference of) front element"<<endl
		<<"back                     : Returns a reference to the last element of the vector"<<endl
		<<"size                     : Returns the number of elements in the vector"<<endl
		<<"capacity                 : Returns the capacity of vector"<<endl
		<<"isEmpty                  : Tests if the vector container is empty"<<endl
		<<"shrink                   : Reduce vector capacity to fit its size (shrink_to_fit)"<<endl
		<<"exit/quit                : Exit the Program"<<endl;
}
//=======================================
int main()
{

	MyVector<int> myVector;
	//unitTest();
	string user_input;
	string command;
	string parameter1;
	string parameter2;
	listCommands();

	do
	{
		cout<<">";
		getline(cin,user_input);

		// parse userinput into command and parameter(s)
		stringstream sstr(user_input);
		getline(sstr,command,' ');
		getline(sstr,parameter1,' ');
		getline(sstr,parameter2);

		try
		{
			if(command=="display" or command=="d") 							myVector.display();
			else if(command=="push_back" or command=="p" )					myVector.push_back(stoi(parameter1)),myVector.display();
			else if(command=="insert" or command=="i")		    			myVector.insert(stoi(parameter1),stoi(parameter2)),myVector.display();
			else if(command=="erase" or command=="e")		    			myVector.erase(stoi(parameter1)),myVector.display();
			else if(command=="at" or command=="a")			    			cout<<myVector.at(stoi(parameter1))<<endl;
			else if(command[0]=='[' and command[command.length()-1]==']') 	cout<<myVector[stoi(command.substr(1,command.length()-2)+"")]<<endl;
			else if(command=="front" or command=="f")			            cout<<myVector.front()<<endl;
			else if(command=="back" or command=="b")			            cout<<myVector.back()<<endl;
			else if(command=="size" or command=="s")                        cout<<myVector.size()<<endl;
			else if(command=="capacity" or command=="c")                    cout<<myVector.capacity()<<endl;
			else if(command=="isEmpty" or command=="ie")			        cout<<boolalpha<<myVector.isEmpty()<<endl;
			else if(command=="shrink")          				            myVector.shrink_to_fit(),myVector.display();
			else if(command == "help" or command=="?")			            listCommands();
			else if(command == "exit" or command=="quit")			        break;
			else 								                            cout<<"Invalid Commad !!"<<endl;
		}
		catch(exception &e)
		{
			cout<<"Exception: "<<e.what()<<endl;
		}

	}while(command!="exit" and command!="quit");

	return EXIT_SUCCESS;
}
//======================================
#ifndef _WIN32
//======================================
template <typename T>
void MyVector<T>::display()
{
	cout<<"╔";
	
	for(int i=0; i<v_capacity; i++)
	{	
		cout<<"════";
		if(i!=v_capacity-1) cout<<"╦";
	}
	cout<<"╗ size = "<<v_size<<endl;
	
	for(int i=0; i<v_capacity; i++)
	{
		cout<<"║";
		if(i<v_size)
			cout<<setw(4)<<data[i];
		else
			cout<<setw(4)<<" ";		
	}
	if(v_capacity==0) cout<<"║";
	cout<<"║"<<endl<<"╚";
	for(int i=0; i<v_capacity; i++)
	{
		cout<<"════";
		if(i != v_capacity-1) cout<<"╩";
	}
	cout<<"╝ capacity = "<<v_capacity<<endl;	
}
//======================================
#else
template <typename T>
void MyVector<T>::display()
{
	cout<<"+";
	
	for(int i=0; i<v_capacity; i++)
	{	
		cout<<"----";
		if(i!=v_capacity-1) cout<<"+";
	}
	cout<<"+ size = "<<v_size<<endl;
	
	for(int i=0; i<v_capacity; i++)
	{
		cout<<"|";
		if(i<v_size)
			cout<<setw(4)<<data[i];
		else
			cout<<setw(4)<<" ";		
	}
	if(v_capacity==0) cout<<"|";
	cout<<"|"<<endl<<"+";
	for(int i=0; i<v_capacity; i++)
	{
		cout<<"----";
		if(i != v_capacity-1) cout<<"+";
	}
	cout<<"+ capacity = "<<v_capacity<<endl;	
}
#endif
//starter-code-ends

//Constructor
template <typename T> MyVector<T>::MyVector(int cap){
	v_capacity=cap;
	v_size=0;
	data = new T[cap]; //dynamically creates an array of size capacity
}

//Destructor
template <typename T> MyVector<T>::~MyVector(){
	delete[] data; //deletes dynamic array
}

//Returns Size
template <typename T> int MyVector<T>::size() const{
	return v_size; 
}

//Returns Capacity
template <typename T> int MyVector<T>::capacity() const{
	return v_capacity;
}

//Returns true if Vector is empty
template <typename T> bool MyVector<T>::isEmpty() const{
	return v_size==0;
}

//returns the front of the vector
template <typename T> const T& MyVector<T>::front(){
	if (isEmpty()) throw logic_error("Vector is Empty"); //throws exception if Vector is empty
		return data[0];
}

//returns the back of vector
template <typename T> const T& MyVector<T>::back(){
	if (isEmpty()) throw logic_error("Vector is Empty"); //throws exception if Vector is empty
		return data[v_size-1];
}

//adds an element to the end of the vector
template <typename T> void MyVector<T>::push_back(T element){
	if (v_size==v_capacity){ //checks if the current capacity is full
		v_capacity = max(1, 2*v_capacity); //If capacity is 0, then 1 is greater than 2*0.
		T *array = new T[v_capacity]; //Creates a new array with the new doubled capacity
		for (int i=0; i<v_size; i++) //Iterates through the vector array
			array[i]=data[i]; //Copies contents of vector array into new array
		if (data != NULL) delete [] data; //deletes contents of vector array
		data=array; //dynamic vector array points to new array
	}
	data[v_size++] = element; //Adds a new element at the back of vector and increases size by 1
}

//Adds an element at a specific index in the vector
template <typename T> void MyVector<T>::insert(int index, T element){
	//Exception handling
	if (index>v_size-1) throw logic_error("out_of_range"); 
	if (index<0) throw logic_error("index less than zero");
	if (v_size==v_capacity){ //Same process as previous method function to double the vector size if full
		v_capacity = max(1, 2*v_capacity);
		T *array = new T[v_capacity];
		for (int i=0; i<v_size; i++)
			array[i]=data[i];
		if (data != NULL) delete [] data;
		data=array;
	}
	for (int i=v_size-1; i>=index; i--){ //Moves all element in front of the index backward by 1
		data[i+1]=data[i];
	}
	data[index] = element; //inserts new element at index
	v_size++; //increases size by 1
}

//erases element at a certain index
template <typename T> void MyVector<T>::erase(int index){
	if (index>v_size-1) throw logic_error("out_of_range"); //Exception Handling
	for (int i=index; i<v_size;i++) //Brings elements after index forward by 1 which overwrites the index to be erased
		data[i]=data[i+1];
	v_size--; //decreases size by 1
}

//Operator function
template <typename T> T& MyVector<T>::operator[](int index){
	if (index>v_size-1) throw logic_error("out_of_range"); //Exception
	return data[index]; //returns the index of the array
}

//returns element at a certain index
template <typename T> T& MyVector<T>::at(int index){
	if (index>v_size-1) throw logic_error("out_of_range"); //Exception
	return data[index];
}

//shrinks the capacity to fit the size
template <typename T> void MyVector<T>::shrink_to_fit(){
	if (v_capacity>v_size){ //checks if capacity is greater than the size
		T *array = new T[v_size]; //creates another dynamic array which is the equal to size of vector
		for (int i=0; i<v_size; i++){ //copies all elements from vector array into new array
			array[i] = data[i];
		}
		if (data != NULL) delete[] data; //deletes contents of vector array
		data = array; //sets new array as vector array
		v_capacity=v_size; //changes capacity to equal the size
	}
}

