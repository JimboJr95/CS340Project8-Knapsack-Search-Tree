//
//  Author:     James Callahan
//  Class:      CS 340
//  Assignment: Project 8
//  Date:       7 December 2017
//  File:       KnapsackSearchTree.h
//

#pragma once

#include<list>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<assert.h>

using namespace std;

typedef double type; // used for weight, value and value to weight ratio

static const int NOT_SELECTED = -999; // For items not selected
static const int SELECTED = 999;


// Node for items in the tree
struct node{
    int index; // The level of the tree that the node is on (root is 0)
    type upperBound; // Largest possible value
    type currentWeight; // Weight held in the knapsack currently
    type currentValue; // Value held in the knapsack currently
    
    node *left; // Left child pointer
    node *right; // Right child pointer
    node *parent; // Parent pointer
    
    bool taken; // Boolean to indicate if the item was taken->true (left child) or not (right child)
};


// Class for Knapsack using a search tree
class KnapsackSearchTree{
public:
    // Constructor
    KnapsackSearchTree();
    
    // Destructor
    ~KnapsackSearchTree();
    
    // Functions
    void run();
    
private:
    // Variables
    ifstream infile; // Input file
    
    node *root; // Root node
    node *current; // Will use for the current node we are at
    
    int W; // Weight bound
    int numValues; // how many items are available for taking
    
    bool continueRunning; // Set to false when we have found a solution
    
    type *values; // Values in order
    type *weights; // Weights in order
    type *valueWeightRatio; // value/weight
    list<node*> frontier; // Hold nodes in the frontier
    
    
    // Functions
    void openFile(string filename, ifstream &in); // Opens file or asserts if there was an error
    void inputs(); // Gets all input from the user
    bool validFileSelection(int num); // Ensueres the selected file is a correct value
    void getFileData(); // Gets the data from the selected text file and pouplates various data members
    void runSearchTree(); // Goes through the process of creating the tree, structuring it as we go
    void addToFrontier(node *n); // Adds a node to the frontier list
    void printKnapsack(); // Prints out the soultion for the knapsack problem
    void outputFrontierList(); // Outputs the frontier list (for testing)
    node *initializeNode(int index); // Initializes the items of a node
    
};
