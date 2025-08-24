/*
Rudolph Travers
Project 2
CS-300
*/

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

const int GLOBAL_SLEEP_TIME = 5000;//default time for sleep

//create course object that contains course information

struct Course {

    string courseId;
    string courseName;
    vector<string> preReqs; //vector to store list of prerequisites
};


//create binary search tree
class BinarySearchTree {

private:
        //private class members
        struct Node {
            Course course;
            Node* right;
            Node* left;

            //constuctor
            Node() {
                left = nullptr;
                right = nullptr;
            }

            //initialize Node with a course
            Node(Course initCourse) {
                course = initCourse;
                left = nullptr;
                right = nullptr;
            }
    };

        Node* root;
        void inOrder(Node* node);
        int size = 0;

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course initCourse);
    void Remove(string courseId);
    Course Search(string courseId);
    int Size();
};

//constructor for Binary search tree
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

//way to navigate through the BST
void BinarySearchTree::InOrder() {
    inOrder(root);
}

//insert method for the BST
void BinarySearchTree::Insert(Course initCourse) {
    
    Node* currentNode = root;
   
    if (root == NULL) {
       
        root = new Node(initCourse);
    }
    else {
        
        while (currentNode != NULL) {
            
            if (initCourse.courseId < currentNode->course.courseId) {

                if (currentNode->left == nullptr) {
                    
                    currentNode->left = new Node(initCourse);
                    currentNode = NULL;
                }
                else {
                    currentNode = currentNode->left;
                }
            }
            else {
                 
                if (currentNode->right == nullptr) {
                    currentNode->right = new Node(initCourse);
                    currentNode = NULL;
                 }
                 else {
                     
                     currentNode = currentNode->right;
                        }
                    }
                }

            }
            size++;
        }

//method to remove a course from the BST
void BinarySearchTree::Remove(string courseId) {
    Node* par = NULL;
    Node* currRoot = root;

    while (currRoot != NULL) {

        if (currRoot->course.courseId == courseId) {
            if (currRoot->left == NULL && currRoot->right == NULL) {
                if (par == NULL) {
                    root = nullptr;
                }
                else if (par->left == currRoot) {
                    par->left = NULL;
                }
                else {
                    par->right = NULL;
                }
            }
            else if (currRoot->right == NULL) {
                if (par == NULL) {
                    root = currRoot->left;
                }
                else if (par->left == currRoot) {
                    par->left = currRoot->left;
                }
                else {
                    par->right = currRoot->left;
                }
            }
            else if (currRoot->left == NULL) {
                if (par == NULL) {
                    root = currRoot->right;
                }
                else if (par->left == currRoot) {
                    par->left = currRoot->right;
                }
                else {
                    par->right = currRoot->right;
                }
            }
            else {
                Node* success = currRoot->right;

                while (success->left != NULL) {
                    success = success->left;
                }

                Node successorData = Node(success->course);
                Remove(success->course.courseId);
                currRoot->course = successorData.course;
            }
            return;
        }
        else if (currRoot->course.courseId < courseId) {
        par = currRoot;
        currRoot = currRoot->right;
        }
        else {
        par = currRoot;
        currRoot = currRoot->left;
            }
        }
        cout << "Incorrect Value" << endl;
         return;
}

//method to search through BST using courseID
Course BinarySearchTree::Search(string courseId) {

    Course initCourse;

    Node* currentNode = root;

    while (currentNode != NULL) {

        if (currentNode->course.courseId == courseId) {

            return currentNode->course;
        }
        else if (courseId < currentNode->course.courseId) {

            currentNode = currentNode->left;
        }
        else {

            currentNode = currentNode->right;
        }
    }

   cout << "Incorrect Value." << endl;
    return initCourse;
}

void BinarySearchTree::inOrder(Node* node) {

    if (node == NULL) {

        return;
    }
    inOrder(node->left);

    cout << node->course.courseId << ", " << node->course.courseName << endl;

    inOrder(node->right);
}

//see the size of the BST
int BinarySearchTree::Size() {

    return size;
}

vector<string> Split(string lineFeed) {

    char delim = ',';

    lineFeed += delim;
    vector<string> lineTokens;
    string temp = "";
    for (int i = 0; i < lineFeed.length(); i++)
    {
        if (lineFeed[i] == delim)
        {
            lineTokens.push_back(temp);
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}

//method to load coursess into the BST
void loadCourses(string csvPath, BinarySearchTree* courseList) {

    ifstream csv; //insteam to read file
    string line; //line feed 
    vector<string> stringTokens;

    csv.open(csvPath); //open the read file

    if (!csv.is_open()) {//small error handler
        cout << "Could not open file." << endl;
        return;
    }

    while (!csv.eof()) {

        Course initCourse;//create a new struct for each "line"

        getline(csv, line);
        stringTokens = Split(line); //split the line using the delimiter

        if (stringTokens.size() < 2) { //if there aren't 2 tokens the line is incorrect

            cout << "\nError. Skipping line." << endl;
        }

        else {

            initCourse.courseId = stringTokens.at(0);
            initCourse.courseName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {

                initCourse.preReqs.push_back(stringTokens.at(i));
            }

            // moves this course to the end
            courseList->Insert(initCourse);
        }
    }

    csv.close(); //close the file
}

void displayCourse(Course initCourse) {

    cout << initCourse.courseId << ", " << initCourse.courseName << endl;
    cout << "Prerequisites: ";

    if (initCourse.preReqs.empty()) {//if the lsit is empty then there are no prereq

        cout << "none" << endl;
    }
    else {

        for (unsigned int i = 0; i < initCourse.preReqs.size(); i++) {

            cout << initCourse.preReqs.at(i);

            if (initCourse.preReqs.size() > 1 && i < initCourse.preReqs.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}


int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, initCourseKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        initCourseKey = argv[2];
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
    }

    // Define a table to hold all the courses
    BinarySearchTree* courseList = new BinarySearchTree();

    Course course;
    bool goodInput;
    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        initCourseKey = ""; //clear the string        
        string anyKey = " "; //clear the string
        choice = 0; //clear the choice

        try {
            cin >> choice;

            if ((choice > 0 && choice < 5) || (choice == 9)) {// limit the user menu inputs to good values
                goodInput = true;
            }
            else {//throw error for catch
                goodInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:

                // Complete the method call to load the courses
                loadCourses(csvPath, courseList);
                cout << courseList->Size() << " courses read" << endl;

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 2:
                courseList->InOrder();
                cin >> anyKey;

                break;

            case 3:

                cout << "Enter the course Id?" << endl;
                cin >> initCourseKey;

                course = courseList->Search(initCourseKey);

                if (!course.courseId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "\nCourse ID " << initCourseKey << " not found." << endl;
                }

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 9:
                exit;
                break;

            default:

                throw 2;
            }
        }

        catch (int err) {

            std::cout << "\nPlease check your input." << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }
        cin.clear();
        cin.ignore();
        system("cls");
    }

    cout << "Good bye." << endl;

    Sleep(GLOBAL_SLEEP_TIME);

    return 0;
}
