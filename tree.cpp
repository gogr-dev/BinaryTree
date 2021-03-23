//-----------------------------------------------------------
//  Purpose:    Implementation of BinaryTree class.
//-----------------------------------------------------------
#include "tree.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

//-----------------------------------------------------------
// Constructor function.
//-----------------------------------------------------------
BinaryTree::BinaryTree()
{
   Root = NULL;
}

//-----------------------------------------------------------
// Constructor function that reads tree from file.
//-----------------------------------------------------------
BinaryTree::BinaryTree(char *Name)
{
   Root = NULL;

   // Open input file
   ifstream din;
   din.open(Name);
   if (din.fail())
      cout << "Error: could not open input file\n";

   // Read the data
    string Names;
    int Count;
    float Percentage;
    int Rank;
    
    din >> Names >> Count >> Percentage >> Rank;
   while (!din.eof())
   {
      InsertHelper(Names, Count, Percentage, Rank, Root);
      din >> Names >> Count >> Percentage >> Rank;
   }

   // Close the file
   din.close();
}

//-----------------------------------------------------------
// Copy constructor helper function.
//-----------------------------------------------------------
void BinaryTree::CopyHelper(Node * Tree1, Node * &Tree2)
{
   // Check terminating condition
   if (Tree1 == NULL)
      Tree2 = NULL;

   // Copy node and it's children
   else
   {
      Tree2 = new Node;
      Tree2->Name = Tree1->Name;
      Tree2->Count = Tree1->Count;
      CopyHelper(Tree1->Left, Tree2->Left);
      CopyHelper(Tree1->Right, Tree2->Right);
   }
}

//-----------------------------------------------------------
// Copy constructor.
//-----------------------------------------------------------
BinaryTree::BinaryTree(const BinaryTree & B)
{
   // Call tree copy function
   CopyHelper(B.Root, Root);
}


//-----------------------------------------------------------
// Destructor function helper function.
//-----------------------------------------------------------
void BinaryTree::DestroyHelper(Node * Tree)
{
   // Delete node and it's children
   if (Tree != NULL)
   {
      DestroyHelper(Tree->Left);
      DestroyHelper(Tree->Right);
      delete Tree;
   }
}

//-----------------------------------------------------------
// Destructor function.
//-----------------------------------------------------------
BinaryTree::~BinaryTree()
{
   // Call tree destroy function
   DestroyHelper(Root);
}

//-----------------------------------------------------------
// Search helper function.
//-----------------------------------------------------------
bool BinaryTree::SearchHelper(string name, int &count, float &percentage, int &rank, Node * &Tree)
{
   // Data value not found 
   if (Tree == NULL)
      return false;

   // Data value found 
   else if (Tree->Name == name)
   {
      count = Tree->Count;
      percentage = Tree->Percentage;
      rank = Tree->Rank;   
      return true;
   }
   
   // Recursively search for data value
   else if (Tree->Name > name)
      return (SearchHelper(name, count, percentage, rank, Tree->Left));
   else 
      return (SearchHelper(name, count, percentage, rank, Tree->Right));
}

//-----------------------------------------------------------
// Search for data in the binary tree.
//-----------------------------------------------------------
bool BinaryTree::Search(string name, int &count, float &percentage, int &rank)
{
   // Call tree searching function
   return (SearchHelper(name, count, percentage, rank, Root));
}

//-----------------------------------------------------------
// Insert helper function.
//-----------------------------------------------------------
bool BinaryTree::InsertHelper(string name, int count, float percentage, int rank, Node * &Tree)
{
   // Insert data into the tree
   if (Tree == NULL)
   {
      Tree = new Node;
      Tree->Name = name;
      Tree->Count = count;
      Tree->Percentage = percentage;
      Tree->Rank = rank;
      Tree->Left = NULL;
      Tree->Right = NULL;
      return true;
   }

   // Recursively search for insertion position
   else if (Tree->Name > name)
      return (InsertHelper(name, count, percentage, rank, Tree->Left));
   else 
      return (InsertHelper(name, count, percentage, rank, Tree->Right));
}

