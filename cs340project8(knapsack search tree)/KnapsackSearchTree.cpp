//
//  Author:     James Callahan
//  Class:      CS 340
//  Assignment: Project 8
//  Date:       7 December 2017
//  File:       KnapsackSearchTree.cpp
//

#include "KnapsackSearchTree.h"
#include<iostream>

using namespace std;

// Constructor
KnapsackSearchTree::KnapsackSearchTree(){
    // Initialize any values needed
    continueRunning = true;
}


// Destructor
KnapsackSearchTree::~KnapsackSearchTree(){
    node *del;
    
    // Delete weights and values
    delete []weights;
    delete []values;
    delete []valueWeightRatio;
    
    // Get rid of frontier list
    while (!frontier.empty()) {
        frontier.pop_front();
    }
    
    // Delete search tree
    del = root;
    current = root;
    
    while (root != NULL) {
        // If left child not null
        if(del->left != NULL){
            current = del;
            del = del->left;
        }
        // Else if right child not null
        else if(del->right != NULL){
            current = del;
            del = del->right;
        }
        // Delete leaf
        else{
            delete del;
            del = current;
        }
    }
    
    // Delete node pointers
    delete root;
    delete current;
}


// Initialize a new node
node *KnapsackSearchTree::initializeNode(int index){
    node *n = new node;
    
    n->taken = false;
    
    n->index = index;
    n->upperBound = 0;
    n->currentWeight = 0;
    n->currentValue = 0;
    
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    
    return n;
}


// Run the knapsack search tree
void KnapsackSearchTree::run(){
    
    // Get input(s) from the user
    inputs();
    
    // Get data from the input file
    getFileData();
    
    // Initialize root pointer
    root = initializeNode(0);
    
    // Goes through search tree
    runSearchTree();
    
    // Prints out which items were taken and info about the knapsack
    printKnapsack();
    
    // Close input file when we are done
    infile.close();
}


// Get inputs needed from the user
void KnapsackSearchTree::inputs(){
    bool validSelection;
    int file;
    
    // Find which file they want to use for the knapsack problem
    do{
        // Get sequence alignment selection
        cout << "Which file would you like to solve knapsack on?\n";
        cout << "1: knapsack1\n";
        cout << "2: knapsack2\n";
        cout << "3: knapsack3\n";
        cout << "4: knapsack4\n";
        cout << "5: knapsack5\n";
        cout << "6: knapsack6\n";
        cout << "Selection: ";
        cin >> file;
        
        // Get whether the selected file is a valid one
        validSelection = validFileSelection(file);
        
        // Output if an incorrect number was entered
        if (!validSelection) {
            cout << "Enter a number in the range 1-6.\n";
        }
        
    }while(!validSelection);
    
    // Open selected file
    openFile("knapsack" + to_string(file) + ".txt", infile);
}


// Validates that the selected sequence alignment is a valid one
bool KnapsackSearchTree::validFileSelection(int num){
    bool val;
    
    if ( (num <= 0) || (num >= 7) ) {
        val = false;
    }
    else{
        val = true;
    }
    
    return val;
}


// Gets data from the input file
void KnapsackSearchTree::getFileData(){
    
    // Get number of values and upper weight bound
    infile >> numValues;
    infile >> W;
    
    // Initialize weight and value arrays
    weights = new type[numValues];
    values = new type[numValues];
    valueWeightRatio = new type[numValues];
    
    // Get the value and weight of each item
    for (int i = numValues - 1; i >= 0; i--) {
        infile >> weights[i];
        infile >> values[i];
        
        valueWeightRatio[i] = values[i]/weights[i];
    }
    
    // Output data (it would look wierd if I displayed it in the order I initialized it)
    cout << "\nUpper weight bound (W): " << W << endl;
    
    for (int i = 0; i < numValues; i++) {
        cout << "\nItem " << (numValues - i) << ":\tweight=" << weights[i] << "\tvalue=" << values[i] << "\tvalue/weight=" << valueWeightRatio[i];
    }
    
    cout << endl;
}


