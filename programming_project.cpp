#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

// Define a struct for storing borrower information
struct borrower {
	std::string borrowid;
	std::string lastname;
	std::string firstname;
	std::string number;
	int borrow;
};

using namespace std;

// Define a class for representing a book
class book{
private:
	string bookid;
	string title;
	string author;
	string publisher;
	int year;
	bool available;
	string borrower;
	int borrowCount = 0;

public:
	book(string bookid, string title, string author, string publisher, int year){
		this->bookid = bookid;
		this->title = title;
		this->author = author;
		this->publisher = publisher;
		this->year = year;
		this->available = true;
	}

	// Define getter methods for accessing private data members
	string getId() const{
		return bookid;
	}

	string getTitle() const{	
		return title;
	}

	string getAuthor() const{
		return author;
	}

	string getPublisher() const{
		return publisher;
	}

	int getYear() const{
		return year;
	}

	bool isAvailable() const{
		return available;
	}

	// Define a setter method for updating the availability status of a book
	void setAvailability(bool available){
		this->available = available;
	}

	string getBorrower(){
		return borrower;
	}

	// Define a setter method for updating the borrower information of a book
	void setBorrower(string borrower){
		this->borrower = borrower;
	}

	// Define a method for incrementing the borrow count of a book
	void incrementBorrowCount(){
		borrowCount++;
	}

	// Define a getter method for accessing the borrow count of a book
	int getBorrowCount() const{
		return borrowCount;
	}
};

// Define a function for extracting fields from a line in a CSV file
int extractFields(string line, char fields[][101]) {
	int numFields = 0;
	bool inQuotes = false;
	string field = "";

	for (size_t i = 0; i < line.length(); ++i) {
		char current = line[i];
		if (current == '"') {
			inQuotes = !inQuotes;
		}
		else if (current == ',' && !inQuotes) {
			strncpy_s(fields[numFields++], 101, field.c_str(), 100);
			field.clear();
		}
		else {
			field += current;
		}
	}
	if (!field.empty()) {
		strncpy_s(fields[numFields++], 101, field.c_str(), 100);
	}
	return numFields;
}

// Define a function for reading data from a CSV file
void readCSV(string filename) {
	fstream inFile;                // for handling file
	string line;                // for storing 1 line in a file
	char fields[10][101] = {};    // for storing extracted fields (assume max. 10 fields per line, each field has max. 100 char)
	int numFields;                // for storing number of fields per line
	int countRecords = 0;        // for counting the number of records in csv file

	inFile.open(filename);
	if (!inFile.is_open()) {
		cout << "Cannot open file "" << filename << ""\n";
		return;
	}

	while (getline(inFile, line, '\n')) {                // read line by line until end of file
		numFields = extractFields(line, fields);        // call function to extract fields from the line

		for (int i = 0; i < numFields; i++)                //  display the fields of this line
			cout << i << ": " << fields[i] << endl;        //  you should modify this code for fields processing
		cout << "\n\n";                                    // *** e.g. assign fields to data members of an object

		countRecords++;
	}

	cout << countRecords << " Record(s) imported.\n";
	inFile.close();
}

// Define a function for counting the number of lines in a file
int countLines(const string& filename) {
	ifstream inFile(filename);
	int count = 0;
	string line;

	if (!inFile.is_open()) {
		cout << "Cannot open file \"" << filename << "\"\n";
		return -1;
	}

	while (getline(inFile, line)) {
		count++;
	}

	inFile.close();
	return count;
}

