//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Geren Rosenlund
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 5-2 Binary Search Tree
//============================================================================

#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <vector>

#include "CSVparser.hpp"

// All credit for sqlite3 here: https://www.sqlite.org/index.html
#include "sqlite3.h"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
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

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Bid aBid) :
        Node() {
        bid = aBid;
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
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void destroyNode(Node* node);
    void Clear();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void reverseInOrder();
    void reverseInOrder(Node* node);
    void collectBids(Node* node, vector<Bid>& bids);
    void Insert(Bid bid);
    void Remove(string bidId);
    void sortDescending();
    void dumpBidsToDatabase(BinarySearchTree* bst, const string& dbFile);
    Bid Search(string bidId);
};

// Default Constructor
BinarySearchTree::BinarySearchTree() {


    // My Stuff
    root = nullptr;
}

// Updated: New algorithm to help remove entries in the list and remove child entries
void BinarySearchTree::destroyNode(Node* node)
{
    if (node != nullptr)
    {
        destroyNode(node->left);
        destroyNode(node->right);
        delete node;
    }
}

// Updated: including a clear function so you can switch files and load a new BST to insert into the database
void BinarySearchTree::Clear()
{
    destroyNode(root);
    root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {

    // updated
    destroyNode(root);
    // for safety adding a null to the root so the function can be reused
    root = nullptr;
}

// Updated: Including a new function to assist with the sorting display options.
void BinarySearchTree::collectBids(Node* node, vector<Bid>& bids)
{
    if (node != nullptr)
    {
        collectBids(node->left, bids);
        bids.push_back(node->bid);
        collectBids(node->right, bids);
    }
}

// Updated: Amount Descending Order
void BinarySearchTree::sortDescending()
{
    vector<Bid> bids;
    collectBids(root, bids);

    sort(bids.begin(), bids.end(), [](const Bid& a, const Bid& b) {
        return a.amount > b.amount;
        });

    for (const auto& bid : bids) 
    {
        cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | " << bid.fund << endl;
    }
}


// Updated: Just the opposite of InOrder to display the data
void BinarySearchTree::reverseInOrder(Node* node)
{
    if (node != nullptr)
    {
        reverseInOrder(node->right);
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
        reverseInOrder(node->left);
    }
}

// Updated: Default recursor
void BinarySearchTree::reverseInOrder()
{
    reverseInOrder(root);
}



/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {

    // My Stuff
    inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // My Stuff

    postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // My Stuff

    preOrder(root);
}



/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // My Stuff

    if (root == nullptr)
    {
        root = new Node(bid);
    }
    else
    {
        addNode(root, bid);
    }
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {


    removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {

    Node* currNode = root;

    while (currNode != nullptr)
    {
        if (currNode->bid.bidId == bidId)
        {
            return currNode->bid;
        }
        if (bidId < currNode->bid.bidId)
        {
            currNode = currNode->left;
        }
        else
        {
            currNode = currNode->right;
        }
    }

    // Not mine
    Bid bid;
    return bid;
}


void BinarySearchTree::addNode(Node* node, Bid bid) {
    // My Stuff

    // Updated to now check for duplicate bid Id's
    if (bid.bidId == node->bid.bidId)
    {
        cout << "Bid ID " << bid.bidId << " already exists." << endl;
        return;
    }

    if (node->bid.bidId > bid.bidId)
    {
        if (node->left == nullptr)
        {
            node->left = new Node(bid);;
        }
        else
        {
            addNode(node->left, bid);
        }
    }
    else
    {
        if (node->right == nullptr)
        {
            node->right = new Node(bid);
        }
        else
        {
            addNode(node->right, bid);
        }
    }
}
void BinarySearchTree::inOrder(Node* node) {
  // My Stuff

    if (node != nullptr)
    {
        inOrder(node->left);
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
        inOrder(node->right);
    }
}
void BinarySearchTree::postOrder(Node* node) {

    if (node != nullptr)
    {
        postOrder(node->left);
        postOrder(node->right);
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
    }
}

void BinarySearchTree::preOrder(Node* node) {

    if (node != nullptr)
    {
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
        preOrder(node->left);
        preOrder(node->right);
    }

}

/**
 * Remove a bid from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string bidId) {

    // My Stuff

    if (node == nullptr)
    {
        return node;
    }

    // Traveling down the chain
    if (bidId < node->bid.bidId)
    {
        node->left = removeNode(node->left, bidId);
    }
    else if (bidId > node->bid.bidId)
    {
        node->right = removeNode(node->right, bidId);
    }
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }
        else if (node->left == nullptr)
        {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr)
        {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        else
        {
            Node* temp = node->right;
            while (temp->left != nullptr)
            {
                temp = temp->left;
            }

            node->bid = temp->bid;

            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }

    return node;
}



//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << "******************************" << endl;
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    cout << "******************************" << endl;
    return;
}

// Load CSV data into a BST
void loadBids(string csvPath, BinarySearchTree* bst) {
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
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
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




// Updated milestone three:
// Method to dump into a database
// credit: https://www.sqlite.org/docs.html

void BinarySearchTree::dumpBidsToDatabase(BinarySearchTree* bst, const string& dbFile)
{
    sqlite3* database;
    int close = sqlite3_open(dbFile.c_str(), &database);

    // checking status of close for error
    if (close)
    {
        // I'm somehow only now learning about cerr to output error messages
        cerr << "Error opening DB: " << sqlite3_errmsg(database) << endl;
        return;
    }

    // Create a table to store data into the database
    string table = "CREATE TABLE IF NOT EXISTS Bids("
                   "BidId TEXT PRIMARY KEY, "
                   "title TEXT, "
                   "fund TEXT, "
                   "amount REAL);";

    close = sqlite3_exec(database, table.c_str(), NULL, 0, NULL);

    // checking status of close running if proper comparison not found
    if (close != SQLITE_OK)
    {
        cerr << "Error: " << sqlite3_errmsg(database) << endl;
        sqlite3_close(database);
        return;
    }

    // must be included in public functions to gain access to root
    // loading into vector to then load into the database later
    vector<Bid> bids;
    bst->collectBids(bst->root, bids);

    sqlite3_stmt* tree;
    // Hard coded command to insert data into the database
    table = "INSERT OR REPLACE INTO Bids (bidId, title, fund, amount) VALUES (?, ?, ?, ?);";

    // redefining the close conditions
    close = sqlite3_prepare_v2(database, table.c_str(), -1, &tree, NULL);

    // Checking status of close again after redefinning
    if (close != SQLITE_OK)
    {
        cerr << "Error: " << sqlite3_errmsg(database) << endl;
        sqlite3_close(database);
        return;
    }


    sqlite3_exec(database, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    // Now we can finally insert data into the database from our vector
    for (auto& bid : bids)
    {
        sqlite3_bind_text(tree, 1, bid.bidId.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(tree, 2, bid.title.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(tree, 3, bid.fund.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(tree, 4, bid.amount);

        // redefine close again
        close = sqlite3_step(tree);

        // Final check of close with the data loaded
        if (close != SQLITE_DONE)
        {
            cerr << "Error: " << sqlite3_errmsg(database) << endl;
        }

        // reset to do the next row
        sqlite3_reset(tree);
    }

    sqlite3_exec(database, "COMMIT;", NULL, NULL, NULL);


    sqlite3_finalize(tree);
    sqlite3_close(database);

    cout << "==========================================" << endl;
    cout << "Bids successfully dumped: " << dbFile << endl;
}


// Identical to the loaidng function, but rewritten for displaying to a string.
// Credit: credit: https://www.sqlite.org/docs.html
void displayBidsFromDatabase(const string& file)
{
    sqlite3* database;
    int close = sqlite3_open(file.c_str(), &database);

    if (close)
    {
        cerr << "Error: " << sqlite3_errmsg(database) << endl;
        return;
    }

    string table = "SELECT bidId, title, fund, amount FROM Bids ORDER BY bidId;";

    sqlite3_stmt* tree;

    close = sqlite3_prepare_v2(database, table.c_str(), -1, &tree, NULL);

    if (close != SQLITE_OK)
    {
        cerr << "Error: " << sqlite3_errmsg(database) << endl;
        sqlite3_close(database);
        return;
    }

    // This is where it differs, uses string variable and casting to insert into a string variable
    cout << "==========================================" << endl;
    while (sqlite3_step(tree) == SQLITE_ROW)
    {
        string bidId = reinterpret_cast<const char*>(sqlite3_column_text(tree, 0));
        string title = reinterpret_cast<const char*>(sqlite3_column_text(tree, 1));
        string fund = reinterpret_cast<const char*>(sqlite3_column_text(tree, 2));
        double amount = sqlite3_column_double(tree, 3);

        // Simple display
        cout << bidId << ": " << title << " | " << amount << " | " << fund << endl;

    }

    sqlite3_finalize(tree);
    sqlite3_close(database);
}



// Creating an HTML file to create the groundwork for having a live server to display and edit the database's data
// Credit: https://www.w3schools.com/html/html_intro.asp and https://www.sqlite.org/docs.html
void createHTML(const string& db, const string& html, string webName)
{
    // creating title for webpage
    string title = "<h1>" + webName + "</h1>";
    // checking for status of database and dbfile
    sqlite3* database;
    if (sqlite3_open(db.c_str(), &database))
    {
        cerr << "Error: " << sqlite3_errmsg(database) << endl;
        return;
    }

    // creating the basis of the html file
    ofstream output(html);
    output << "<!DOCTYPE html><html><head><title>Bids</title></head><body>";
    output << title;
    output << "<table border='1'><tr><th>Bid ID</th><th>Title</th><th>Fund</th><th>Amount</th></tr>";

    // creating the table for the database search
    string table = "SELECT bidId, title, fund, amount FROM Bids ORDER BY bidId;";
    sqlite3_stmt* tree;

    //checking the database validity
    if (sqlite3_prepare_v2(database, table.c_str(), -1, &tree, NULL) != SQLITE_OK)
    {
        cerr << "Error" << endl;
        sqlite3_close(database);
        return;
    }

    // Setting up the HTML table from the database
    while (sqlite3_step(tree) == SQLITE_ROW)
    {
        output << "<tr>";
        output << "<td>" << reinterpret_cast<const char*>(sqlite3_column_text(tree, 0)) << "</td>";
        output << "<td>" << reinterpret_cast<const char*>(sqlite3_column_text(tree, 1)) << "</td>";
        output << "<td>" << reinterpret_cast<const char*>(sqlite3_column_text(tree, 2)) << "</td>";
        output << "<td>" << sqlite3_column_double(tree, 3) << "</td>";
        output << "</tr>";
    }

    output << "</table></body></html>";
    sqlite3_finalize(tree);
    sqlite3_close(database);
    output.close();

    cout << "HTML created: " << html << endl;

}



// The one and only main() method
int main(int argc, char* argv[]) {
    // process command line arguments
    string csvPath, csvName, bidKey;
    string searchBidId;
    string amount;
    Bid userBid;
    switch (argc) {
        
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
        
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        csvName = "eBid_Monthly_sales";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "==========================================" << endl;
        cout << "Menu:" << endl;
        cout << "Currently selected CSV file: " << csvPath << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  5. Change File" << endl;
        cout << "  6. Insert Bid" << endl;
        cout << "  7. Dump Into Database" << endl;
        cout << "  8. Display from Database" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        cin.clear();

        switch (choice)
        {

        case 1:
            try
            {
                // Update: now calls the clear() function so you can load a new file
                bst->Clear();

                // Initialize a timer variable before loading bids
                ticks = clock();

                // Complete the method call to load the bids
                loadBids(csvPath, bst);

                //cout << bst->Size() << " bids read" << endl;

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            }
            catch (...)
            {
                cout << "==========================================" << endl;
                cerr << "Error: Name is spelled wrong (most likely)" << endl;
            }
            break;

        case 2:
        {
            int sortChoice = 0;

            cout << "==========================================" << endl;
            cout << "Sorting Display Options:" << endl;
            cout << "  1. Normal Order" << endl;
            cout << "  2. Reverse Order" << endl;
            cout << "  3. Amount from high to low" << endl;
            cout << "Enter choice: ";
            cin >> sortChoice;
            cin.ignore();
            cin.clear();

            switch (sortChoice)
            {
            case 1:
                bst->InOrder();
                break;
            case 2:
                bst->reverseInOrder();
                break;
            case 3:
                bst->sortDescending();
            default:
                cout << "That's not a choice." << endl;
                break;
            }
        }   
        break;

        case 3:
            // Updated: Allowing the user to search for a specific Bid ID
            cout << "Enter Bid ID (to search for): ";
            getline(cin, searchBidId);


            ticks = clock();

            bid = bst->Search(searchBidId);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                // Update: added some visual clarity to the console menue
                cout << "******************************" << endl;
                cout << "Bid Id " << searchBidId << " not found." << endl;
                cout << "******************************" << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            // Updated: added searching for the bid to remove
            cout << "Enter Bid ID (to delete): ";
            getline(cin, searchBidId);


            bst->Remove(searchBidId);
            break;
            
            // Update: included new case for changing the file name
        case 5:
            cout << "Enter new CSV file name (without extension): ";
            getline(cin, csvName);
            csvPath = csvName + ".csv";
            cout << "CSV file changed to: " << csvPath << endl;
            break;
        
            // Update: You can now insert bids using this
        case 6:

            cout << "Enter Bid ID: ";
            getline(cin, userBid.bidId);

            cout << "Enter Bid Title: ";
            getline(cin, userBid.title);

            cout << "Enter Bid Fund: ";
            getline(cin, userBid.fund);

            cout << "Enter Bid Amount (10.50): ";
            getline(cin, amount);
            userBid.amount = strToDouble(amount, '$');

            bst->Insert(userBid);

            cout << "Inserting new Bid" << endl;
            break;

            // loading data into the database
        case 7:
        {
            ticks = clock();
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            // FIXED: Created and calling the dumpdatabase funcion
            string dbName = csvName + ".db";
            bst->dumpBidsToDatabase(bst, dbName);

            createHTML(dbName, csvName + ".html", csvName);

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        }

        // displaying data from the database
        case 8:
        {
            string dbName = csvName + ".db";
            displayBidsFromDatabase(dbName);
            break;
        }

        default:
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}