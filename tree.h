//-----------------------------------------------------------
//  Purpose:    Header file for the BinaryTree class.
//-----------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//-----------------------------------------------------------
// BinaryTree data node definition
//-----------------------------------------------------------
class Node
{
 public:
   string Name;
   int Count;
   float Percentage;
   int Rank;
   Node *Left;
   Node *Right;
};

//-----------------------------------------------------------
// Define the BinaryTree class interface 
//-----------------------------------------------------------
class BinaryTree
{
 public:
   // Constructor functions
   BinaryTree();
   BinaryTree(char *Name);
   BinaryTree(const BinaryTree & tree);
  ~BinaryTree();

   // General binary tree operations
   void Read(string name);
   bool Search(string name, int &count, float &percent, int &rank);
   bool Insert(string name, int count, float percent, int rank);
   bool Delete(string name);
   void Print();
   void Print(ofstream & dout);

 private:
   // Helper functions
   void CopyHelper(Node * Tree1, Node * &Tree2);
   void DestroyHelper(Node * Tree);
   bool SearchHelper(string name, int &count, float &percent, int &rank, Node * &Tree);
   bool InsertHelper(string name, int count, float percent, int rank, Node * &Tree);
   bool DeleteNode(Node * &Tree);
   bool DeleteHelper(string name, Node * &Tree);
   void PrintHelper(Node * Tree);
   void PrintHelper(Node * Tree, ofstream & dout);

   // Tree pointer
   Node *Root;
};