//-----------------------------------------------------------
// Insert data into the binary tree.
//-----------------------------------------------------------
bool BinaryTree::Insert(string name, int count, float percentage, int rank)
{
   // Call tree insertion function
   return (InsertHelper(name, count, percentage, rank, Root));
}

//-----------------------------------------------------------
// Delete a single node from the binary tree.
//-----------------------------------------------------------
bool BinaryTree::DeleteNode(Node * &Tree)
{
   Node *Temp = Tree;

   // Node has 0 children
   if ((Tree->Left == NULL) && (Tree->Right == NULL))
      Tree = NULL;

   // Node has 1 child
   else if (Tree->Left == NULL)
      Tree = Tree->Right;
   else if (Tree->Right == NULL)
      Tree = Tree->Left;


   // Node has 2 children
   else
   {
      // Find leftmost node in right subtree
      Node *Parent = Tree;
      Temp = Tree->Right;
      while (Temp->Left != NULL)
      {
	 Parent = Temp;
	 Temp = Parent->Left;
      }

      // Replace deleted data with leftmost value
      if (Parent == Tree)
	 Parent->Right = Temp->Right;
      else
	 Parent->Left = Temp->Right;
      Tree->Name = Temp->Name;
   }

   // Delete node
   delete Temp;
   return true;
}

//-----------------------------------------------------------
// Delete helper function.
//-----------------------------------------------------------
bool BinaryTree::DeleteHelper(string name, Node * &Tree)
{
   // Data value not found
   if (Tree == NULL)
      return false;

   // Data value found and deleted
   else if (Tree->Name == name)
      return (DeleteNode(Tree));

   // Recursively search for node to delete
   else if (Tree->Name > name)
      return (DeleteHelper(name, Tree->Left));
   else 
      return (DeleteHelper(name, Tree->Right));
}

//-----------------------------------------------------------
// Delete data from the binary tree.
//-----------------------------------------------------------
bool BinaryTree::Delete(string name)
{
   // Call tree deletion function
   return (DeleteHelper(name, Root));
}

//-----------------------------------------------------------
// Print helper function.
//-----------------------------------------------------------
void BinaryTree::PrintHelper(Node * Tree)
{
   // Check terminating condition
   if (Tree != NULL)
   {
      // Print left subtree
      PrintHelper(Tree->Left);

      // Print node value
      cout << Tree->Name << " " << Tree->Count << " " << Tree->Percentage << "% " << Tree->Rank << endl;

      // Print right subtree
      PrintHelper(Tree->Right);
   }
}

//-----------------------------------------------------------
// Print all records in the binary tree.
//-----------------------------------------------------------
void BinaryTree::Print()
{
   // Call tree printing function
   PrintHelper(Root);
   cout << endl;
}

//-----------------------------------------------------------
// Main program tests the BinaryTree class.
//-----------------------------------------------------------
int main()
{
   // Create tree
   BinaryTree Tree;

   string names;
   string name;
   int count;
   float percentage;
   int rank;
   
   // Read information from file
   ifstream din;
   din.open("male.txt");
   if (din.fail())
   {
      cout << "Could not open male.txt" << "\n";
   }
   
   // Read names into BST
   din >> name >> count >> percentage >> rank;
   while (!din.eof())
   {
      Tree.Insert(name, count, percentage, rank);
      din >> name >> count >> percentage >> rank;
   }
   
   // Close the file
   din.close();
 
   cout << "Top 1000 names program" << endl;
   
   // Interface
   while (names != "QUIT")
   {
        cout << "Enter a name: " << endl;
        cout << "To exit, enter QUIT" << endl;
        cin >> names;
        
        if (Tree.Search(names, count, percentage, rank))
        {
            cout << "Name: " << names << endl;
            cout << "Count: " << count << endl;
            cout << "Percentage: " << percentage << "%" << endl;
            cout << "Rank: " << rank << endl;
            cout << endl;
        }
        else
        {
	      cout << "Name not found" << endl;
	      cout << endl;
        }
   }
   
   Tree.Print();

   return 0;
}
