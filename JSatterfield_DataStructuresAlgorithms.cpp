//============================================================================
// Name        : JSatterfield_DataStructuresAlgorithms.cpp
// Author      : John Satterfield
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
//============================================================================

#include <algorithm>
#include <climits> // UINT_MAX
#include <iostream>
#include <string> // atoi
#include <time.h> // clock_t, clock(), CLOCK_PER_SECONDS

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// Forward declarations
double strToDouble(string str, char ch);

// Define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// Internal structure for tree node
struct Node {
	Bid bid;

	Node* left;
	Node* right;
	Node* cur;

	Node() {
		left = NULL;
		right = NULL;
		cur = NULL;
	}

	Node(Bid bid): Node() {
		this->bid = bid;
	}

};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
	root = NULL;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	this->inOrder(root);
}

/**
 * BST: Remove Node
 */
Node* BinarySearchTree::removeNode(Node *node, string bidId) {
	// If node does not exist
	if (node == NULL) {
		return node;
	}
	// Search left subtree
	if (bidId.compare(node->bid.bidId) < 0) {
		node->left = removeNode(node->left, bidId);
	}
	// Search right subtree
	else if (bidId.compare(node->bid.bidId) > 0) {
		node->right = removeNode(node->right, bidId);
	}
	else {
		// If no children - delete node
		if (node->left == NULL && node->right == NULL) {
			delete node;
			node = NULL;
		}
		// Remove node if only left child
		else if (node->left != NULL && node->right == NULL) {
			Node* tempNode = node;
			node = node->left;
			delete tempNode;
		}
		// Remove node if only right child
		else if (node->left == NULL && node->right != NULL) {
			Node* tempNode = node;
			node = node->right;
			delete tempNode;
		}
		else { // Remove node with two children
			Node* suc = node->right;
			// Find successor (leftmost child of right subtree)
			while (suc->left == NULL) {
				suc = suc->left;
			}
			node->bid = suc->bid; // Copy successor to current node
			node->right = removeNode(node->right, suc->bid.bidId); // Remove successor from right subtree
		}
	}

	return node;
}

/**
 * BST: Insert Bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // Implement inserting a bid into the tree

	// If no root exists
	if (root == NULL) {
	    root = new Node(bid);
	}
	// Call addNode to traverse and add bid
	else {
	    this->addNode(root, bid);
	}
}

/**
 * BST: Remove Bid
 */
void BinarySearchTree::Remove(string bidId) {
    // Implement removing a bid from the tree
	this->removeNode(root, bidId);
}

