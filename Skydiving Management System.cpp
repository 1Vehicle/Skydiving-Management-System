#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <cstring>

using namespace std;

class Welcome 
{
public:
	// Welcome as constructor
	Welcome() 
	{
    // Read from the text file
    ifstream welcomeFile("welcome.txt");
	// Check if the file is open
        if (welcomeFile.is_open()) 
		{
        // Use a while loop together with the getline() function to read the file line by line
        string line;
        while (getline(welcomeFile, line)) 
		{
        // Output the text from the file
        cout << line << endl;
        }
		// Close the file
        welcomeFile.close();
        } 
		else 
		{
        // Print an error message if the file couldn't be opened
        cerr << "Unable to open the welcome file." << endl;
        }
    }
};

// Structure to hold user registration details
struct User {
    string username;
    string password;
    string role;
    User* next;

    User(const string& uname, const string& pwd, const string& r) 
        : username(uname), password(pwd), role(r), next(NULL) {}
};
    // Helper function for partitioning
User* partition(User* start, User* end) {
    string pivotValue = end->username;
    User* pivot = start;
    User* current = start;

    while (current != end) {
        if (current->username < pivotValue) {
            swap(pivot->username, current->username);
            swap(pivot->password, current->password);
            swap(pivot->role, current->role);
            pivot = pivot->next;
        }
        current = current->next;
    }
    swap(pivot->username, end->username);
    swap(pivot->password, end->password);
    swap(pivot->role, end->role);
    return pivot;
}

// Recursive function for quick sort
void quickSortRecursive(User* start, User* end) {
    if (start == NULL || end == NULL || start == end || start == end->next) {
        return;
    }

    User* pivot = partition(start, end);
    if (pivot != start) {
        User* temp = start;
        while (temp->next != pivot && temp->next != NULL) {
            temp = temp->next;
        }
        quickSortRecursive(start, temp);
    }
    quickSortRecursive(pivot->next, end);
}
// Helper function for recursive binary search
User* binarySearchRecursive(User* start, const string& key) {
    if (start == NULL)
        return NULL;

    User* mid = start;
    int comparison = key.compare(mid->username);

    if (comparison == 0)
        return mid;
    else if (comparison < 0)
        return binarySearchRecursive(start->next, key);
    else
        return binarySearchRecursive(mid->next, key);
}

// Function to perform binary search on the sorted linked list
User* binarySearch(User* head, const string& key) {
    return binarySearchRecursive(head, key);
}

// Class to manage user registration and authentication
class Registration {
private:
    User* head; // Head of the linked list
	
public:
    Registration() : head(NULL) 
	{
		// Load existing users from the file into the linked list
        loadUsersFromFile();
	}
   // Function to perform binary search on the sorted linked list
    User* binarySearch(const string& key) {
        return ::binarySearchRecursive(head, key);
    }
    void sortUsers() {
        if (head == NULL || head->next == NULL) {
            return;
        }
        User* end = head;
        while (end->next != NULL) {
            end = end->next;
        }
        quickSortRecursive(head, end);
        updateFile(); // Update the file after sorting
    }
    // Function to check the login credentials
    bool authenticate(const string& username, const string& password, const string& role) {
        ifstream userFile("registration.txt");

        if (userFile.is_open()) {
            string storedUsername, storedPassword, storedRole;

            // Read the file line by line
            while (userFile >> storedRole >> storedUsername >> storedPassword) {
                // Check if the input credentials match with the stored credentials
                if (storedUsername == username && storedPassword == password && storedRole == role) {
                    userFile.close();
                    updateUserHead(username);
                    return true; // Authentication successful
                }
            }

            userFile.close();
        }

        return false; // Authentication failed
    }

    // Function to register a new user
    void registerUser(const string& username, const string& password, const string& role) {
        ofstream userFile("registration.txt", ios::app);

        if (userFile.is_open()) {
            // Save the registration details in the file
            userFile << role << " " << username << " " << password << endl;
            userFile.close();

            // Add user to linked list
            User* newUser = new User(username, password, role);
            addUserToList(newUser);

            cout << "User registered successfully!" << endl;
        } else {
            cout << "Unable to open the registration file." << endl;
        }
    }

