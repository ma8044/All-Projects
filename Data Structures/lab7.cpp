//============================================================================
// Name         : Hash Table
// Author       : 
// Version      : 1.0
// Date Created : 
// Date Modified: 
// Description  : Hash Table implementation using C++
//============================================================================
//starter-code-begins

#include<iostream>
#include<fstream>
#include<sstream>
#include<math.h>
#include<iomanip>
#include<list>

using namespace std;
class Entry
{
	private:
		string key;
		string value;
		bool deleted; 	//Is the bucket available to be reused (after deletion)
	public:
		Entry(string key, string value)
		{
			this->key = key;
			this->value = value;
			this->deleted = false;
		}
		friend class HashTable;
};

class HashTable
{
	private:
		Entry **buckets;		        			// Array of Pointers to Entries
		unsigned int capacity;				    	// Total Capacity of Hash Table
		unsigned int size;					   		// Current # of entries in the Hash Table
		unsigned int collisions; 					// Total Number of Collisions
	public:
		HashTable(int capacity);
		unsigned int getSize();
		unsigned int getCollisions();
		unsigned long hashCode(string key);
		void insert(string key, string value);
		void remove(string key); 
		string search(string key);
		~HashTable();
};
//======================================================
int main(void)
{
	ifstream fin;
	fin.open("zipcodes.csv");
	if(!fin){
		cout<<"Can not open the file <zipcodes.txt>...!"<<endl;
		exit(-1);
	}
	
	string line;
	HashTable myHashTable(28859);// Prime number 30% bigger than >25844 (19880*1.3);
	getline(fin,line);  //skip first line
	while(!fin.eof())
	{
		string key, value;
		getline(fin,key,',');
		getline(fin,value);
		//cout<<key<<":"<<value<<endl;
		std::cout.setstate(std::ios_base::failbit); //dont print any thing on terminal until the failbit is cleared again.
		myHashTable.insert(key,value);
		std::cout.clear();							//clear the failbit
	}
	fin.close();
	cout<<"==================================================="<<endl;
	cout<<"Hash Table size = "<<myHashTable.getSize()<<endl;
	cout<<"Total Number of Collisions = "<<myHashTable.getCollisions()<<endl;
	cout<<"Avg. # collisions/entry = "<<setprecision(2)<<float(myHashTable.getCollisions())/myHashTable.getSize()<<endl;	
	cout<<"==================================================="<<endl;

	string user_input, command, argument1, argument2;

	while(true)
	{
	
		cout<<">";
		getline(cin,user_input);

		stringstream sstr(user_input);
		getline(sstr,command,' ');
		getline(sstr,argument1,',');
		getline(sstr,argument2);

			if(command == "find")      cout<<"Zip code for "<<argument1<<" is: "<<myHashTable.search(argument1)<<endl;
		else if(command == "insert")   myHashTable.insert(argument1,argument2);
		else if(command == "remove")   myHashTable.remove(argument1);
		else if(command == "help")	   cout<<"Available Commands:\ninsert <key,value>\nremove <key>\nfind <key>"<<endl;
		else if(command == "exit")	   break;
		else cout<<"Invalid command !!!"<<endl;
	}
	return 0;
}
//===================================================
HashTable::HashTable(int capacity)
{
	buckets = new Entry*[capacity];		// Array of Pointers to Entries
	for(int i=0; i<capacity; i++)
		buckets[i] = nullptr;

	this->capacity = capacity;
	this->size = 0;
	this->collisions = 0;
}
//starter-code-ends
//======================================================
//Please add your implementaiton below this line

//Implements polynomial hash code
unsigned long HashTable::hashCode(string key){
	const unsigned long a = 31; //a is a constant which helps us in our hash code
	unsigned long h = 0; //h will return hash code
	unsigned long power_a = 1; //this helps make power of a initially 0, because a^0 will be 1.
	for (int i = key.length() - 1; i >=0 ; i--){ //iterates backward through the chars in the string
		h += key[i]*power_a; //adds the ascii of code * the current value of power_a to the hash value
		power_a *= a; //raises the power by 1 after every char
	}
	return h % capacity; //returns the hash value % capacity to make it fit into the array
}