/**
 * BST: Search Bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // Implement searching the tree for a bid
	Node* cur = root;

	while (cur != NULL) {
		if (cur->bid.bidId.compare(bidId) == 0) {
			return cur->bid; // bid found
		}
		// If bidId is smaller - look left
		else if (bidId.compare(cur->bid.bidId) < 0) {
			cur = cur->left;
		}
		// Else bidId is larger - look right
		else {
			cur = cur->right;
		}
	}

	Bid bid;
    return bid;
}

/**
 * BST: Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    // Implement inserting a bid into the tree

	// If larger - add to left subtree
	if (node->bid.bidId.compare(bid.bidId) > 0) {
		if (node->left == NULL) {
			node->left = new Node(bid);
		}
		else {
			this->addNode(node->left, bid);
		}
	}
	// Else - add to right subtree
	else {
		if (node->right == NULL) {
			node->right = new Node(bid);
		}
		else {
			this->addNode(node->right, bid);
		}
	}
}

void BinarySearchTree::inOrder(Node* node) {
	if (node != NULL) {
		inOrder(node->left);
		cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "<< node->bid.fund << endl;
		inOrder(node->right);
	}
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void BSTloadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    struct Node {
    	Bid bid;
    	Node* next;

    	Node() {
    		next = NULL;
    	}

    	Node(Bid bidItem) {
    		bid = bidItem;
    		next = NULL;
    	}
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // Initialize housekeeping variables
	head = NULL;
	tail = NULL;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
	Node* newNode = new Node(bid);

	// Assign newNode to head if no other nodes exist
	if (head == NULL) {
		head = newNode;
	}
	else {
		// Move to last node
		Node* curNode = head;
		while (curNode->next != NULL) {
			curNode = curNode->next;
		}
		// Link after node
		curNode->next = newNode;
	}
	// Increment size of list
	size++;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
	Node* newNode = new Node(bid);

	if (head != NULL) {
		newNode->next = head;
	}
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
	// Set curNode to head
	Node* curNode = head;

	// Loop until end if list
	while (curNode != NULL) {

		// Display as "bid title | bid amound | bid fund"
		cout << curNode->bid.title << " | " << curNode->bid.amount
			 << " | " << curNode->bid.fund << endl;

		// Change curNode to next list item
		curNode = curNode->next;
	}
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
	// Check that items exist in list
	if (head != NULL) {
		// If first item matches bidId
		if (head->bid.bidId == bidId) {
			// Delete head item and assign to end
			delete head;
			head = NULL;
		}
	}

	// Assign curNode to head
	Node* curNode = head;

	// While curNode is not at the end
	while(curNode->next != NULL) {
		// Check if curNode matches bidId
		if (curNode->next->bid.bidId == bidId) {
			// Create instance of sucNode and assign two values from curNode
			Node* sucNode = curNode->next->next;
			// Assign next value to sucNode
			curNode->next = sucNode;
			// Decrement list size
			size--;
			return;
		}
		// Change curNode to next list item
		curNode = curNode->next;
	}
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
    // Implement search logic

	// Assign curNode to head
	Node* curNode = head;

	// While not at the end of list
	while (curNode != NULL) {
		// If curNode matches bidId
		if (curNode->bid.bidId == bidId) {
			return curNode->bid;
		}
		// Advance curNode to next node
		curNode = curNode->next;
	}
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

void linkedListLoadBids(string csvPath, LinkedList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}


//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
	struct Node {
	    Bid bid;
	    Node* next;
	    unsigned key;

	    Node() {
	    	key = UINT_MAX;
	   		next = NULL;
	   	}

	   	Node(Bid bidItem) : Node() {
	   		bid = bidItem;
	   	}

	   	Node(Bid bidItem, unsigned keyItem) : Node(bidItem) {
	   		key = keyItem;
	   		bid = bidItem;
	   	}
	};

	std::vector<Node> nodes;
	unsigned tableSize = DEFAULT_SIZE;
    unsigned int hash(int key);

public:
    HashTable();
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // Initialize the structures used to hold bids
	nodes.resize(tableSize);
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    // Implement logic to free storage when class is destroyed
	nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // Implement logic to calculate a hash value
	return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
	// Convert bidId from string to int
	unsigned key = hash(atoi(bid.bidId.c_str()));

	// Get node using key
	Node* prev = &(nodes.at(key));

	// Key address is blank
	if (prev == NULL) {
		Node* newNode = new Node(bid, key);
		nodes.insert(nodes.begin()+key, *newNode);
	}
	else {
		// Node found and key address is empty
		if (prev->key == UINT_MAX) {
			prev->key = key;
			prev->bid = bid;
			prev->next = NULL;
		}
		else {
			// Locate next open node
			while (prev->next != NULL) {
				prev = prev->next;
			}
			prev->next = new Node(bid, key);
		}
	}
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
	//HashTable bidTable;
	//Bid bid;

	// Iterate through hash table
	for (unsigned i=0; i < nodes.size(); i++) {

		// Create pointer to display each value
		Node* node = &(nodes.at(i));

		// If node is not empty
		if (node != NULL && node->bid.amount != 0) {
			cout << "Key " << node->key << ": " << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "
					<< node->bid.fund << endl;
			// Loop while node has more than 1 value
			while (node->key != UINT_MAX && node->next != NULL) {
				// Set pointer to next value
				node = node->next;
				cout << "    " << node->key << ": " << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "
									<< node->bid.fund << endl;
			}
		}
	}
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
	HashTable bidTable;

	// Convert bidID from string to int
	unsigned key = hash(atoi(bidId.c_str()));

	// Locate node at key address
	Node* node = &(nodes.at(key));

	if (node == NULL || node->key != key) {
		cout << "No element found at key " << key << endl;
		return;
	}
	else if (node->next != NULL) {
		Node* temp = node;
		node = node->next;
		delete temp;
	}
	else {
		nodes.erase(nodes.begin()+key);
	}
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // Convert bidId from string to int
    unsigned key = hash(atoi(bidId.c_str()));

    // Get node by key
    Node* node = &(nodes.at(key));

    // If node is found and bidID matches
    if (node != NULL && node->key != UINT_MAX && node->bid.bidId.compare(bidId)==0) {
    	return node->bid;
    }
    // Node not found
    else if (node == NULL || node->key == UINT_MAX) {
    	return bid;
    }

    // Iterate chained links
    while (node != NULL) {
    	if (node->key != UINT_MAX && node->bid.bidId.compare(bidId)==0) {
    		return node->bid;
    	}
    	node = node->next;
    }
    return bid;
}

void hashTableLoadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}


//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.title << " | " << bid.amount << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
	// Define a vector data structure to hold a collection of bids.
	vector<Bid> bidItem;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

	// loop to read rows of a CSV file
	for (int i = 0; i < file.rowCount(); i++) {
        // Data structure to hold data from each row and add to vector
		Bid bid;
		bid.title = file[i][0]; // first column
		bid.fund = file[i][8]; // ninth column
		bid.amount = strToDouble(file[i][4], '$'); // fifth column
		bidItem.push_back(bid); // add bidItem to last value
    }

	cout << file.rowCount() << " bids read" << endl;

    return bidItem;
}

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
	int l = begin;;
	int h = end;
	int pivot;
	//int temp = 0;
	bool done = false;

	// Pick middle element as pivot
	pivot = begin + (end-begin)/2;

	while (!done) {
		/* Increment l while bids[l] < pivot */
		while (bids.at(l).title < bids.at(pivot).title) {
			++l;
		}

	    /* Decrement h while pivot < bids[h] */
		while (bids.at(pivot).title < bids.at(h).title) {
			--h;
		}

	    /* If there are zero or one elements remaining,
	       all numbers are partitioned. Return h */
	    if (l >= h) {
	        done = true;
	    }
	    else {
	        /* Swap bids[l] and bids[h],
	           update l and h */
	    	//temp = bids.at(l);
	    	//bids.at(l) = bids.at(h);
	    	//bids.at(h) = temp;
	    	swap(bids.at(l), bids.at(h));
	        ++l;
	        --h;
	    }
	 }
	 return h;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
	int midpoint = 0;

	/* Base case: If there are 1 or zero elements to sort,
	   partition is already sorted */
	if (begin >= end) {
		return;
	}

	/* Partition the data within the array. Value middle returned
	   from partitioning is location of last element in low partition. */
	midpoint = partition(bids, begin, end);

	/* Recursively sort low partition (beginning to middle) and
	   high partition (middle + 1 to end) */
	quickSort(bids, begin, midpoint);
	quickSort(bids, midpoint + 1, end);

	return;
}

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
	int indexSmallest = 0;
	//int temp;

	for (unsigned i = 0; i < bids.size() - 1; ++i) {

		// Find index of smallest remaining element
	    indexSmallest = i;

	    for (unsigned j = i + 1; j < bids.size(); ++j) {

	    	if (bids.at(j).title < bids.at(indexSmallest).title) {
	    		indexSmallest = j;
	    	}
	    }

	    // Swap bids[i] and bids[indexSmallest]
	    //temp = bids.at(i);
	    //bids.at(i) = bids.at(indexSmallest);
	    //bids.at(indexSmallest) = temp;
	    swap(bids.at(i), bids.at(indexSmallest));
	}
}