    // Function to add a user to the linked list
    void addUserToList(User* newUser) {
        if (head == NULL) {
            head = newUser;
        } else {
            User* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newUser;
        }
    }

 // Function to display all registered users (for testing)
    void displayUsers() {
        User* temp = head;
        while (temp != NULL) {
            cout << "Username: " << temp->username << ", Role: " << temp->role << endl;
            temp = temp->next;
        }
    }
// Function to read user information from a file and add to the linked list
    void loadUsersFromFile() {
        ifstream userFile("registration.txt");

        if (userFile.is_open()) {
            string storedRole, storedUsername, storedPassword;
            while (userFile >> storedRole >> storedUsername >> storedPassword) {
                User* newUser = new User(storedUsername, storedPassword, storedRole);
                addUserToList(newUser);
            }
            userFile.close();
        } else {
            cout << "Unable to open the registration file." << endl;
        }
    }

    // Function to read user information from a file and display
    void displayUsersFromFile() {
        ifstream MyReadFile("registration.txt");

        if (MyReadFile.is_open()) {
            cout << "\n====================" << endl;
            cout << "Users Information" << endl;
            cout << "====================\n" << endl;

            string storedRole, storedUsername, storedPassword;
            while (MyReadFile >> storedRole >> storedUsername >> storedPassword) {
                cout << "Username: " << storedUsername << ", Role: " << storedRole << "Password: " << storedPassword<< endl;
            }

            MyReadFile.close();
        } else {
            cout << "Unable to open the file." << endl;
        }
    }