//inserts element with a certain key and a certain value
void HashTable::insert(string key, string value){
	unsigned long i = hashCode(key); //converts our key to hashcode giving us an index
	unsigned long attempt = 0; //records the number of attempts
	unsigned long orig_i = i; //saves the original value of the index

	//while condition checks if the key is already present, or if the position in the array is occupied by something else.
	while (buckets[i] != nullptr && !buckets[i]->deleted && buckets[i]->key != key){
		//If the position is occupied, we perform quadratic probing
		attempt++; //attempt increases by 1 for every time the loop runs
		i = (orig_i+attempt*attempt) % capacity; //makes a new index, which is the original index + the square of current attempt

		if (attempt*attempt>capacity){ //condition makes sure that a suitable position is found.
			cout << "Hashtable is full or cannot find a suitable position to insert." << endl;
			return;
		}
	}
	//checks if the index is a nullptr
	if (buckets[i] == nullptr){
		buckets[i] = new Entry(key, value); //inserts the new entry into the index
		cout<<"New record has been added successfully."<<endl;

	}
	//checks if index has a key but lazy deletion has occured there
	else if (buckets[i]->deleted){
		//changes value of key and value and sets deleted to false
		buckets[i]->key = key;
		buckets[i]->value = value;
		buckets[i]->deleted = false;
		cout<<"New record has been added successfully."<<endl;
	}
	//this is only in the condition that the record already exists
	else{
		//updates the value of the key
		buckets[i]->value = value;
		cout<<"Existing record has been updated."<<endl;
		return;
	}
	//increases size by 1 and capacity by number of attempts
	size++;
	collisions+=attempt;
}

//searches for an element thats already present, or prints statement if not found
string HashTable::search(string key){
	//uses the same code as search to find the index where the key might be present
	unsigned long i = hashCode(key);
	unsigned long attempt = 0;
	unsigned long orig_i = i;

	while (buckets[i] != nullptr && !buckets[i]->deleted && buckets[i]->key != key){
		attempt++;
		i = (orig_i+attempt*attempt) % capacity;

		if (attempt*attempt>capacity){
			return "Suitable position was not found during insertion.";
		}
	}
	//if a nullptr or a deleted position was found, prints that record was not found
	if (buckets[i] == nullptr || buckets[i]->deleted) return "Record not found...!";
	//if the key was found, returns the value along with number of attempts made to reach it.
	return buckets[i]->value + "  (Collisions = " + to_string(attempt) + ")";
}

//removes a key
void HashTable::remove(string key){
	//uses same code as insertion and search to find the index
	unsigned long i = hashCode(key);
	unsigned long attempt = 0;
	unsigned long orig_i = i;

	while (buckets[i] != nullptr && !buckets[i]->deleted && buckets[i]->key != key){
		attempt++;
		i = (orig_i+attempt*attempt) % capacity;

		if (attempt*attempt > capacity){
			cout << "Suitable position was not found during insertion." << endl;
			return;
		}
	}
	//if index is not found
	if (buckets[i] == nullptr || buckets[i]->deleted) cout<<"Key was not found."<<endl;
	//Case when key is found
	else if (buckets[i]->key == key){
		//sets the deleted of the entry to true
		buckets[i]->deleted = true;
		//decreases size by 1
		size--;
		cout<<"Record was removed."<<endl;
	}
}
//returns size
unsigned int HashTable::getSize(){
	return size;
}
//returns collision
unsigned int HashTable::getCollisions(){
	return collisions;
}
//Destructor
HashTable::~HashTable(){
	for (int i = 0; i<capacity; i++){
			delete buckets[i]; //deletes the Entry at every index
		}
	//deletes the array itself
	delete[] buckets;
}
