
//============================================================================
// Name         : Mustafa Asif
// Author       : Khalid Mengal 
// Version      : 1
// Date Created : 7 Jun 2024
// Date Modified: 
// Description  : General Tree implmentation in C++
//============================================================================
//starter_code_begins
#include<iostream>
#include<sstream>
#include<stdlib.h>
#include<vector>
using namespace std;

class Node
{
	private:
		string name;				//name of the Node
		vector<Node*> children;		//Children of Node
		Node* parent; 				//link to the parent 

	public:
		Node(string name) : name(name), parent(nullptr)
		{ }
		string getName()
		{
			return this->name;
		}
		~Node();
		friend class Tree;
};
//==========================================================
class Tree
{
	private:
		Node *root;				//root of the Tree
		Node *curr_Node;			//current Node
		
	public:	
		//Methods that are part of the starter-code	
		Tree();	
		~Tree();
		Node* getRoot();
		Node* getCurrentNode();
		void cd(string name);
		void print();			//Print entire tree
	private:	//Helper methods
		void print_helper(string padding, string pointer,Node *node);
		bool isLastChild(Node *ptr);
	public:
		//Required Methods
		void insert(Node* node,string name);
		void remove(Node* node,string child_name);
		bool isExternal(Node* node);
		bool isInternal(Node* node);
		int size(Node* node);
		int depth(Node* node);
		int height(Node *node);
		int treeHeight();
		void preorder(Node *node);
		void postorder(Node *node);
};
//==================================================================
void help(void)
{
	cout<<"print/tree          : Print the Tree"<<endl
	    <<"insert <child-name> : Insert a new node as a child of the current node"<<endl
	    <<"remove <child-name> : Removes a specific child from the current node"<<endl
	    <<"size                : Print the total number of nodes in the Tree"<<endl
	    <<"height              : Print the height of the current node"<<endl
	    <<"depth               : Print the depth of the current node"<<endl
	    <<"treeHeight          : Print the height(Max-depth) of the Tree"<<endl
	    <<"isExternal          : Check if the current node is an external node or not"<<endl
	    <<"isInternal          : Check if the current node is an internal node or not"<<endl
	    <<"preorder            : Traverse/Print the Tree in pre-order"<<endl
	    <<"postorder           : Traverse/Print the Tree in post-order"<<endl
	    <<"help                : Display the list of available commands"<<endl
	    <<"exit                : Exit the Program"<<endl;
}
//==================================================================
int main()
{
	Tree tree;
	help();
	while(true)
	{
		string user_input;
		string command;
		string parameter;
		cout<<">";
		getline(cin,user_input);

		// parse userinput into command and parameter(s)
		stringstream sstr(user_input);
		getline(sstr,command,' ');
		getline(sstr,parameter);

		try
		{
				 if(command=="insert" or command=="i")		tree.insert(tree.getCurrentNode(),parameter); 	
			else if(command=="remove" or command=="r")		tree.remove(tree.getCurrentNode(),parameter);
			else if(command=="cd")							tree.cd(parameter);
			else if(command=="size")						cout<<"Size of the Tree is :"<<tree.size(tree.getRoot())<<endl;
			else if(command=="depth")						cout<<"The depth of the current node("<<tree.getCurrentNode()->getName()<<") is: "<<tree.depth(tree.getCurrentNode())<<endl;
			else if(command=="height")						cout<<"The height of the current node("<<tree.getCurrentNode()->getName()<<") is: "<<tree.height(tree.getCurrentNode())<<endl;
			else if(command=="treeHeight")					cout<<"The height(max-depth) of the Tree is: "<<tree.treeHeight()<<endl;
			else if(command=="print" or command=="tree")	tree.print();		//print the tree
			else if(command=="preorder")					tree.preorder(tree.getRoot()),cout<<endl;
			else if(command=="postorder")					tree.postorder(tree.getRoot()),cout<<endl;
			else if(command=="help")                        help();
			else if(command=="exit")						break;
			else if(command=="isExternal")					cout<<"Current node("<<tree.getCurrentNode()->getName()<<") is "<<((tree.isExternal(tree.getCurrentNode())==true) ? "":"not ")<<"an external node of the tree."<<endl;
			else if(command=="isInternal")					cout<<"Current node("<<tree.getCurrentNode()->getName()<<") is "<<((tree.isInternal(tree.getCurrentNode())==true) ? "":"not ")<<"an internal node of the tree."<<endl;
			else 											cout<<command<<": command not found"<<endl;
		}
		catch(exception &e)
		{
			cout<<"Exception: "<<e.what()<<endl;
		}

	}
}
//========================================================================
Tree::Tree()
{
	this->root = new Node("root");
	this->curr_Node = this->root;
}
Tree::~Tree()
{
	delete this->root;
}
//========================================================================
Node* Tree::getRoot()
{
	return root;
}
//========================================================================
Node* Tree::getCurrentNode()
{
	return curr_Node;
}
//========================================================================
void Tree::cd(string name)
{
	if(name=="..")
	{
		if(curr_Node!=root)	curr_Node = curr_Node->parent;
	}
	else
	{
		for(int i=0; i<curr_Node->children.size(); i++)
			if(name == curr_Node->children[i]->name)
			{
				curr_Node = curr_Node->children[i];						//update current working directory
				return;
			}

		cout<<name<<": child not found"<<endl;
	}
}
//==========================================================================
void Tree::print()
{
	print_helper("","",root);
}
//==========================================================================
void Tree::print_helper(string padding, string pointer,Node *node)
{
    if (node != nullptr) 
    {
        if(node == curr_Node)	//print current Node in blue color
        	cout <<padding<<pointer<<"\x1B[33m"<<node->name<<"\x1B[0m"<<endl;
        else 
        	cout <<padding<<pointer<<node->name<<endl;

		if(node!=root)	padding+=(isLastChild(node)) ? "   " : "│  ";


        for(int i=0; i<node->children.size(); i++)	//remove the file/folder from original path
		{
			string marker = isLastChild(node->children[i]) ? "└──" : "├──";
			print_helper(padding,marker, node->children[i]);

		}
    }
}
//==========================================================================
bool Tree::isLastChild(Node *ptr)
{
	if(ptr!=root and ptr == ptr->parent->children[ptr->parent->children.size()-1])
		return true;
	return false;
}
//===========================================================================
//starter_code_ends
//==========================================================================
//Checks whether Node is a leaf
bool Tree::isExternal(Node* node)
{
	return (node -> children.empty()); //If children vector is empty, the node is external
}
//==========================================================================
//Checks whether Node is internal
bool Tree::isInternal(Node* node)
{
	return (!isExternal(node)); //If node is not external it is internal
}
//==========================================================================
//Returns size of tree/number of nodes
int Tree::size(Node* node)
{
		int t_size = 1; //Sets the initial size as 1
		if (!node->children.empty()){ //If node does not have children, goes to return statement
			for (int i = 0; i < node->children.size(); i++){ //iterates through children array
				t_size += size(node->children[i]); //recursively calls the size function and adds result to t_size
			}
		}
		return t_size; //returns number of nodes rooted at node. Returns 1 if children vector is empty.
}
//========================================================================
//Inserts a child into the node
void Tree::insert(Node* node, string child_name)
{
		Node* child = new Node(child_name); //creates a pointer pointing to a new node
		child->parent = node; //sets parent of new node as our selected node
		node -> children.push_back(child); //adds child node into our parent's children vector
}
//========================================================================
//Removes child from node
void Tree::remove(Node* node, string child_name)
{
		for(int i=0; i < node->children.size();i++){ //Iterates through children vector
			if (node->children[i]->getName() == child_name){ //searches for child with name
				delete node->children[i]; //deletes the child node
				node->children.erase(node->children.begin()+i); //erases the pointer pointing to the child in the children vector
				cout << child_name << " has been deleted successfully." << endl; //print statement
				return;
			}
		}
		throw logic_error("Child not found."); //throws logic error if child was not found
}
//==========================================================================
//Calculates depth of selected node
int Tree::depth(Node* node)
{	
	int depth = 0;
	while (node -> parent != nullptr){ //while loop stops when the node is the root
		depth++; //increases depth by 1
		node = node -> parent; //sets the parent as the node
	}
	return depth; //returns depth
}
//==========================================================================
//Calculates height of selected node
int Tree::height(Node* node)
{	
	int m_height = 0; 
	if (node->children.empty()) return m_height; //If children vector is empty, returns 0
	for (int i = 0; i<node->children.size(); i++){
			int c_height = height(node->children[i]); //recursively calls height function on each child
			if (c_height>m_height){ //checks if the height of the chil is greater than our current height int
				m_height = c_height; //If it is, sets our current height as the height of our child
			}
		}
	return m_height+1; //returns our height with one added to account for our current node
}
//==========================================================================
//Calculates total tree height
int Tree::treeHeight()
{
	return height(root); //calls height function on root
}
//==========================================================================
//Prints tree in preorder
void Tree::preorder(Node *node)
{	
	cout << node->getName(); //Print statement is encountered first
	for (int i = 0; i < node->children.size();i++){
		cout << " ";
		preorder(node -> children[i]); //and then recursively calls the preorder function
	}
}
//==========================================================================
//Prints tree in postorder
void Tree::postorder(Node *node)
{
	for (int i = 0; i < node->children.size();i++){
		postorder(node -> children[i]); //First it recursively calls postorder function
		cout<< " ";
	}
	cout << node->getName(); //and then prints the name
}
//==========================================================================
//destructor for node
Node::~Node()
{
	for (int i = 0; i < children.size(); i++){ //iterates through nodes children vector
		delete children[i]; //deletes the node at each index. Since each index is a node, it recursively calls destructor until children vector of each node is deleted.
	}
}