// Creates the search tree and runs through knapsack
void KnapsackSearchTree::runSearchTree(){
    // Node pointer used for various comparisons throughout the function
    node *tmp;
    
    // Initialize root node values
    root->upperBound = W * valueWeightRatio[numValues - 1];
    
    // Starting out at the root (current used to create and move down tree
    current = root;
    
    // While we have not found a solution
    while (continueRunning) {
    
        // Make branches
        current->left = initializeNode(current->index + 1);
        current->right = initializeNode(current->index + 1);
        
        // Set parents of new branches to current node
        current->left->parent = current;
        current->right->parent = current;

        
        // Calculation of upper bound (and other values):
        
        // Left branch (with item) [check to see if it exceeds weight limit]
        if( (current->currentWeight + weights[numValues - current->index - 1]) < W ){
            current->left->currentValue = current->currentValue + values[numValues - current->index - 1];
            current->left->currentWeight = current->currentWeight + weights[numValues - current->index - 1];
            
            // If this isn't the last value (not possible solution)
            if(current->left->index != numValues){
            current->left->upperBound = current->left->currentValue + ( (W - current->left->currentWeight) * valueWeightRatio[numValues - current->index - 2]);
            }
            // Last value (a possible solution)
            else{
                current->left->upperBound = current->currentValue + values[numValues - current->index];
            }
        }
        // If adding that item would exceed the weight bound
        else{
            current->left->currentValue = NOT_SELECTED;
            current->left->currentWeight = NOT_SELECTED;
            current->left->upperBound = NOT_SELECTED;
        }
        
        
        // Right branch (without item)
        current->right->currentValue = current->currentValue;
        current->right->currentWeight = current->currentWeight;
        
        // If this isn't the last value (not possible solution)
        if (current->right->index != numValues) {
            current->right->upperBound = current->right->currentValue + ( (W - current->right->currentWeight) * valueWeightRatio[numValues - current->index - 2]);
        }
        // Last value (a possible solution)
        else{
            current->right->upperBound = current->currentValue;
        }
        
        
        // Indicate which item is taken and which isn't
        current->left->taken = true;
        current->right->taken =  false;
        
        
        
        // Add to frontier - Adds them in increasing order (most promising always at the front)
        //
        // If the frontier list is currently empty
        if (frontier.empty()) {
            frontier.emplace_front(current->left); // place left leaf on the front
            
            // Place right leaf in correct position
            for (list<node*>::iterator i = frontier.begin(); i != frontier.end(); i++) {
                tmp = *i;
                if ((tmp->upperBound < current->right->upperBound) ) {
                    frontier.insert(i, current->right);
                    break;
                }
            }
        }
        // If this isn't the first addition to the frontier list
        else{
            // Add left and right child
            addToFrontier(current->left);
            addToFrontier(current->right);
        }
        
        
        
        // Best First Visitations (Best-first visitations) :
        //
        // Gets the next most promising item (frontier already has largest value in front)
        current = frontier.front();
        frontier.pop_front();
        
        

        // Pruning :
        //
        //            My method of pruning is a little different from how we did it in class.
        //            Since my list is created with values in increasing order, we will know right away if
        //              there will be an upper bound greater than the solution currrently found by looking at
        //              the front of the frontier list.
        //
        
        // If we have reached a possible solution
        if ( current->index == (numValues) ) {
            tmp = frontier.front();
            
            // Backtracking :
            //
            // The next greatest value will be at the front of the list, so if that item's upper bound is
            //  greater than the current solution, we set that as our current point and keep going.
            if (tmp->upperBound > current->upperBound) {
                continueRunning = true;
                current = frontier.front();
                frontier.pop_front();
            }
            
            // If this is the best solution, we will leave the while loop by setting the continueRunning bool to false
            else{
                continueRunning = false;
            }
        }
    
    }
    
}


// Adds a node to the frontier list
void KnapsackSearchTree::addToFrontier(node *n){
    // Adds them in increasing order (most promising always at the front)

    // i used to iterate through the frontier list, temp used to compare leaf with item in the list
    list<node*>::iterator i = frontier.begin();
    node *tmp = *i;
    
    // Add child
    for (i = frontier.begin(); i!= frontier.end(); i++) {
        tmp = *i;
        
        // If the upper bound of the item in the list is less than the upper bound of the child
        if ( (tmp->upperBound < n->upperBound) ) {
            frontier.insert(i, n);
            break;
        }
    }
    // If the item needs to go on the end of the list
    if (i == frontier.end()) {
        frontier.emplace_back(n);
    }
}


// Ensures that the file opens, or assert
void KnapsackSearchTree::openFile(string filename, ifstream &in){
    in.open(filename);
    
    if(in.fail()){
        cout << "Error opening file " << filename << "\n";
        assert(!in.fail());
    }
}


// Prints out the end state of the knapsack problem
void KnapsackSearchTree::printKnapsack(){
    node *temp = current;
    
    cout << "\nWeght of items taken: " << current->currentWeight << "\nValue taken: " << current->currentValue << "\nItems taken:";
    
    // While we haven't reached the root
    while (temp != root) {
        // If it is a left child (taken), then output that it was taken.
        if (temp->taken) {
            cout << " " << temp->index;
        }
        temp = temp->parent; // moving up the tree
    }
    
    cout << endl;
}


// Outputs the frontier list (testing)
void KnapsackSearchTree::outputFrontierList(){
    node *tmp; // for going through list
    
    // Outputs frontier list
    cout << "\nFrontier list:\n";
    for (list<node*>::iterator i = frontier.begin(); i != frontier.end(); i++) {
        tmp = *i;
        cout << "curVal: " << tmp->currentValue << "\tcurWeight: " << tmp->currentWeight << "\tub: " << tmp->upperBound << "\n";
    }

    cout << "current info: val: " << current->currentValue << "\tweight: " << current->currentWeight << "\tub: " << current->upperBound << endl;
}
