//Garret Blake
//CS-300


#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>
#include <algorithm>

using namespace std;
//struct for course 
struct Course {
    string ID;
    string Name;
    vector<string> List;
};
//creation of binary search tree
class SearchTree {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;
        Node(const Course& aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
    };

//functions for search tree
    Node* root;
    void inOrder(Node* node);
    Node* insert(Node* node, const Course& aCourse);
    Node* findMin(Node* node);
    Node* search(Node* node, const string& ID);

//search the tree
public:
    SearchTree() : root(nullptr) {}
    void InOrder();
    void Insert(const Course& aCourse);
    Course Search(const string& ID);
};
//add commas between classes
void SearchTree::inOrder(Node* node) {
    if (!node) return;
    inOrder(node->left);
    cout << node->course.ID << "," << node->course.Name << endl;
    inOrder(node->right);
}
//in order traversal of tree
void SearchTree::InOrder() {
    inOrder(root);
}
//insert node for course
void SearchTree::Insert(const Course& aCourse) {
    root = insert(root, aCourse);
}
SearchTree::Node* SearchTree::insert(Node* node, const Course& aCourse) {
    if (!node) return new Node(aCourse);
    if (aCourse.ID < node->course.ID) {
        node->left = insert(node->left, aCourse);
    }
    else {
        node->right = insert(node->right, aCourse);
    }
    return node;
}

SearchTree::Node* SearchTree::findMin(Node* node) {
    while (node && node->left) node = node->left;
    return node;
}

//search tree for course
Course SearchTree::Search(const string& ID) {
    Node* result = search(root, ID);
    return result ? result->course : Course();
}

//search for course
SearchTree::Node* SearchTree::search(Node* node, const string& ID) {
    if (!node || node->course.ID == ID) return node;
    if (ID < node->course.ID) return search(node->left, ID);
    return search(node->right, ID);
}
//split string into tokens
vector<string> Split(const string& line) {
    vector<string> result;
    string temp;
    for (char ch : line) {
        if (ch == ',') {
            result.push_back(temp);
            temp.clear();
        }
        else {
            temp += ch;
        }
    }
    result.push_back(temp);
    return result;
}

//load CSV into file
void loadCourses(const string& csvPath, SearchTree& List) {
    ifstream inFS(csvPath);
    if (!inFS.is_open()) {
        cout << "   Could not open file";
        return;
    }
    string line;
    while (getline(inFS, line)) {
        vector<string> tokens = Split(line);
        if (tokens.size() < 2) {
            cout << "   Error" << endl;
            continue;
        }
        Course aCourse = { tokens[0], tokens[1], vector<string>(tokens.begin() + 2, tokens.end()) };
        List.Insert(aCourse);
    }
    inFS.close();
}

//prerequisite search and print
void displayCourse(const Course& aCourse) {
    cout << aCourse.ID << ", " << aCourse.Name << endl;
    cout << "   Prerequisites: ";
    if (aCourse.List.empty()) {
        cout << "   No Prerequisites" << endl;
    }
    else {
        for (size_t i = 0; i < aCourse.List.size(); ++i) {
            cout << aCourse.List[i];
            if (i < aCourse.List.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

//uppercase for strings
void convertCase(string& toConvert) {
    transform(toConvert.begin(), toConvert.end(), toConvert.begin(), ::toupper);
}

//main function
int main(int argc, char* argv[]) {
    string csvPath = (argc > 1) ? argv[1] : "CS 300 ABCU_Advising_Program_Input.csv";
    SearchTree List;
    int choice = 0;

//loop for menu until user exits
    while (choice != 9) {
        cout << "   Menu:" << endl;
        cout << "   1.Load Courses" << endl;
        cout << "   2.Display All Courses" << endl;
        cout << "   3.Find Course" << endl;
        cout << "   9.Exit" << endl;
        cout << "   Enter choice: ";
        cin >> choice;
        cin.ignore();
//load courses from CSV
        switch (choice) {
        case 1:
            loadCourses(csvPath, List);
            break;
//print courses
        case 2:
            List.InOrder();
            cout << "   Press Enter to continue";
            cin.ignore(); 
            cin.get();
            break;
//search course for prerequisites
        case 3: {
            string courseKey;
            cout << "   What course are you looking for?";
            cin >> courseKey;
            convertCase(courseKey);
            Course course = List.Search(courseKey);
            if (!course.ID.empty()) {
                displayCourse(course);
            }
            else {
                cout << "   Course not found.";
            }
            cout << "   Press Enter to continue";
            cin.ignore();
            cin.get(); 
            break;
        }
//exit program
        case 9:
            cout << "   Goodbye";
            break;
        default:
            cout << "   Invalid choice";
            cout << "   Press Enter to continue";
            cin.ignore(); 
            cin.get(); 
        }
    }
    return 0;
}