    // Function to handle user registration and login
    void Reg() {
        string role, log, username, password;

        // Ask if the user is admin or customer
        cout << "Are you an admin or customer? (admin/customer): ";
        cin >> role;

        if (role != "admin" && role != "customer") {
            cout << "Invalid role. Exiting program." << endl;
            return;
        }

        bool loggedIn = false;

     while (true)  { // Loop until the user successfully logs in or chooses to exit
        // Ask the user if they want to login or register
        cout << "Do you want to login or register? (login/register): ";
        cin >> log;

            if (log == "login") {
                // Ask for login credentials
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                // Check the credentials
                if (authenticate(username, password, role)) {
                    cout << "Login successful!" << endl;
                     loggedIn = true;;
                     break;
                } else {
                    cout << "Authentication failed. Please try again." << endl;
                    // Allow user to retry login
                }
            } else if (log == "register") {
                // Ask for registration credentials
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                // Register the new user
                registerUser(username, password, role);

                // After registration, return to login phase
                continue; // Continue to loop for login/register choice
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
// Function to update the file with the current state of the linked list
    void updateFile() {
        ofstream userFile("registration.txt");

        if (userFile.is_open()) {
            User* temp = head;
            while (temp != NULL) {
                userFile << temp->role << " " << temp->username << " " << temp->password << endl;
                temp = temp->next;
            }
            userFile.close();
        } else {
            cout << "Unable to open the registration file." << endl;
        }
    }
    // Destructor to free allocated memory
    ~Registration() {
        User* temp = head;
        while (temp != NULL) {
            User* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }
    // Getter for role
    string getRole() const {
        if (head != NULL) {
            return head->role;
        }
        return "";
    }
// Function to update head to point to the logged-in user
    void updateUserHead(const string& username) {
        User* temp = head;
        while (temp != NULL) {
            if (temp->username == username) {
                head = temp;
                return;
            }
            temp = temp->next;
        }
    }
void Users() {
	char US;
    do {
        cout << "\n===================================" << endl;
        cout << "Welcome to Pak Man Skydiving Centre" << endl;
        cout << "===================================\n" << endl;
        cout << "[1] Display Users" << endl;
        cout << "[2] Sort Users" << endl;
        cout << "[3] Search Users" << endl;
        cout << "[4] Exit" << endl;
        cout << "======================================\n" << endl;

        cout << "Enter your choice: ";
        cin >> US;

        switch (US) {
            case '1':
                displayUsersFromFile();
                break;
            case '2':
                sortUsers();
                    cout << "Users have been sorted successfully." << endl;
                break;
            case '3': {
                string username;
                cout << "Enter username to search: ";
                cin >> username;
                User* foundUser = binarySearch(username);
                if (foundUser) {
                    cout << "User found: " << foundUser->username << ", Role: " << foundUser->role << endl;
                } else {
                    cout << "User not found." << endl;
                }
                break;
            }
            case '4':
                cout << "Exiting to main menu..." << endl;
                return;
            default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (US != '4');
}

};

// Define a structure for the nodes of the linked list
struct Node {
    char name[100];
    char ID[12];
    char number[20];
    string package;
    Node* next;
};

// Function declarations for sorting and searching
void heapify(Node* arr[], int n, int i);
void heapSort(Node* arr[], int n);
void countingSort(Node* arr[], int n);
Node* jumpSearch(Node* arr[], int n, const char* key);
Node* binarySearch(Node* arr[], int l, int r, const char* key);
Node* metaBinarySearch(Node* arr[], int n, const char* key);
// Helper functions for sorting and searching
void heapify(Node* arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && strcmp(arr[left]->name, arr[largest]->name) > 0)
        largest = left;

    if (right < n && strcmp(arr[right]->name, arr[largest]->name) > 0)
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(Node* arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void countingSort(Node* arr[], int n) {
    Node* output[n];
    int count[256];
    memset(count, 0, sizeof(count));

    for (int i = 0; i < n; i++)
        count[arr[i]->name[0]]++;

    for (int i = 1; i < 256; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i]->name[0]] - 1] = arr[i];
        count[arr[i]->name[0]]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}

Node* jumpSearch(Node* arr[], int n, const char* key) {
    int step = sqrt(n);
    int prev = 0;

    while (strcmp(arr[min(step, n) - 1]->name, key) < 0) {
        prev = step;
        step += sqrt(n);
        if (prev >= n)
            return NULL;
    }

    for (int i = prev; i < min(step, n); i++) {
        if (strcmp(arr[i]->name, key) == 0)
            return arr[i];
    }

    return NULL;
}

Node* binarySearch(Node* arr[], int l, int r, const char* key) {
    while (l <= r) {
        int m = l + (r - l) / 2;

        int res = strcmp(arr[m]->name, key);

        if (res == 0)
            return arr[m];

        if (res < 0)
            l = m + 1;
        else
            r = m - 1;
    }

    return NULL;
}

Node* metaBinarySearch(Node* arr[], int n, const char* key) {
    return binarySearch(arr, 0, n - 1, key);
}

class LinkedList {
private:
    Node* head; // Pointer to the first node in the linked list
    string filename; // File name for storing the data
	float total;
public:
    // Constructor
    LinkedList(const string& filename) {
        head = NULL;
        this->filename = filename;
    }

    // Destructor
    ~LinkedList() {
        Node* current = head;
        while (current != NULL) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = NULL;
    }

    // Function to add a new node to the linked list and store in file
    void addNode(const char* name, const char* ID, const char* number, const string& package) {
        Node* newNode = new Node;
        strncpy(newNode->name, name, 99);
        newNode->name[99] = '\0';
        strncpy(newNode->ID, ID, 11);
        newNode->ID[11] = '\0';
        strncpy(newNode->number, number, 19);
        newNode->number[19] = '\0';
        newNode->package = package;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }

        // Store the node details in the specified file
        ofstream outFile(filename.c_str(), ios::app);
        if (outFile.is_open()) {
            outFile << newNode->name << " " << newNode->ID << " " << newNode->number << " " << newNode->package << endl;
            outFile.close();
        } else {
            cout << "Unable to open file for writing." << endl;
        }
    }

    // Function to display all nodes in the linked list
    void displayNodes() {
        Node* current = head;
        while (current != NULL) {
            cout << "Name: " << current->name << endl;
            cout << "ID: " << current->ID << endl;
            cout << "Number: " << current->number << endl;
            cout << "Package: " << current->package << endl;
            cout << "====================" << endl;
            current = current->next;
        }
    }
    bool modifyNode(const char* oldName, const char* newName, const char* newID, const char* newNumber, const string& newPackage) {
        Node* current = head;
        bool found = false;

        while (current != NULL) {
            if (strcmp(current->name, oldName) == 0) {
                // Modify the node's data
                strncpy(current->name, newName, 99);
                current->name[99] = '\0';
                strncpy(current->ID, newID, 11);
                current->ID[11] = '\0';
                strncpy(current->number, newNumber, 19);
                current->number[19] = '\0';
                current->package = newPackage;
                found = true;
                break;
            }
            current = current->next;
        }

        return found;
    }

    void saveToFile(const string& filename) {
        ofstream outFile(filename.c_str(), ios::app);

        if (outFile.is_open()) {
            Node* current = head;
            while (current != NULL) {
                outFile << current->name << " " << current->ID << " "
                        << current->number << " " << current->package << endl;
                current = current->next;
            }
            outFile.close();
        } else {
            cout << "Unable to open file for saving." << endl;
        }
    }

    void clear() {
        Node* current = head;
        while (current != NULL) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = NULL;
    }
 void calculate(const string& package) {
        float price = 0.0;
        if (package == "A" || package == "a") {
            price = (400.0 - (0.75 * 400.0)) * 7;
        } else if (package == "B" || package == "b") {
            price = (300.0 - (0.50 * 300.0)) * 5;
        } else if (package == "C" || package == "c") {
            price = (200.0 - (0.25 * 200.0)) * 3;
        } else if (package == "D" || package == "d") {
            price = 100.0;
        } else {
            cout << "Invalid package. Defaulting to No Package." << endl;
            price = 0.0;
        }
        total = price;
    }
void receipt(const char* name, const string& package) {
        cout << "=================" << endl;
        cout << "=== Receipt ====" << endl;
        cout << "=================" << endl;
        
        // Print the receipt details
        cout << "Name: " << name << endl;
        cout << "Package Info: " << package << endl;
        cout << "Total: RM" << fixed << setprecision(2) << total << endl;
            
        // Open a file for writing the receipt
        ofstream receiptFile("receipt.txt", ios::app);

        // Check if the file is open
        if (receiptFile.is_open()) {
            // Write the receipt data to the file
            receiptFile << "=================" << endl;
            receiptFile << "=== Receipt ====" << endl;
            receiptFile << "=================" << endl;
            receiptFile << "Name: " << name << endl;
            receiptFile << "Package Info: " << package << endl;
            receiptFile << "Total: RM" << fixed << setprecision(2) << total << endl;

            // Close the file
            receiptFile.close();
            cout << "Receipt saved to receipt.txt" << endl;
        } else {
            // Print an error message if the file couldn't be opened
            cout << "Unable to open the receipt file." << endl;
        }
    }

    // Function to sort the linked list using heap sort
    void sortNodesHeap() {
        // Count number of nodes
        int n = 0;
        Node* current = head;
        while (current != NULL) {
            n++;
            current = current->next;
        }

        // Create an array of pointers to nodes
        Node* arr[n];
        int i = 0;
        current = head;
        while (current != NULL) {
            arr[i] = current;
            current = current->next;
            i++;
        }

        // Perform heap sort on the array of pointers
        heapSort(arr, n);

        // Reorder the linked list based on the sorted array
        head = arr[0];
        current = head;
        for (int i = 1; i < n; i++) {
            current->next = arr[i];
            current = current->next;
        }
        current->next = NULL;
    }

    // Function to sort the linked list using counting sort
    void sortNodesCounting() {
        // Count number of nodes
        int n = 0;
        Node* current = head;
        while (current != NULL) {
            n++;
            current = current->next;
        }

        // Create an array of pointers to nodes
        Node* arr[n];
        int i = 0;
        current = head;
        while (current != NULL) {
            arr[i] = current;
            current = current->next;
            i++;
        }

        // Perform counting sort on the array of pointers
        countingSort(arr, n);

        // Reorder the linked list based on the sorted array
        head = arr[0];
        current = head;
        for (int i = 1; i < n; i++) {
            current->next = arr[i];
            current = current->next;
        }
        current->next = NULL;
    }

    // Function to search for a node using jump search
    Node* searchNodeJump(const char* key) {
        // Count number of nodes
        int n = 0;
        Node* current = head;
        while (current != NULL) {
            n++;
            current = current->next;
        }

        // Create an array of pointers to nodes
        Node* arr[n];
        int i = 0;
        current = head;
        while (current != NULL) {
            arr[i] = current;
            current = current->next;
            i++;
        }

        // Perform jump search on the array of pointers
        return jumpSearch(arr, n, key);
    }

    // Function to search for a node using meta binary search
    Node* searchNodeMetaBinary(const char* key) {
        // Count number of nodes
        int n = 0;
        Node* current = head;
        while (current != NULL) {
            n++;
            current = current->next;
        }

        // Create an array of pointers to nodes
        Node* arr[n];
        int i = 0;
        current = head;
        while (current != NULL) {
            arr[i] = current;
            current = current->next;
            i++;
        }

        // Perform meta binary search on the array of pointers
        return metaBinarySearch(arr, n, key);
    }
};

class Skydiving
 {
private:
    LinkedList skydiversList;
	char package;
    float price, total;
public:
    Skydiving() : skydiversList("skydivers.txt") {}

    void addSkydiver() {
        char name[100], ID[12], number[20];
        string package;
        cout << "Enter name: ";
        cin >> name;
        cout << "Enter IC.No: ";
        cin >> ID;
        cout << "Enter contact number: ";
        cin >> number;
        cout << "Please select your package: ";
        cin >> package;

        skydiversList.addNode(name, ID, number, package);
        skydiversList.calculate(package);
        skydiversList.receipt(name, package);
        
    }

   void displaySkydivers() {
    // Open the file for reading
    ifstream MyReadFile("skydivers.txt");

    // Check if the file is open
    if (MyReadFile.is_open()) {
        cout << "\n====================" << endl;
        cout << "Skydiver Information" << endl;
        cout << "====================\n" << endl;

        // Display skydivers from linked list
        skydiversList.displayNodes();

        // Close the file
        MyReadFile.close();
    } else {
        // Print an error message if the file couldn't be opened
        cout << "Unable to open the file." << endl;
    }
}

    void sortSkydivers() {
        cout << "\nSorting Skydivers...\n";
        skydiversList.sortNodesHeap();
        cout << "Skydivers sorted successfully." << endl;
    }

    void searchSkydiver() {
        char key[100];
        cout << "Enter name to search: ";
        cin >> key;

        Node* result = skydiversList.searchNodeJump(key);
        if (result != NULL) {
            cout << "Skydiver found:" << endl;
            cout << "Name: " << result->name << endl;
            cout << "ID: " << result->ID << endl;
            cout << "Number: " << result->number << endl;
            cout << "Package: " << result->package << endl;
        } else {
            cout << "Skydiver not found." << endl;
        }
    }
void modify_skydiver() {
    // Prompt for skydiver name to modify
    cout << "\nEnter Skydiver Name to Modify: ";
    cin.ignore(); // Ignore any newline characters left in the input buffer
    char oldName[100];
    cin.getline(oldName, 100);

    // Prompt for new information
    cout << "Enter new Skydiver Name: ";
    char newName[100];
    cin.getline(newName, 100);

    // Prompt for new IC.No
    cout << "Enter new IC.No: ";
    char newID[12];
    cin.getline(newID, 12);

    // Prompt for new contact number
    cout << "Enter new contact number: ";
    char newNumber[20];
    cin.getline(newNumber, 20);

    // Prompt for new package
    cout << "Please select new package: ";
    char newPackage[100];
    cin.getline(newPackage, 100);

    // Modify the skydiver in the linked list
    if (skydiversList.modifyNode(oldName, newName, newID, newNumber, newPackage)) {
        cout << "Skydiver Modified Successfully!" << endl;
        // Save the modified list to file
        skydiversList.saveToFile("skydivers.txt");
    } else {
        cout << "Skydiver not found. No modifications made." << endl;
    }
}

void delete_skydiver() {
    // Clear the linked list
    skydiversList.clear();

    // Save the cleared list to file
    skydiversList.saveToFile("skydivers.txt");

    cout << "Skydivers Data Cleared Successfully!" << endl;
}

// Function to display the packages
void packages() {
    cout << "\n===============" << endl;
    cout << "List of packages" << endl;
    cout << "===============\n" << endl;
    cout << "Package A - This package costs RM400.00 and includes 7 jumps with a 75% discount." << endl;
    cout << "Package B - This package costs RM300.00 and includes 5 jumps with a 50% discount." << endl;
    cout << "Package C - This package costs RM200.00 and includes 3 jumps with a 25% discount." << endl;
    cout << "Package D - This package costs RM100.00 and does not include any jumps and discount.\n" << endl;
    cout << "[1] Book Skydiving" << endl;
    cout << "[2] Exit to Main Menu\n" << endl;

    char subchoice;
    cout << "Enter your choice: ";
    cin >> subchoice;

    switch (subchoice) {
        case '1':
            addSkydiver();

            break;
        case '2':
            skydivers(); // Assuming skydivers() is another function you want to call
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
    }
}

void skydivers()
{
    cout << "\n===================================" << endl;
    cout << "Welcome to Pak Man Skydiving Centre" << endl;
    cout << "===================================\n" << endl;
    cout << "[1] Book Skydivers" << endl;
    cout<<"[2] View Packages"<<endl;
    cout << "[3] Display Skydivers" << endl;
    cout << "[4] Modify Skydivers" << endl;
    cout << "[5] Delete Skydivers" << endl;
    cout << "[6] Sort Skydivers" << endl;
    cout << "[7] Search Skydivers" << endl;
    cout << "[8] Exit" << endl;
	cout<<"======================================\n"<<endl;
        
	char sky;
    cout << "Enter your choice: ";
    cin >> sky;

    switch (sky) 
	{
        case '1':
            addSkydiver();           
			break;
        case '2':
            packages();
            break;
        case '3':
            displaySkydivers();
            break;
        case '4':
            modify_skydiver();
            break;
        case '5':
            delete_skydiver();
            break;
        case '6':
            sortSkydivers();
            break;
        case '7':
            searchSkydiver();
            break;
        case '8':
            break;
            default:
            cout << "Invalid choice. Please try again." << endl;
            break;
    }
}
// Destructor
    ~Skydiving() 
	{
        cout << "=======Have a Great Journey=======" << endl;
        cout << "=======Thank You======";
    }
};

class Instructor {
private:
    LinkedList instructorsList;

public:
    Instructor() : instructorsList("instructors.txt") {}

    void addInstructor() {
        char name[100], ID[12], number[20];
        string package;
        cout << "Enter name: ";
        cin >> name;
        cout << "Enter IC.No: ";
        cin >> ID;
        cout << "Enter contact number: ";
        cin >> number;
        cout << "Please select your package: ";
        cin >> package;

        instructorsList.addNode(name, ID, number, package);
    }

    void displayInstructors() 
	{
	ifstream MyReadFile("instructors.txt");
	// Check if the file is open
    if (MyReadFile.is_open())
	{
        cout << "\n====================" << endl;
        cout << "Instructor Information" << endl;
        cout << "====================\n" << endl;
        instructorsList.displayNodes();
        
		MyReadFile.close();
    }
    else 
	{
    // Print an error message if the file couldn't be opened
    cout << "Unable to open the file." << endl;
    }
}
    void sortInstructors() {
        cout << "\nSorting Instructors...\n";
        instructorsList.sortNodesCounting();
        cout << "Instructors sorted successfully." << endl;
    }

    void searchInstructor() {
        char key[100];
        cout << "Enter name to search: ";
        cin >> key;

        Node* result = instructorsList.searchNodeMetaBinary(key);
        if (result != NULL) {
            cout << "Instructor found:" << endl;
            cout << "Name: " << result->name << endl;
            cout << "ID: " << result->ID << endl;
            cout << "Number: " << result->number << endl;
            cout << "Package: " << result->package << endl;
        } else {
            cout << "Instructor not found." << endl;
        }
    }
void modify_instructor()
{
// Prompt for Instructors name to modify
    cout<<"\nEnter Instructor Name to Modify: ";
    cin.ignore();
    char oldName[100];
    cin.getline(oldName,100);
   
// Prompt for new information
    cout<<"Update new Instructor Name: ";
    char newName[100];
    cin.getline(newName, 100);
	cout<<"Update ID.No: ";
	char newID[12];
    cin.getline(newID, 12);
	cout<<"Update contact number: ";
    char newNumber[20];
    cin.getline(newNumber, 20);
    cout<<"Update package details:";
	char newPackage[100];
    cin.getline(newPackage, 100);

if (instructorsList.modifyNode(oldName, newName, newID, newNumber, newPackage)) {
        cout << "Instructor Modified Successfully!" << endl;
        // Save the modified list to file
        instructorsList.saveToFile("instructors.txt");
    } else {
        cout << "Instructor not found. No modifications made." << endl;
    }
}

    void delete_instructor() 
{
    // Clear the linked list
    instructorsList.clear();

    // Save the cleared list to file
    instructorsList.saveToFile("skydivers.txt");

    cout << "Instructors Data Cleared Successfully!" << endl;
}
void instructor ()
{
	cout << "\n===================================" << endl;
    cout << "Welcome to Pak Man Skydiving Centre" << endl;
    cout << "===================================\n" << endl;
    cout << "[1] Add Instructors" << endl;
    cout << "[2] Display Instructors" << endl;
    cout << "[3] Modify Instructors" << endl;
    cout << "[4] Delete Instructors" << endl;
    cout << "[5] Sort Instructors" << endl;
    cout << "[6] Search Instructors" << endl;
    cout << "[7] Exit" << endl;
	cout<<"======================================\n"<<endl;	
		char ins;
        cout << "Enter your choice: ";
        cin >> ins;

        switch (ins) 
		{
            case '1':
            addInstructor(); 
            break;
			case '2':
			displayInstructors(); 
                break;
            case '3':
            modify_instructor(); 
                break;
            case '4':
            delete_instructor();
                break;
            case '5':
            sortInstructors();
                break;
            case '6':
            searchInstructor();
                break;
			case '7':
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
}
};

class Equipment {
private:
    LinkedList equipmentsList;

public:
    Equipment() : equipmentsList("equipments.txt") {}

    void addEquipment() {
        char name[100], ID[12], number[20];
        string package;
        cout << "Enter equipment name: ";
        cin >> name;
        cout << "Enter equipment ID: ";
        cin >> ID;
        cout << "Enter quantity: ";
        cin >> number;
		cout << "Enter equipment type: ";
        cin >> package;
        equipmentsList.addNode(name, ID, number,package);
    }

    void displayEquipments() 
{
    ifstream MyReadFile("equipments.txt");

    // Check if the file is open
    if (MyReadFile.is_open()) 
	{
        cout << "\n====================" << endl;
        cout << "Equipment Information" << endl;
        cout << "====================\n" << endl;
        equipmentsList.displayNodes();
        
        MyReadFile.close();
    }
    else 
	{
    // Print an error message if the file couldn't be opened
    cout << "Unable to open the file." << endl;
    }
}
    void sortEquipments() {
        cout << "\nSorting Equipments...\n";
        equipmentsList.sortNodesHeap();
        cout << "Equipments sorted successfully." << endl;
    }

    void searchEquipment() {
        char key[100];
        cout << "Enter equipment name to search: ";
        cin >> key;

        Node* result = equipmentsList.searchNodeJump(key);
        if (result != NULL) {
            cout << "Equipment found:" << endl;
            cout << "Name: " << result->name << endl;
            cout << "ID: " << result->ID << endl;
            cout << "Number: " << result->number << endl;
            cout << "Type: " << result->package << endl;
        } else {
            cout << "Equipment not found." << endl;
        }
    }
void modify_equipment()
{
// Prompt for skydiver name to modify
    cout<<"\nEnter Equipment Name to Modify: ";
    cin.ignore();
    char oldName[100];
    cin.getline(oldName, 100);
   
// Prompt for new information
    cout<<"Update new Equipment Name: ";
    char newName[100];
    cin.getline(newName, 100);
	cout<<"Update equipment ID.No: ";
	char newID[12];
    cin.getline(newID, 12);
    cout<<"Update equipment quantity: ";
    char newNumber[100];
    cin.getline(newNumber, 100);
    cout << "Equipment Type: ";
    char newPackage[100];
    cin.getline(newPackage, 100);
    if (equipmentsList.modifyNode(oldName, newName, newID, newNumber, newPackage)) {
        cout << "Equipment Modified Successfully!" << endl;
        // Save the modified list to file
        equipmentsList.saveToFile("equipments.txt");
    } else {
        cout << "Equipment not found. No modifications made." << endl;
    }
} 
void delete_equipment() 
	{
    // Clear the linked list
    equipmentsList.clear();

    // Save the cleared list to file
    equipmentsList.saveToFile("equipments.txt");

    cout << "Skydivers Data Cleared Successfully!" << endl;
}
void equipment ()
{
	cout << "\n===================================" << endl;
    cout << "Welcome to Pak Man Skydiving Centre" << endl;
    cout << "===================================\n" << endl;
    cout << "[1] Add Equipments" << endl;
    cout << "[2] Display Equipments" << endl;
    cout << "[3] Modify Equipments" << endl;
    cout << "[4] Delete Equipments" << endl;
    cout << "[5] Sort Equipments" << endl;
    cout << "[6] Search Equipments" << endl;
    cout << "[7] Exit" << endl;
	cout<<"======================================\n"<<endl;	
		
	char equip;
    cout << "Enter your choice: ";
    cin >> equip;

    switch (equip) 
	{
        case '1':
        addEquipment();
        break;
		case '2':
		displayEquipments();
        break;
        case '3':
        modify_equipment();
        break;
    	case '4':
        delete_equipment();
        break;
        case '5':
        sortEquipments();
        break;
        case '6':
        searchEquipment();
        break;
    	case '7':
        break;
        default:
        cout << "Invalid choice. Please try again." << endl;
        break;
   }
}
};
int main() {
    int choice;
    Skydiving sky;
    Instructor ins;
    Equipment eq;
    Welcome welcomeInstance;
    Registration reg;

    do {
        // Perform registration or login
        reg.Reg();
        reg.getRole();
        

        if (reg.getRole() == "admin") {
            char menuChoice;
            do {
                cout << "\n=============================================" << endl;
                cout << "Welcome to Pakman's Skydiving Management System (Admin)" << endl;
                cout << "===============================================\n" << endl;
                cout << "[1] Skydivers" << endl;
                cout << "[2] Instructors" << endl;
                cout << "[3] Equipments" << endl;
                cout << "[4] Users" << endl;
                cout << "[5] Logout" << endl;  // Changed to "Logout" instead of "Exit"
                cout << "======================================\n" << endl;
                cout << "Enter your choice: ";
                cin >> menuChoice;

                switch (menuChoice) {
                    case '1':
                        sky.skydivers();
                        break;
                    case '2':
                        ins.instructor();
                        break;
                    case '3':
                        eq.equipment();
                        break;
                    case '4':
                        reg.Users();
                        break;
                    case '5':
                        cout << "Logging out..." << endl;
                        break;  // Breaks out of admin menu loop, returning to registration/login
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }

            } while (menuChoice != '5');
        } else if(reg.getRole() == "customer") {
            char menuChoice;
            do {
                cout << "\n=============================================" << endl;
                cout << "Welcome to Pakman's Skydiving Management System (Customer)" << endl;
                cout << "===============================================\n" << endl;
                cout << "[1] Skydivers" << endl;
                cout << "[2] Logout" << endl;  // Changed to "Logout" instead of "Exit"
                cout << "======================================\n" << endl;
                cout << "Enter your choice: ";
                cin >> menuChoice;

                switch (menuChoice) {
                    case '1':
                        sky.skydivers();
                        break;
                    case '2':
                        cout << "Logging out..." << endl;
                        break;  // Breaks out of customer menu loop, returning to registration/login
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }

            } while (menuChoice != '2');
        }

    } while (true);  // Loop indefinitely until user chooses to exit the program

    cout << "Goodbye!" << endl;
    return 0;
}