// Define a function for reading borrower data from a CSV file
int readCSV_borrow(string filename, borrower*& borrowers) {
	int numLines = countLines(filename);
	if (numLines < 0) {
		return -1;
	}

	borrowers = new borrower[numLines];

	fstream inFile;
	string line;
	char fields[10][101] = {};
	int numFields;
	int countRecords = 0;

	inFile.open(filename);
	if (!inFile.is_open()) {
		cout << "Cannot open file \"" << filename << "\"\n";
		return -1;
	}

	while (getline(inFile, line, '\n')) {
		numFields = extractFields(line, fields);

		if (numFields >= 3) {
			stringstream id;// Generate a unique borrower ID for each borrower
			id << std::setw(4) << std::setfill('0') << countRecords;
			std::string in_id = id.str();

			// Create a new borrower object and populate its fields
			borrower b;
			b.borrowid = "HKCC" + in_id;
			b.lastname = fields[0];
			b.firstname = fields[1];
			b.number = fields[2];
			b.borrow = 0; // You can set this to a default value or read it from the CSV if available
			borrowers[countRecords] = b;
		}

		countRecords++;
	}

	cout << countRecords << " Record(s) imported.\n";
	inFile.close();

	// Print the borrower details
	for (int i = 0; i < countRecords; i++) {
		cout << "Borrower_ID:" << borrowers[i].borrowid
			<< ", Lastname: " << borrowers[i].lastname
			<< ", Firstname: " << borrowers[i].firstname
			<< ", Number: " << borrowers[i].number
			<< ", Borrow: " << borrowers[i].borrow << endl;
	}
	return countRecords;
}

// Define a function for finding a borrower by their borrower ID
int findBorrowerByUserId(const borrower* borrowers, int numBorrowers, const string& userId) {
	for (int i = 0; i < numBorrowers; i++) {
		if (borrowers[i].borrowid == userId) {
			return i;
		}
	}
	return -1; // If not found
}

// Define a function for finding a book by its book ID
int findBookBybookId(vector<book>& books, const string& userId) {

	vector<book>::iterator it;

	for (it = books.begin(); it != books.end(); it++) {
		if (it->getId() == userId) {
			return it - books.begin();
		}
	}
	return -1; // If not found
}

/// <summary>
/// file reading and editing
/// </summary>  
//fuctions for remove books into file.
void removebookbyid(char YorN) {//R1.4
	
	if (YorN == 'y' || YorN == 'Y') {
		cout << "Book removed successfully." << endl;
		cout << "************************ \n";
	}
	else if  (YorN == 'n' || YorN == 'N') {
		cout << "Book not removed." << endl;
		cout << "************************ \n";
	}
	
}