/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

	// Declare instance of data structure to hold bid information
	Bid item;

	// process command line arguments
	string csvPath, bidKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		bidKey = "98109";
		break;
	case 3:
		csvPath = argv[1];
		bidKey = argv[2];
		break;
	default:
		csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
		bidKey = "98109";
	}

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;

    // Define a linked list to hold all bids
    LinkedList bidList;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 10) {
    	cout << "Menu:" << endl;
    	cout << "  1. Use Binary Search Tree" << endl;
    	cout << "  2. Use Linked List" << endl;
    	cout << "  3. Use Hash Table" << endl;
    	cout << "  4. Use Vector Sorting" << endl;
    	cout << "  5. Use Selection Sorting" << endl;
    	cout << "  6. Use Quick Sorting" << endl;
    	cout << "  9. Exit" << endl;
    	cout << "Enter choice: ";
    	cin >> choice;

    	switch (choice) {
    	// Binary Search Tree Menu
    	case 1:
    	{
    		int bstchoice = 0;
    		while (bstchoice != 9) {
    			cout << "Binary Search Tree Menu:" << endl;
    			cout << "  1. Load Bids" << endl;
    			cout << "  2. List all Bids" << endl;
    			cout << "  3. Add a Bid" << endl;
    			cout << "  4. Find a Bid" << endl;
    			cout << "  5. Remove a Bid" << endl;
    			cout << "  9. Return to previous menu" << endl;
    			cout << "Enter choice: ";
    			cin >> bstchoice;
    		}

    		if (bstchoice == 1 ) {
    			// BST load bids
    			bst = new BinarySearchTree();

    			// Initialize a timer variable before loading bids
    			ticks = clock();

    			// Complete the method call to load the bids
    			BSTloadBids(csvPath, bst);

    			//cout << bst->Size() << " bids read" << endl;

    			// Calculate elapsed time and display result
    			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
    			cout << "time: " << ticks << " clock ticks" << endl;
    			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
    		}
    		if (bstchoice == 2) {
    			// BST list all bids
    			bst->InOrder();
    		}
    		if (bstchoice == 3) {
    			// BST add bid
    			bid = getBid();
    			bst->Insert(bid);
    			displayBid(bid);
    		}
    		if (bstchoice == 4) {
    			// BST find bid
    			ticks = clock();

    			bid = bst->Search(bidKey);

    			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

    			if (!bid.bidId.empty()) {
    				displayBid(bid);
    			} else {
    				cout << "Bid Id " << bidKey << " not found." << endl;
    			}

    			cout << "time: " << ticks << " clock ticks" << endl;
    			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
    		}
    		if (bstchoice == 5) {
    			// BST remove bid
    			bst->Remove(bidKey);
    		}
    		if (bstchoice == 9) {
    			return 0;
    		}
    		else {
    			cout << "Please enter valid option." << endl;
    		}
    	}
    	break;

    	// Linked List Menu
    	case 2:
    	{
    		int linkchoice = 0;
    		while (linkchoice != 9) {
    			cout << "Linked List Menu:" << endl;
    			cout << "  1. Load Bids" << endl;
    			cout << "  2. List all Bids" << endl;
    			cout << "  3. Add a Bid" << endl;
    			cout << "  4. Find a Bid" << endl;
    			cout << "  5. Remove a Bid" << endl;
    			cout << "  9. Return to previous menu" << endl;
    			cout << "Enter choice: ";
    			cin >> linkchoice;
    		}
    		if (linkchoice == 1 ) {
    			// Linked List load bids
    			ticks = clock();

    			linkedListLoadBids(csvPath, &bidList);

    			cout << bidList.Size() << " bids read" << endl;

    			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
    			cout << "time: " << ticks << " milliseconds" << endl;
    			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
    		}
    		else if (linkchoice == 2) {
    			// Linked List list all bids
    			bidList.PrintList();
    		}
    		else if (linkchoice == 3) {
    			// Linked List enter bid
    			bid = getBid();
    			bidList.Append(bid);
    			displayBid(bid);
    		}
    		else if (linkchoice == 4) {
    			// Linked List find bid
    			ticks = clock();

    			bid = bidList.Search(bidKey);

    			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

    			if (!bid.bidId.empty()) {
    				displayBid(bid);
    			} else {
    				cout << "Bid Id " << bidKey << " not found." << endl;
    			}

    			cout << "time: " << ticks << " clock ticks" << endl;
    			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
    		}
    		else if (linkchoice == 5) {
    			// Linked List remove bid
    			bidList.Remove(bidKey);
    		}
    		else if (linkchoice == 9) {
    			return 0;
    		}
    		else {
    			cout << "Please enter valid option." << endl;
    		}
    	}
    	break;

    	// Hash Table Menu
    	case 3:
    	{
    		int hashchoice = 0;
    		while (hashchoice != 9) {
    			cout << "Hash Table Menu:" << endl;
    			cout << "  1. Load Bids" << endl;
    			cout << "  2. List all Bids" << endl;
    			cout << "  3. Add a Bid" << endl;
    			cout << "  4. Find a Bid" << endl;
    			cout << "  5. Remove a Bid" << endl;
    			cout << "  9. Return to previous menu" << endl;
    			cout << "Enter choice: ";
    			cin >> hashchoice;
    		}
    		if (hashchoice == 1 ) {
    			// Hash Table load bids
    			bidTable = new HashTable();

    			// Initialize a timer variable before loading bids
    			ticks = clock();

    			// Complete the method call to load the bids
    			hashTableLoadBids(csvPath, bidTable);

    			// Display bid count
    			//cout << bidTable.size() << " bids read" << endl;

    			// Calculate elapsed time and display result
    			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
    			cout << "time: " << ticks << " clock ticks" << endl;
    			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
    		}
    		else if (hashchoice == 2) {
    			// Hash Table list all bids
    			bidTable->PrintAll();
    		}
    		else if (hashchoice == 3) {
    			// Hash Table enter bid
    			bid = getBid();
    			bidTable->Insert(bid);
    			displayBid(bid);
    		}
    		else if (hashchoice == 4) {
    			// Hash Table find bid
    			ticks = clock();

    			bid = bidTable->Search(bidKey);

    			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

    			if (!bid.bidId.empty()) {
    				displayBid(bid);
    			} else {
    				cout << "Bid Id " << bidKey << " not found." << endl;
    			}

    			cout << "time: " << ticks << " clock ticks" << endl;
    			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
    		}
    		else if (hashchoice == 5) {
    			// Hash Table remove bid
    			bidTable->Remove(bidKey);
    		}
    		else if (hashchoice == 9) {
    			return 0;
    		}
    		else {
    			cout << "Please enter valid option." << endl;
    		}
    	}
    	break;

    	// Vector Sorting Menu
    	case 4:
    	{
    		int vectchoice = 0;
    		while (vectchoice != 9) {
    			cout << "Vector Sorting Menu:" << endl;
    			cout << "  1. Load Bids" << endl;
    			cout << "  2. List all Bids" << endl;
    			cout << "  9. Return to previous menu" << endl;
    			cout << "Enter choice: ";
    			cin >> vectchoice;
    		}
    		// Vector Sorting load bids
    		if (vectchoice == 1 ) {
    			// Initialize a timer variable before loading bids
    			ticks = clock();

    			// Complete the method call to load the bids
    			bids = loadBids(csvPath);
    			cout << bids.size() << " bids read" << endl;

    			// Calculate elapsed time and display result
    			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
    			cout << "time: " << ticks << " clock ticks" << endl;
    			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
    		}
    		// Vector Sorting list all bids
    		else if (vectchoice == 2) {
    			// Loop and display the bids read
    			for (unsigned int i = 0; i < bids.size(); ++i) {
    				displayBid(bids[i]);
    			}
    			cout << endl;
    		}
    		// Return to main menu
    		else if (vectchoice == 9) {
    			return 0;
    		}
    		// Invalid entry
    		else {
    			cout << "Please enter valid option." << endl;
    		}
    	}
    	break;

    	// Selection Sorting Menu
    	case 5:
    	{
    		int selectchoice = 0;
    		while (selectchoice != 9) {
    			cout << "Selection Sorting Menu:" << endl;
    			cout << "  1. Load Bids" << endl;
    			cout << "  2. List all Bids" << endl;
    			cout << "  9. Return to previous menu" << endl;
    			cout << "Enter choice: ";
    			cin >> selectchoice;
    		}
    		// Selection Sorting load bids
    		if (selectchoice == 1 ) {
    			// Initialize a timer variable before loading bids
    			ticks = clock();

    			// Complete the method call to load the bids
    			selectionSort(bids);

    			cout << bids.size() << " bids read" << endl;

    			// Calculate elapsed time and display result
    			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
    			cout << "time: " << ticks << " clock ticks" << endl;
    			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
    		}
    		// Selection Sorting list all bids
    		else if (selectchoice == 2) {
    			// Loop and display the bids read
    			for (unsigned int i = 0; i < bids.size(); ++i) {
    				displayBid(bids[i]);
    			}
    			cout << endl;
    		}
    		// Return to main menu
    		else if (selectchoice == 9) {
    			return 0;
    		}
    		// Invalid entry
    		else {
    			cout << "Please enter valid option." << endl;
    		}
    	}
    	break;

    	// Quick Sorting Menu
    	case 6:
    	{
    		int quickchoice = 0;
    		while (quickchoice != 9) {
    			cout << "Quick Sorting Menu:" << endl;
    			cout << "  1. Load Bids" << endl;
    			cout << "  2. List all Bids" << endl;
    			cout << "  9. Return to previous menu" << endl;
    			cout << "Enter choice: ";
    			cin >> quickchoice;
    		}
    		// Quick Sorting load bids
    		if (quickchoice == 1 ) {
    			// Initialize a timer variable before loading bids
    			ticks = clock();

    			// Complete the method call to load the bids
    			selectionSort(bids);

    			cout << bids.size() << " bids read" << endl;

    			// Calculate elapsed time and display result
    			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
    			cout << "time: " << ticks << " clock ticks" << endl;
    			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
    		}
    		// Quick Sorting list all bids
    		else if (quickchoice == 2) {
    			// Loop and display the bids read
    			for (unsigned int i = 0; i < bids.size(); ++i) {
    				displayBid(bids[i]);
    			}
    			cout << endl;
    		}
    		// Return to main menu
    		else if (quickchoice == 9) {
    			return 0;
    		}
    		// Invalid entry
    		else {
    			cout << "Please enter valid option." << endl;
    		}
    	}
    	break;

    	// Exit
    	case 9:
    		break;

    	default:
    		cout << "Please enter valid option." << endl;
    	} // End Switch
    } // Ent While Loop

    cout << "Good bye." << endl;

    return 0;
}

