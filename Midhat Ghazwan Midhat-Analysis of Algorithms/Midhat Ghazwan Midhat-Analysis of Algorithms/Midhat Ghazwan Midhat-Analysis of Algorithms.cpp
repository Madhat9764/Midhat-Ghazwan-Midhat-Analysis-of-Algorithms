/*
Midhat Ghazwan Midhat Project
Balanced search Tree
this is a program written in c++ for Student database
it uses the balanced search tree (avl tree)
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits> // For numeric_limits
using namespace std;
fstream inFile, outFile;
// Structure to represent a node in the AVL Tree
struct AVLNode {
    int id;            // Unique identifier for the student
    string firstName;  // Student's first name
    string lastName;   // Student's last name
    float gpa;         // Student's GPA
    string dob;        // Student's date of birth (YYYY-MM-DD)
    int height;        // Height of the node for balancing the tree
    AVLNode* left;     // Pointer to the left child node
    AVLNode* right;    // Pointer to the right child node

    // Constructor to initialize a new AVL node
    AVLNode(int i, const string& fn, const string& ln, float g, const string& d)
        : id(i), firstName(fn), lastName(ln), gpa(g), dob(d), height(1), left(nullptr), right(nullptr) {}
};

// Class to represent the AVL Tree
class AVLTree {
private:
    AVLNode* root; // Pointer to the root node of the AVL Tree

    // Utility function to get the height of a node, returns 0 if the node is nullptr
    int height(AVLNode* node) {
        return node ? node->height : 0;
    }

    // Function to compute the balance factor of a node
    int balanceFactor(AVLNode* node) {
        return height(node->left) - height(node->right);
    }

    // Function to update the height of a node based on its children
    void updateHeight(AVLNode* node) {
        if (node) {
            node->height = max(height(node->left), height(node->right)) + 1;
        }
    }

    // Right rotation function to balance the tree
    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T = x->right;

        // Perform the rotation
        x->right = y;
        y->left = T;

        // Update heights after the rotation
        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Left rotation function to balance the tree
    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T = y->left;

        // Perform the rotation
        y->left = x;
        x->right = T;

        // Update heights after the rotation
        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // Function to balance the node to ensure the AVL property
    AVLNode* balance(AVLNode* node) {
        updateHeight(node); // Update the height of the current node

        // Left heavy subtree
        if (balanceFactor(node) > 1) {
            // Left-Right case: Left child is right heavy
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node); // Perform right rotation
        }

        // Right heavy subtree
        if (balanceFactor(node) < -1) {
            // Right-Left case: Right child is left heavy
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node); // Perform left rotation
        }

        return node; // No imbalance, return the node as is
    }

    // Function to insert a new node into the tree
    AVLNode* insert(AVLNode* node, int id, const string& firstName, const string& lastName, float gpa, const string& dob) {
        if (!node) return new AVLNode(id, firstName, lastName, gpa, dob); // If the node is empty, create a new node

        // If the ID is smaller, insert into the left subtree
        if (id < node->id) {
            node->left = insert(node->left, id, firstName, lastName, gpa, dob);
        }
        // If the ID is larger, insert into the right subtree
        else if (id > node->id) {
            node->right = insert(node->right, id, firstName, lastName, gpa, dob);
        }
        else {
            return node; // Return the node if ID already exists (duplicates not allowed)
        }

        return balance(node); // Balance the tree after insertion
    }

    // Utility function to find the node with the minimum ID (used during deletion)
    AVLNode* minValueNode(AVLNode* node) {
        while (node->left) node = node->left; // Traverse left to find the smallest node
        return node;
    }

    // Function to delete a node from the AVL tree
    AVLNode* deleteNode(AVLNode* node, int id) {
        if (!node) return node; // Base case: if the node is null, nothing to delete

        // Recursively find the node to delete
        if (id < node->id) {
            node->left = deleteNode(node->left, id);
        }
        else if (id > node->id) {
            node->right = deleteNode(node->right, id);
        }
        else {
            // Node to be deleted found
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right; // Get the child node (if any)
                delete node; // Delete the current node
                return temp; // Return the child node to replace the deleted node
            }

            // Node with two children: Get the inorder successor (smallest in the right subtree)
            AVLNode* temp = minValueNode(node->right);

            // Replace the node's data with the inorder successor's data
            node->id = temp->id;
            node->firstName = temp->firstName;
            node->lastName = temp->lastName;
            node->gpa = temp->gpa;
            node->dob = temp->dob;

            // Delete the inorder successor
            node->right = deleteNode(node->right, temp->id);
        }

        return balance(node); // Balance the tree after deletion
    }

    // In-order traversal to get sorted data in the form of strings
    void inOrderTraversal(AVLNode* node, vector<string>& result) {
        if (node) {
            inOrderTraversal(node->left, result); // Traverse left subtree
            result.push_back(to_string(node->id) + ", " + node->firstName + ", " + node->lastName + ", " + to_string(node->gpa) + ", " + node->dob); // Collect the node's data
            inOrderTraversal(node->right, result); // Traverse right subtree
        }
    }

    // Search for a node by ID
    AVLNode* search(AVLNode* node, int id) {
        if (!node || node->id == id) return node; // Return the node if found or null if not found
        return id < node->id ? search(node->left, id) : search(node->right, id); // Search left or right depending on the ID
    }

public:
    // Constructor to initialize the root of the tree
    AVLTree() : root(nullptr) {}

    // Public function to insert a student record
    void insert(int id, const string& firstName, const string& lastName, float gpa, const string& dob) {
        root = insert(root, id, firstName, lastName, gpa, dob);
    }

    // Public function to delete a student record
    void deleteKey(int id) {
        root = deleteNode(root, id);
    }

    // Public function to search for a student record by ID
    string search(int id) {
        AVLNode* result = search(root, id);
        if (result) {
            return "ID: " + to_string(result->id) + ", Name: " + result->firstName + " " + result->lastName + ", GPA: " + to_string(result->gpa) + ", DOB: " + result->dob;
        }
        return "Key not found"; // If the student is not found
    }

    // Get sorted data in the form of a vector of strings
    vector<string> getSortedData() {
        vector<string> result;
        inOrderTraversal(root, result);
        return result;
    }

    // Save the sorted data to a file
    void saveToFile(const string& filename) {
        ofstream outFile(filename); // Open the file for writing

        if (!outFile.is_open()) {// to ensure file is open
            cerr << "Error: Could not open file " << filename << endl;// error messgae in  case the file is not open
            return;
        }
        vector<string> data = getSortedData();
        for (const string& record : data) {
            outFile << record << endl; // Write each record to the file
        }
        outFile.close(); // Close the file
    }

    // Load data from a file and insert it into the AVL tree
    void loadFromFile(const string& filename) {
        ifstream inFile(filename); // Open the file for reading
        string line;

        if (!inFile.is_open()) {// to ensure inFile is open
            cerr << "Error: Could not open file " << filename << endl;// error message in case inFile is not open
            return;
        }
        while (getline(inFile, line)) {
            stringstream ss(line);
            int id;
            string firstName, lastName, dob;
            float gpa;

            ss >> id; // Read the ID
            ss.ignore(); // Ignore the comma
            getline(ss, firstName, ','); // Read first name
            ss.ignore(); //
            getline(ss, lastName, ',');//Read last name
            ss.ignore();
            ss >> gpa;// Read gpa
            ss.ignore();
            getline(ss, dob);//Read  date of birth

            insert(id, firstName, lastName, gpa, dob);
        }
        inFile.close();// Close inFile
    }
};


// int main
int main() {
    AVLTree studentDirectory;
    //Name of text file that stores the information
    string filename = "students.txt";

    // Load data from file
    studentDirectory.loadFromFile(filename);

    //A DO While loop to implement a switch 
    int choice;
    do {
        // A menu for the program that keeps showing until terminated
        cout << "\nWelcome to The student database program\n";
        cout << "Please select one of the following choices\n";
        cout << "1. Add Student\n";
        cout << "2. Delete Student\n";
        cout << "3. Search Student\n";
        cout << "4. Display All Students\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;//Value Input for switch
        if (cin.fail()) {// if choice is inputted is not available cin will fail 
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid choice. Please try again.\n";
            continue;
        }

        switch (choice) {
            // First Case to add and enter information of a student
        case 1: {
            int id;
            string firstName, lastName, dob;
            float gpa;

            cout << "Enter ID: ";
            cin >> id;
            cin.ignore(); // Clear the input buffer
            cout << "Enter First Name: ";
            getline(cin, firstName);
            cout << "Enter Last Name: ";
            getline(cin, lastName);
            cout << "Enter GPA: ";
            cin >> gpa;
            cin.ignore();
            cout << "Enter Date of Birth (YYYY-MM-DD): ";
            getline(cin, dob);

            studentDirectory.insert(id, firstName, lastName, gpa, dob);
            studentDirectory.saveToFile(filename);

            cout << "Student added successfully!\n";

            break;

        }
              // Second Case to delete a student
        case 2: {
            int id;
            cout << "Enter ID of student to delete: ";
            cin >> id;

            studentDirectory.deleteKey(id);
            studentDirectory.saveToFile(filename);

            cout << "Student deleted successfully!\n";

            break;

        }
              // Third Case to search for a student
        case 3: {
            int id;
            cout << "Enter ID of student to search: ";
            cin >> id;

            string result = studentDirectory.search(id);
            cout << result << endl;

            break;

        }
              // Fourth Case to show all students
        case 4: {
            vector<string> students = studentDirectory.getSortedData();
            cout << "Student Directory:\n";
            for (const string& student : students) {
                cout << student << endl;
            }

            break;

        }
              // Fifth Case to exit program
        case 5:
            cout << "Exiting program...\n";

            break;

            // Default case when an invalid choice is selected
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);//Stopping the while loop when choice is 5 and case 5 is activated 

    return 0;
}