string ID, Title, Author, Publisher;
char confirmation;
//R1
void displayMenu(const vector<book>& books) {//R1
	int answer;
	int Book_ID;
	cout << "*** Manage Books *** \n"
		"[1] Display books \n"
		"[2] Search book \n"
		"[3] Add book \n"
		"[4] Remove book \n"
		"[5] Back \n"
		"************************ \n"
		"Option (1 - 5):";

	cin >> answer;
	string search_query;
	int num_books = sizeof(books) / sizeof(books[0]);
	while (true) {
		switch (answer) {
		case 1:
			cout << "ID         Title                          Author                        Publisher                     Year" << endl;
			cout << "------------------------------------------------------------------------------------------------------------" << endl;

			for (const book& b : books) {
				sort(b.getId().begin(), b.getId().end());
				cout << left << setw(11) << b.getId();
				cout << setw(30) << b.getTitle();
				cout << setw(30) << b.getAuthor();
				cout << setw(30) << b.getPublisher();
				cout << b.getYear() << endl;
			}
			cout << "*** Manage Books *** \n"
				"[1] Display books \n"
				"[2] Search book \n"
				"[3] Add book \n"
				"[4] Remove book \n"
				"[5] Back \n"
				"************************ \n"
				"Option (1 - 5):";
			cin >> answer;
			break;

		case 2: {
			//function of R1.2				
			cout << "Enter a string with a maximum of 50 characters: ";
			getline(cin, search_query);

			if (search_query.length() > 50) {
				cout << "Error: String exceeds maximum length of 50 characters." << endl;
				cout << "Enter a string with a maximum of 50 characters: ";
				getline(cin, search_query);
			}

			transform(search_query.begin(), search_query.end(), search_query.begin(), ::tolower);
			cout << "*** Manage Books *** \n"
				"[1] Display books \n"
				"[2] Search book \n"
				"[3] Add book \n"
				"[4] Remove book \n"
				"[5] Back \n"
				"************************ \n"
				"Option (1 - 5):";

			break;

		case 3:
			void addbook(vector<book>&books); {
				//Get details from user input
				string id, title, author, publisher;
				int year;
				cout << "Enter ID: ";
				getline(cin, id);
				cout << "Enter Title: ";
				getline(cin, title);
				cout << "Enter auther(s) separated by semicolon: ";
				getline(cin, author);
				cout << "Enter Publisher: ";
				getline(cin, publisher);
				cout << "Enter Year: ";
				cin >> year;

				//Check ID is unique
				for (const book& book : books) {
					cout << "Error: ID already exists.\n";
					return;
				}

				//Check year is positive
				if (year <= 0) {
					cout << "Error:Invalid year.\n";
					return;
				}

				// Create new book object and add to vector
				(id, title, author, publisher, year, true);
				cout << "Book added successfully." << endl;
			}
			cout << "*** Manage Books *** \n"
				"[1] Display books \n"
				"[2] Search book \n"
				"[3] Add book \n"
				"[4] Remove book \n"
				"[5] Back \n"
				"************************ \n"
				"Option (1 - 5):";
			cin >> answer;
			break;
		case 4:
			//function of R1.4
			cout << "To remove a book from the system, the user needs to provide the ID of the book." << endl;
			cin >> ID;

			cout << "*********************************\n";
			cout << "Do you want to remove this book? (y/n): ";
			cin >> confirmation;
			removebookbyid(confirmation);


			cout << "*** Manage Borrowers *** \n"
				"[1] Display borrowers \n"
				"[2] Search borrower \n"
				"[3] Add borrower \n"
				"[4] Remove borrower \n"
				"[5] Back \n"
				"************************ \n"
				"Option (1 - 5):";
			cin >> answer;
			break;
		case 5:
			//function of R1.5
			return;
			break;
		default:
			cout << "Enter number between 1-5 only \n"
				"********************************* \n"
				"Option(1 - 5) :";
			cin >> answer;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		}
	}
}

int numOfBooksRead = 0;// Initialize a variable to keep track of the number of books read
vector<book> books;// Declare a vector to store information about books

void readBookCSV(string filename){

	// Count the number of lines in the file
	int numLines = countLines(filename);

	// If the file is empty or cannot be opened, return
	if (numLines < 0){
		return;
	}

	// Open the file for reading
	fstream inFile;
	inFile.open(filename);

	// If the file cannot be opened, print an error message and return
	if (!inFile.is_open()){
		cout << "Cannot open file \"" << filename << "\"\n";
		return;
	}

	// Initialize variables
	string line;
	char fields[10][101] = {};// A 2D array to store the fields of each line
	int numFields;

	// Read each line from the file
	while (getline(inFile, line, '\n')){

		// Extract the fields from the line and count the number of fields
		numFields = extractFields(line, fields);

		// If the line has at least 5 fields, create a book object and add it to the vector of books
		if (numFields >= 5){

			// Convert the fields to the appropriate data types and create a book object
			std::string id(fields[0]);
			std::string title(fields[1]);
			std::string author(fields[2]);
			std::string publisher(fields[3]);
			int year = stoi(fields[4]);
			book b(id, title, author, publisher, year);
			books.push_back(b);
		}

		// Increment the number of books read
		numOfBooksRead++;
	}

	// Close the file
	inFile.close();

	// Print a message indicating how many records were read from the file
	cout << "Read " << numOfBooksRead << " records from file \"" << filename << "\"\n";
}

//R2.4
void removeBorrowerById(borrower*& borrowers, int& numBorrowers, const string& userId) {

	// Find the index of the borrower with the specified user ID
	int index = findBorrowerByUserId(borrowers, numBorrowers, userId);

	// If the borrower is found, remove them from the array of borrowers
	if (index >= 0) {

		// Create a new array of borrowers with one less element
		borrower* newBorrowers = new borrower[numBorrowers - 1];
		int j = 0;

		// Copy all borrowers except the one being removed to the new array
		for (int i = 0; i < numBorrowers; i++) {
			if (i != index) {
				newBorrowers[j++] = borrowers[i];
			}
		}

		// Delete the old array and replace it with the new one
		delete[] borrowers;
		borrowers = newBorrowers;
		numBorrowers--;

		// Print a message indicating that the borrower was successfully removed
		cout << "Borrower with user ID " << userId << " removed." << endl;
	}

	// If the borrower is not found, print an error message
	else {
		cout << "Borrower with user ID " << userId << " not found." << endl;
	}
}

bool compareBorrowers(const borrower& a, const borrower& b) {

	// Compare borrowers by last name first
	if (a.lastname == b.lastname) {

		// If the last name is the same, compare by first name
		return a.firstname < b.firstname;
	}

	// If the last names are different, compare by last name
	return a.lastname < b.lastname;
}

string generateBorrowerId(int numBorrowers) {

	// Generate a borrower ID based on the number of borrowers
	stringstream id;

	// Use setw and setfill to ensure the ID has 4 digits
	id << std::setw(4) << std::setfill('0') << numBorrowers;
	std::string in_id = id.str();

	// Add "HKCC" to the beginning of the ID
	return "HKCC" + in_id;
}

void addBorrower(borrower*& borrowers, int& numBorrowers, const string& borrowerId, const string& lastName, const string& firstName, const string& contactNumber) {
	
	// Create a new borrower with the specified information
	borrower newBorrower;
	newBorrower.borrowid = borrowerId;
	newBorrower.lastname = lastName;
	newBorrower.firstname = firstName;
	newBorrower.number = contactNumber;
	newBorrower.borrow = 0;

	// Resize the borrowers array
	borrower* temp = new borrower[numBorrowers + 1];
	for (int i = 0; i < numBorrowers; ++i) {
		temp[i] = borrowers[i];
	}
	temp[numBorrowers] = newBorrower;
	delete[] borrowers;
	borrowers = temp;

	// Increment the number of borrowers
	numBorrowers++;
}

//R2
void manageborrower(borrower*& borrowers, int& numBorrowers) {
	int answer;
	cout << "*** Manage Borrowers *** \n"
		"[1] Display borrowers \n"
		"[2] Search borrower \n"
		"[3] Add borrower \n"
		"[4] Remove borrower \n"
		"[5] Back \n"
		"************************ \n"
		"Option (1 - 5):";
	cin >> answer;
	string im;
	string searchid;
	int removeIndex;

	while (true) {
		switch (answer) {
		case 1:
			// Sort the borrowers array
			sort(borrowers, borrowers + numBorrowers, compareBorrowers);

			// Display the table header
			cout << "ID         Name                          Contact number  Number of books borrowed" << endl;
			cout << "--------------------------------------------------------------------------------" << endl;

			// Display the borrowers list
			for (int i = 0; i < numBorrowers; ++i) {
				cout << left << setw(11) << borrowers[i].borrowid
					<< setw(30) << (borrowers[i].lastname + ", " + borrowers[i].firstname)
					<< setw(16) << borrowers[i].number
					<< borrowers[i].borrow << endl;
			}

			// Display the manage borrowers menu again
			cout << "*** Manage Borrowers *** \n"
				"[1] Display borrowers \n"
				"[2] Search borrower \n"
				"[3] Add borrower \n"
				"[4] Remove borrower \n"
				"[5] Back \n"
				"************************ \n"
				"Option (1 - 5):";
			cin >> answer;
			break;

		case 2: {
			cout << "*********************************\n"
				"Search brower"
				"Borrower_ID:";
			cin >> searchid;

			// Find the borrower in the array
			int index = findBorrowerByUserId(borrowers, numBorrowers, searchid);

			if (index >= 0) {
				cout << "Borrower with user ID " << searchid << " found: " << endl;
				cout << "Borrower_ID:" << borrowers[index].borrowid
					<< ", Lastname: " << borrowers[index].lastname
					<< ", Firstname: " << borrowers[index].firstname
					<< ", Number: " << borrowers[index].number
					<< ", Borrow: " << borrowers[index].borrow << endl;
			}
			else {
				cout << "Borrower with user ID " << searchid << " not found." << endl;
			}

			// Display the manage borrowers menu again
			cout << "*** Manage Borrowers *** \n"
				"[1] Display borrowers \n"
				"[2] Search borrower \n"
				"[3] Add borrower \n"
				"[4] Remove borrower \n"
				"[5] Back \n"
				"************************ \n"
				"Option (1 - 5):";
			cin >> answer;
			break;
		}

		case 3: {
			string lastName, firstName, contactNumber;
			cout << "Enter the last name: ";
			cin >> lastName;
			cout << "Enter the first name: ";
			cin.ignore();
			getline(cin, firstName);
			cout << "Enter the contact number: ";
			cin >> contactNumber;

			// Convert last name to uppercase
			transform(lastName.begin(), lastName.end(), lastName.begin(), ::toupper);

			// Capitalize each word in first name
			stringstream ss(firstName);
			string word;
			firstName.clear();
			while (ss >> word) {
				word[0] = toupper(word[0]);
				for (size_t i = 1; i < word.length(); ++i) {
					word[i] = tolower(word[i]);
				}
				firstName += (firstName.empty() ? "" : " ") + word;
			}

			// Check if the contact number is valid
			if (contactNumber.length() == 8 && (contactNumber[0] == '2' || contactNumber[0] == '3' || contactNumber[0] == '5' || contactNumber[0] == '6' || contactNumber[0] == '9')) {
				
				// Generate a borrower ID and add the borrower to the array
				string borrowerId = generateBorrowerId(numBorrowers);
				addBorrower(borrowers, numBorrowers, borrowerId, lastName, firstName, contactNumber);
				cout << "Borrower has been successfully added with ID: " << borrowerId << endl;
			}
			else {
				cout << "Invalid contact number. Please try again." << endl;
			}
		}
			  // Display the manage borrowers menu again
			  cout << "*** Manage Borrowers *** \n"
				  "[1] Display borrowers \n"
				  "[2] Search borrower \n"
				  "[3] Add borrower \n"
				  "[4] Remove borrower \n"
				  "[5] Back \n"
				  "************************ \n"
				  "Option (1 - 5):";
			  cin >> answer;
			  break;

		case 4:
			// Prompt user to input borrower ID for removal
			cout << "*********************************\n"
				"Careful!!!Borrower Removing Mode \n "
				"Input the borrowerID:";
			cin >> im;

			// Find the [borrower index] by ID
			removeIndex = findBorrowerByUserId(borrowers, numBorrowers, im);

			// If borrower is found and has no books borrowed, remove the borrower
			if (removeIndex >= 0 && borrowers[removeIndex].borrow == 0) {
				removeBorrowerById(borrowers, numBorrowers, im);
				cout << "Borrower with ID " << im << " has been successfully removed." << endl;
			}

			// If borrower has books borrowed, do not remove and display error message
			else if (removeIndex >= 0) {
				cout << "Unable to delete (still have books not returned)" << endl;
			}

			// If borrower is not found, display error message
			else {
				cout << "Borrower not found" << endl;
			}

			// Display the manage borrowers menu again
			cout << "*** Manage Borrowers *** \n"
				"[1] Display borrowers \n"
				"[2] Search borrower \n"
				"[3] Add borrower \n"
				"[4] Remove borrower \n"
				"[5] Back \n"
				"************************ \n"
				"Option (1 - 5):";
			cin >> answer;
			break;

		case 5:
			// Return to previous menu
			return;
			break;

		default:
			// Display error message for invalid input and prompt user for proper input
			cout << "Enter number between 1-5 only \n"
				"********************************* \n"
				"Option(1 - 5) :";
			cin >> answer;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

//R3
void borrowBook(vector<book>& books, borrower* borrowers, int numBorrowers) {

	// Prompt the user to input a borrower ID
	string borrowerId;
	cout << "Please enter borrower ID (or 'q' to MainMenu): ";
	cin >> borrowerId;

	// Check if borrower ID is valid
	int borrowerIndex = findBorrowerByUserId(borrowers, numBorrowers, borrowerId);
	if (borrowerIndex < 0) {
		cout << "Invalid borrower ID." << endl;
		return;
	}

	// Check if borrower has quota to borrow more books
	int remainingQuota = 5 - borrowers[borrowerIndex].borrow;
	if (remainingQuota <= 0) {
		cout << "The borrower has used up their quota." << endl;
		return;
	}

	// Prompt the user for book ID to borrow
	int numBooksBorrowed = 0;
	bool continueBorrowing = true;
	while (remainingQuota > 0 && continueBorrowing) {
		string bookId;
		cout << "Please enter book ID to borrow (or 'q' to MainMenu): ";
		cin >> bookId;

		// If the user inputs 'q', stop borrowing
		if (bookId == "q") {
			break;
		}

		// Check if the book ID is valid
		int bookIndex = findBookBybookId(books, bookId);
		if (bookIndex < 0) {
			cout << "Invalid book ID." << endl;
			continue;
		}

		// Check if the book is available
		book& selectedBook = books[bookIndex];
		if (!selectedBook.isAvailable()) {
			cout << "The book is not available." << endl;
			continue;
		}

		// Update book availability and borrower information
		selectedBook.setAvailability(false);
		selectedBook.setBorrower(borrowerId);
		selectedBook.incrementBorrowCount();
		borrowers[borrowerIndex].borrow++;
		remainingQuota--;
		numBooksBorrowed++;

		// Prompt to continue borrowing
		if (remainingQuota > 0) {
			cout << "Successfully borrowed " << numBooksBorrowed << " book(s)." << endl;
			char response;
			cout << "Continue borrowing? (Y/N): ";
			cin >> response;
			if (response != 'Y' && response != 'y') {
				continueBorrowing = false;
			}
		}
	}
}

//R4
void returnbook(const string& file, borrower* borrowers, int numBorrowers, vector<book>& books, int numofbook) {

	// Prompt the user for the borrower ID
	string userId;
	string bookid;
	string ynn;

	do {
		cout << "*********************************\n"
			<< "*** Book Returning ***\n"
			<< "Please input your borrower_ID (or 'q' to MainMenu): ";
		cin >> userId;

		// Check if user wants to quit
		if (userId == "q") {
			break;
		}

		// Check if borrower ID is valid
		int index = findBorrowerByUserId(borrowers, numBorrowers, userId);
		while (index < 0) {
			cout << "Borrower with user ID " << userId << " not found." << endl
				<< "Please input your borrower_ID: ";
			cin >> userId;
		}

		// Display borrower information
		cout << "Borrower with user ID " << userId << " found: " << endl;
		cout << "Borrower_ID:" << borrowers[index].borrowid
			<< "Lastname: " << borrowers[index].lastname
			<< ", Firstname: " << borrowers[index].firstname
			<< ", Number: " << borrowers[index].number
			<< ", Borrow: " << borrowers[index].borrow << endl;

		// Prompt the user for the book ID
		cout << "Enter the returning book ID (or 'q' to MainMenu): ";
		cin >> bookid;// Prompt user for book ID

		// Check if user wants to quit
		if (bookid == "q") {
			break;
		}

		// Check if the borrower has borrowed the book
		vector<book>::iterator it;
		bool bookFound = false;
		for (it = books.begin(); it != books.end(); it++) {
			if (it->getId() == bookid) {
				if (it->getBorrower() == userId) {
					it->setBorrower("");
					it->setAvailability(true);
					borrowers[index].borrow -= 1;
					cout << "Book returned successfully." << endl;
				}
				else {
					cout << "Book not borrowed by this borrower." << endl;
				}
				bookFound = true;
				break;
			}
		}

		if (!bookFound) {
			cout << "Borrowed book not found" << endl;
		}

		// Prompt to return another book
		cout << "Do you want to enter another book ID? (Y/N): ";
		cin >> ynn;
	} while (ynn != "n" && ynn != "N");
}

vector<book> top_books(const vector<book>& books, int numBooks){

	// Sort the books based on their borrow count in descending order
	vector<book> sortedBooks(books.begin(), books.end());
	sort(sortedBooks.begin(), sortedBooks.end(), [](const book& a, const book& b) {
		return a.getBorrowCount() > b.getBorrowCount();
	});

	// Return the top 5 books or all the books if there are less than 5 books
	vector<book> top5Books(sortedBooks.begin(), sortedBooks.begin() + min(5, numBooks));
	return top5Books;
}

//R5
void print_top_books(const vector<book>& top5Books){

	// Check if there are any books to print
	if (top5Books.empty()) {
		cout << "Not found" << endl;
	}

	// Print the top 5 books and their borrow count
	else {
		for (size_t i = 0; i < top5Books.size(); ++i)
		{
			cout << (i + 1) << ". [" << top5Books[i].getId() << "] "
				<< top5Books[i].getTitle() << " has been borrowed for "
				<< top5Books[i].getBorrowCount() << " times." << endl;
		}
	}
}

//R6
void Member_list() {
	cout << "   Student name     Student ID   Class   Tutorial group" << endl;
	cout << "   CHEN Junjie       22017785A    201           C" << endl;
	cout << "  CHENG Wai Shing    22063403A    201           C" << endl;
	cout << " CHEUNG Chun Sing    22176446A    204           C" << endl;
	cout << "   CHAN KWOK HIN     22156066A    204           C" << endl;
	cout << "  CHEUNG LOK YIN     22166557A    204           C" << endl;
	cout << "    LA Yu Fung       22177271A    204           D" << endl;
}

int main() {
	string filename;
	string filename_borrow;
	char answer;
	int mode;
	borrower* borrowers = nullptr;
	int numBorrowers = 0;

	// Prompt the user if they want to import the book list from a file and read the CSV file
	cout << "Import book list from file? [Y/N]: ";
	cin >> answer;
	cin.ignore();
	if (tolower(answer) == 'y') {
		cout << "Input path to CSV file: ";
		getline(cin, filename);
		readCSV(filename);
		readBookCSV(filename);
	}
	else {
		cout << "No borrower list is imported \n";
	}
	
	// Prompt the user if they want to import the borrower list from a file and read the CSV file
	cout << "Import borrower list from file? [Y/N]: ";
	cin >> answer;
	cin.ignore();
	if (tolower(answer) == 'y') {
		cout << "Input path to CSV file: ";
		getline(cin, filename_borrow);
		numBorrowers = readCSV_borrow(filename_borrow, borrowers); // Assign the return value to numBorrowers
	}
	else {
		cout << "No borrower list is imported \n";
	}

	// Display the main menu and prompt the user for their choice
	cout << "*** Library Management System *** \n"
		"[1] Manage books \n"
		"[2] Manage borrowers \n"
		"[3] Borrow book(s) \n"
		"[4] Return book(s) \n"
		"[5] Popular books \n"
		"[6] Member List \n"
		"[7] Exit \n"
		"********************************* \n"
		"Option(1 - 7) :";
	cin >> mode;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// Get the top 5 most popular books
	vector<book> top5 = top_books(books, books.size());

	// Loop until the user chooses to exit
	while (true) {
		switch (mode) {
		case 1:
			// Manage books
			displayMenu(books);
			cout << "*** Library Management System *** \n"
				"[1] Manage books \n"
				"[2] Manage borrowers \n"
				"[3] Borrow book(s) \n"
				"[4] Return book(s) \n"
				"[5] popular book \n"
				"[6] Member List \n"
				"[7] Exit \n"
				"********************************* \n"
				"Option(1 - 7) :";
			cin >> mode;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			//
			break;

		case 2:
			// Manage borrowers
			manageborrower(borrowers, numBorrowers);

			// Display the main menu and prompt the user for their choice
			cout << "*** Library Management System *** \n"
				"[1] Manage books \n"
				"[2] Manage borrowers \n"
				"[3] Borrow book(s) \n"
				"[4] Return book(s) \n"
				"[5] Useful feature(s) added \n"
				"[6] Member List \n"
				"[7] Exit \n"
				"********************************* \n"
				"Option(1 - 7) :";
			cin >> mode;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;

		case 3:
			// Borrow books
			cout << "*********************************\n";
			cout << "*********Borrow book(s)**********\n";
			borrowBook(books, borrowers, numBorrowers);
			cout << "*********************************\n";

			// Display the main menu and prompt the user for their choice
			cout << "*** Library Management System *** \n"
				"[1] Manage books \n"
				"[2] Manage borrowers \n"
				"[3] Borrow book(s) \n"
				"[4] Return book(s) \n"
				"[5] Useful feature(s) added \n"
				"[6] Member List \n"
				"[7] Exit \n"
				"********************************* \n"
				"Option(1 - 7) :";
			cin >> mode;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;

		case 4:
			// Return books
			returnbook(filename_borrow, borrowers, numBorrowers, books, numOfBooksRead);

			// Display the main menu and prompt the user for their choice
			cout << "*** Library Management System *** \n"
				"[1] Manage books \n"
				"[2] Manage borrowers \n"
				"[3] Borrow book(s) \n"
				"[4] Return book(s) \n"
				"[5] popular book \n"
				"[6] Member List \n"
				"[7] Exit \n"
				"********************************* \n"
				"Option(1 - 7) :";
			cin >> mode;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;

		case 5:
			// Display the top 5 most popular books
			cout << "Top 5 most popular books: " << endl;
			print_top_books(top5);

			// Display the main menu and prompt the user for their choice
			cout << "*** Library Management System *** \n"
				"[1] Manage books \n"
				"[2] Manage borrowers \n"
				"[3] Borrow book(s) \n"
				"[4] Return book(s) \n"
				"[5] popular book \n"
				"[6] Member List \n"
				"[7] Exit \n"
				"********************************* \n"
				"Option(1 - 7) :";
			cin >> mode;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;

		case 6:
			// Display the member list
			cout << "*********************************\n";
			cout << "***********Member list***********\n";
			Member_list();
			cout << "*********************************\n";

			// Display the main menu and prompt the user for their choice
			cout << "*** Library Management System *** \n"
				"[1] Manage books \n"
				"[2] Manage borrowers \n"
				"[3] Borrow book(s) \n"
				"[4] Return book(s) \n"
				"[5] popular book \n"
				"[6] Member List \n"
				"[7] Exit \n"
				"********************************* \n"
				"Option(1 - 7) :";
			cin >> mode;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;

		case 7:
			// Prompt the user to confirm if they want to exit
			char confirmExit;
			do {
				cout << "Are you sure you want to exit? (Y/N): ";
				cin >> confirmExit;
				confirmExit = tolower(confirmExit);
				if (confirmExit == 'y') {
					exit(0);
				}

				// Prompt the user to input another option
				else if (confirmExit == 'n') {
					cout << "********************************* \n"
						"Option(1 - 7) :";
					cin >> mode;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}

				// Notify the user of invalid input and prompt to enter either Y or N
				else {
					cout << "Invalid input. Please enter 'Y' or 'N'.\n";
				}
			} while (confirmExit != 'y' && confirmExit != 'n');
			break;

		default:
			// Notify the user of invalid input and prompt to enter a number between 1-7
			cout << "Enter number between 1-7 only \n"
				"********************************* \n"
				"Option(1 - 7) :";
			cin >> mode;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return 0;
}