/*
 * @file lab6.cpp
 * @author Mustafa Asif 
 * @description Binary Heap Implementation in C++
 * @date 7 June 2024
 */
//starter_code_begins
#include<iostream>
#include<exception>
#include<math.h>
#include<ctime>
#include<vector>
#include<iomanip>
#include<sstream>
using namespace std;

class Heap
{
	private:
		vector<int> array;
	public:
		Heap();
		void insert(int key);
		int removeMin();
		int getMin();
		int parent(int k);
		int left(int k);
		int right(int k);
		void bubbleup(int k);
		void bubbledown(int k);
		void print();
		void sort();
};

void listCommands()
{
	cout<<"-----------------------------------------------------------------"<<endl;
	cout<<"display            :Display the Heap"<<endl
		<<"insert <key>       :Insert a new element in the Heap"<<endl
		<<"getMin             :Get the element with Min. Key in the Heap"<<endl
		<<"removeMin          :Remove the element with Min. Key from Heap"<<endl
		<<"sort               :Sort the Heap"<<endl
		<<"help               :Display the available commands"<<endl
		<<"exit/quit          :Exit the program"<<endl;
	cout<<"-----------------------------------------------------------------"<<endl;
}
//=============================================
void demo(Heap &myHeap)
{
	for (int i=1; i<=7; i++)
	{
		int n = rand()%25;
		myHeap.insert(n);
	}
	myHeap.print();
}
//=============================================
int main()
{

	Heap myHeap;
	string user_input;
	string command;
	string argument;

	listCommands();

	while(true)
	{
		try
		{
			cout<<">";
			getline(cin,user_input);
		
			// parse userinput into command and parameter(s)
			stringstream sstr(user_input);
			getline(sstr,command,' ');
			getline(sstr,argument);

			     if(command =="display" or command=="d")        myHeap.print();
			else if(command =="insert"  or command=="i")        { myHeap.insert(stoi(argument)); myHeap.print(); }
			else if(command =="getMin")        					{int min=myHeap.getMin();cout<<"Min Key = "<<min<<endl;}
			else if(command =="removeMin"  or command=="r")     { cout<<myHeap.removeMin()<<" has been removed..!"<<endl; myHeap.print(); }     
			else if(command =="sort")          					{ myHeap.sort(); myHeap.print(); }
			else if(command == "help")         					listCommands();
			else if(command == "demo")         					demo(myHeap);
			else if(command == "exit" or command == "quit") 	break;
			else cout<<"Invalid command !!!"<<endl;
		}
		catch (exception &e)
		{
			cout<<"Exception: "<<e.what()<<endl;
		}
	}
	return EXIT_SUCCESS;
}
//==================================================================
//This method prints a Heap in 2D format. 
void Heap::print()
{
	if(array.size()>1)
	{
		int levels = int(log2(array.size()));

		int *spaces = new int[levels+1];
		spaces[levels]=0;

		for(int j=levels-1; j>=0; j--)
		{
				spaces[j]=2*spaces[j+1]+1;
		}
		
		int level=0;
		for (int i=0; i<array.size()-1; i++)
		{
			if(i == (pow(2,level)-1))
			{
				cout<<endl<<endl;
			
				for(int k=0; k<spaces[level]; k++)
				cout<<"   ";
				level++;
			}
			cout<<std::left<<setw(3)<<array[i+1];
		
			if(level>1)
			{
				for(int k=0; k<spaces[level-2]; k++)
					cout<<"   ";
			}
			
		}
		cout<<endl;
		for(int i=0; i<spaces[0]*log2(array.size()/2)+4; i++)
			cout<<"__";
		
		cout<<endl;
		delete[] spaces;
	}

}
//starter_code_ends
//Constructor
Heap::Heap()
{
	array.push_back(0); //initializes the heap vector by pushing 0 into the 0 index.
}
// Insert an element in Heap keeping the Heap property intact 
void Heap::insert(int key)
{
	array.push_back(key); //pushes the new key into the vector
	bubbleup(array.size()-1); //places the key into the correct position
}
// Remove the minimum value from Heap keeping the Heap property intact
int Heap::removeMin()
{	
	if (array.size()==1) throw logic_error("Heap is Empty."); //throws exception
	int min = array[1]; //saves the minimum value
	array.erase(array.begin()+1); //erases the minimum
	array.insert(array.begin()+1, array[array.size()-1]); //replaces minimum with the last key
	array.erase(array.end()-1); //removes the last index
	bubbledown(1); //bubbles the last key down to its correct position
	return min; //returns the smallest value
}
// Return (but don't remove) the minimum value from the Heap
int Heap::getMin()
{
	if (array.size()==1) throw logic_error("Heap is Empty."); //throws exception
	return array[1]; //returns minimum which is top of the heap
}
// Returns the index of the parent of the node i
int Heap::parent(int k)
{	
	if (k>array.size()-1) return 0; //returns 0 if index is out of range
	if (k>1){ 
		if (k%2 == 0)	return k/2; //returns parent if key is on left of parent 
		else	return (k-1)/2; //returns parent if key is on right of parent
	}
	return 0; //returns 0 if k is the root
}
// Returns the index of the left child of the node i
int Heap::left(int k)
{
	if (2*k>array.size()) return 0; //Checks if k is external
	return 2*k; //If not, returns left key
}
// Returns the index of the right child of the node i
int Heap::right(int k)
{
	if ((2*k+1)>array.size()) return 0; //checks if k is external and has a right node
	return 2*k+1; //returns right key
}

void Heap::bubbleup(int k)
{
	if (parent(k) != 0 && array[parent(k)] > array[k]){ //Ensures that k is not out of range, is not root, and if key of parent is greater than child.
		int tmp = array[parent(k)]; //creates tmp int for key of parent
		array[parent(k)] = array[k]; //replaces key at parent with the key at k
		array[k] = tmp; //sets the key of k as that of parent
		bubbleup(parent(k)); //recursively calls the function until either root is reached, or key of parent is less than that of child
	}
	return; //returns when recursion stop
}

void Heap::bubbledown(int k)
{	
	//for condiiton when k has left but no right
	if (left(k) != 0 && right(k) == 0 && array[left(k)] < array[k]){ //checks if left is less than k
		//Swaps the keys of k and left index
		int tmp = array[left(k)]; 
		array[left(k)] = array [k];
		array[k] = tmp;
		bubbledown(left(k)); //recursively calls function until conditions are not met
	}

	//for condition when k has both left and right
	if (left(k) != 0 && right(k) != 0){
		//saves the index of the lower key from left and right
		int index = (array[left(k)]<array[right(k)] ? left(k) : right(k));
		if (array[index] < array[k]){ //checks if lower key is less than key at k
			//Swaps the key of k and lower index
			int tmp = array[index];
			array[index] = array[k];
			array[k] = tmp;
			bubbledown(index); //recursively calls function until conditions are not met
		}
	}
	return;	//returns when recursion stops
}
//=====================================
// This method will sort the internal array/vector 
// Hint: Leep extracting the min value from the heap and store them into an auxiliary array.
//       Copy the values from auxiliary array to the internal array/vector of heap
void Heap::sort()
{	
	if (array.size() == 1) throw logic_error("Heap is Empty."); //throws exception
	int size = array.size() - 1; //saves the size of the heap
	int aux[size]; //creates an auxilliary array of the size of the heap
	for (int i = 0; i<size; i++){
		aux[i] = removeMin(); //removes the minimum value from heap and adds it to the auxilliary array
	}
	for (int i = 0; i<size; i++){
		array.push_back(aux[i]); //adds everything from auxilliary array back into heap
	}
}
//=============================================