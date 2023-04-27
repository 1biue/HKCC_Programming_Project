#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

struct borrower {
	std::string borrowid;
	std::string lastname;
	std::string firstname;
	std::string number;
	int borrow;
};

using namespace std;

class book
{
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
    book(string bookid, string title, string author, string publisher, int year)
    {
        this->bookid = bookid;
        this->title = title;
        this->author = author;
        this->publisher = publisher;
        this->year = year;
		this->available = true;
    }

    string getId() const
    {
        return bookid;
    }

    string getTitle() const
    {
        return title;
    }

    string getAuthor() const
    {
        return author;
    }

    string getPublisher() const
    {
        return publisher;
    }

    int getYear() const
    {
        return year;
    }

    bool isAvailable() const
    {
        return available;
    }

	void setAvailability(bool available)
	{
		this->available = available;
	}

    string getBorrower()
    {
        return borrower;
    }

    void setBorrower(string borrower)
    {
        this->borrower = borrower;
    }

	void incrementBorrowCount()
	{
		borrowCount++;
	}

	int getBorrowCount() const
	{
		return borrowCount;
	}
};



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
				stringstream id;
				id << std::setw(4) << std::setfill('0') << countRecords;
				std::string in_id = id.str();

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
			cout<< "Borrower_ID:" << borrowers[i].borrowid
				<< ", Lastname: " << borrowers[i].lastname
				<< ", Firstname: " << borrowers[i].firstname
				<< ", Number: " << borrowers[i].number
				<< ", Borrow: " << borrowers[i].borrow << endl;
		}


		return countRecords;
	}

	int findBorrowerByUserId(const borrower* borrowers, int numBorrowers, const string& userId) {
		for (int i = 0; i < numBorrowers; i++) {
			if (borrowers[i].borrowid == userId) {
				return i;
			}
		}
		return -1; // If not found
	}

	int findBookBybookId(vector<book>& books, const string& userId) {
		
		vector<book>::iterator it;

		for (it = books.begin(); it != books.end(); it++) {
			if (it -> getId() == userId) {
				return it - books.begin();
			}
		}

		return -1; // If not found
	}

	/// <summary>
	/// file reading and editing
	/// </summary>
	/// 
	bool contains(const book& b, const string& search_query) {
		string lowercase_search_query = search_query;
		transform(lowercase_search_query.begin(), lowercase_search_query.end(), lowercase_search_query.begin(), ::tolower);

		string lowercase_title = b.getTitle();
		string lowercase_author = b.getAuthor();
		string lowercase_publisher = b.getPublisher();
		transform(lowercase_title.begin(), lowercase_title.end(), lowercase_title.begin(), ::tolower);
		transform(lowercase_author.begin(), lowercase_author.end(), lowercase_author.begin(), ::tolower);
		transform(lowercase_publisher.begin(), lowercase_publisher.end(), lowercase_publisher.begin(), ::tolower);

		return (lowercase_title.find(lowercase_search_query) != string::npos) ||
			(lowercase_author.find(lowercase_search_query) != string::npos) ||
			(lowercase_publisher.find(lowercase_search_query) != string::npos);
	}


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
		string search_query = "example";
		int num_books = sizeof(books) / sizeof(books[0]);
		int Year;
		string ID, Title, Author, Publisher;
		while (true) {
			switch (answer) {
			case 1:
				cout << "ID         Title                          Author                        Publisher                     Year" << endl;
				cout << "------------------------------------------------------------------------------------------------------------" << endl;

				for (const book& b : books) {
					sort(b.getId().begin(), b.getId().end());
					cout << left << setw(11) << b.getId()
						<< setw(30) << b.getTitle()
						<< setw(30) << b.getAuthor()
						<< setw(30) << b.getPublisher()
						<< b.getYear() << endl;
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
					cout << "Please enter a string with a maximum of 50 characters: ";
					getline(cin, search_query);
				}

				transform(search_query.begin(), search_query.end(), search_query.begin(), ::tolower);

				vector<book> books; // Assuming this contains a list of books
				for (const auto& b : books) {
					if (contains(b, search_query)) {
						// The book contains the search query
						cout << "Searching result" << endl;
						cout << left << setw(11) << b.getId()
							<< setw(30) << b.getTitle()
							<< setw(30) << b.getAuthor()
							<< setw(30) << b.getPublisher()
							<< b.getYear() << endl;
					}
				}

				
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
				//function of R1.3				
				cout << "provide the following details: " << endl;
				cout << "Provide ID: \n";
				cin >> ID;
				cout << "Provide Title: \n";
				cin >> Title;
				cout << "Provide Author: \n";
				cin >> Author;
				cout << "Provide Publisher: \n";
				cin >> Publisher;


				for (int i = 0; i < num_books; i++) {
					if (ID.length() <= 10 && ID != books[i].getId()){
						
						
					}
					else {
						cout << "ID must be a string with maximum 10 characters and has to be unique." << endl;
					}
			
					if (Title.length() <= 100) {
						
					}
					else {
						cout << "Title must be a string with maximum 100 characters." << endl;
					}

					if (Author.length() <= 50){
						
					}
					else {
						cout << "Author must be a string with maximum 50 characters." << endl;
					}

					if ((Publisher.length() <= 50)) {
						
					}
					else {
						cout << "Publisher must be a string with maximum 50 characters." << endl;
					}

					if (Year >= 0) {
						
					}
					else {
						cout << "Year must be positive." << endl;
					}

						 
				break;
			case 4:
				//function of R1.4
				break;
			case 5:
				//function of R1.5
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

	void returnbook(const string& file, borrower* borrowers, int numBorrowers, vector<book>& books, int numofbook) {//R4
		string userId;
		string bookid;
		string ynn;

		do {
			cout << "*********************************\n"
				<< "*** Book Returning ***\n"
				<< "Please input your borrower_ID: ";
			cin >> userId;

			int index = findBorrowerByUserId(borrowers, numBorrowers, userId);

			while (index < 0) {
				cout << "Borrower with user ID " << userId << " not found." << endl
					<< "Please input your borrower_ID: ";
				cin >> userId;
			}
			cout << "Borrower with user ID " << userId << " found: " << endl;
			cout << "Borrower_ID:" << borrowers[index].borrowid
				<< "Lastname: " << borrowers[index].lastname
				<< ", Firstname: " << borrowers[index].firstname
				<< ", Number: " << borrowers[index].number
				<< ", Borrow: " << borrowers[index].borrow << endl;

			cout << "Enter the returning book ID: ";
			cin >> bookid;

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

			cout << "Do you want to enter another book ID? (Y/N): ";
			cin >> ynn;
		} while (ynn != "n" && ynn != "N");
	}




    int numOfBooksRead = 0;
	vector<book> books;

void readBookCSV(string filename)
{
    int numLines = countLines(filename);
    if (numLines < 0)
    {
        return;
    }

    fstream inFile;
    inFile.open(filename);

    if (!inFile.is_open())
    {
        cout << "Cannot open file \"" << filename << "\"\n";
        return;
    }

    string line;
    char fields[10][101] = {};
    int numFields;

    while (getline(inFile, line, '\n'))
    {
        numFields = extractFields(line, fields);

        if (numFields >= 5)
        {
			std::string id(fields[0]);
			std::string title(fields[1]);
			std::string author(fields[2]);
			std::string publisher(fields[3]);
			int year = stoi(fields[4]);
			book b(id, title, author, publisher, year);
            books.push_back(b);
        }

        numOfBooksRead++;
    }

    inFile.close();

    cout << "Read " << numOfBooksRead << " records from file \"" << filename << "\"\n";
}

void removeBorrowerById(borrower*& borrowers, int& numBorrowers, const string& userId) {//R2.4
	int index = findBorrowerByUserId(borrowers, numBorrowers, userId);

	if (index >= 0) {
		borrower* newBorrowers = new borrower[numBorrowers - 1];
		int j = 0;

		for (int i = 0; i < numBorrowers; i++) {
			if (i != index) {
				newBorrowers[j++] = borrowers[i];
			}
		}

		delete[] borrowers;
		borrowers = newBorrowers;
		numBorrowers--;

		cout << "Borrower with user ID " << userId << " removed." << endl;
	}
	else {
		cout << "Borrower with user ID " << userId << " not found." << endl;
	}
}

bool compareBorrowers(const borrower& a, const borrower& b) {
	if (a.lastname == b.lastname) {
		return a.firstname < b.firstname;
	}
	return a.lastname < b.lastname;
}

string generateBorrowerId(int numBorrowers) {
	stringstream id;
	id << std::setw(4) << std::setfill('0') << numBorrowers;
	std::string in_id = id.str();

	return "HKCC" + in_id;
}

void addBorrower(borrower*& borrowers, int& numBorrowers, const string& borrowerId, const string& lastName, const string& firstName, const string& contactNumber) {
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


void manageborrower(borrower*& borrowers, int& numBorrowers) {//R2
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
				string borrowerId = generateBorrowerId(numBorrowers);
				addBorrower(borrowers, numBorrowers, borrowerId, lastName, firstName, contactNumber);
				cout << "Borrower has been successfully added with ID: " << borrowerId << endl;
			}
			else {
				cout << "Invalid contact number. Please try again." << endl;
			}
		}
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
			cout << "*********************************\n"
				"Careful!!!Borrower Removing Mode \n "
				"Input the borrowerID:";
			cin >> im;
			removeIndex = findBorrowerByUserId(borrowers, numBorrowers, im);
			if (removeIndex >= 0 && borrowers[removeIndex].borrow == 0) {
				removeBorrowerById(borrowers, numBorrowers, im);
				cout << "Borrower with ID " << im << " has been successfully removed." << endl;
			}
			else if (removeIndex >= 0) {
				cout << "Unable to delete (still have books not returned)" << endl;
			}
			else {
				cout << "Borrower not found" << endl;
			}
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
	
	
void borrowBook(vector<book>& books, borrower* borrowers, int numBorrowers) {
	// Prompt for borrower ID
	string borrowerId;
	cout << "Please enter borrower ID: ";
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

	// Prompt for book IDs and borrow them
	int numBooksBorrowed = 0;
	while (remainingQuota > 0) {
		string bookId;
		cout << "Please enter book ID to borrow (or 'q' to quit): ";
		cin >> bookId;

		if (bookId == "q") {
			break;
		}

		int bookIndex = findBookBybookId(books, bookId);
		if (bookIndex < 0) {
			cout << "Invalid book ID." << endl;
			continue;
		}

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
	}

	if (numBooksBorrowed > 0) {
		cout << "Successfully borrowed " << numBooksBorrowed << " book(s)." << endl;
	}
	else {
		cout << "No books were borrowed." << endl;
	}
}

vector<book> top_books(const vector<book>& books, int numBooks)//R5 La Yu Fung
{
	vector<book> sortedBooks(books.begin(), books.end());
	sort(sortedBooks.begin(), sortedBooks.end(), [](const book& a, const book& b) {
		return a.getBorrowCount() > b.getBorrowCount();
		});

	vector<book> top5Books(sortedBooks.begin(), sortedBooks.begin() + min(5, numBooks));
	return top5Books;
}


void print_top_books(const vector<book>& top5Books)//R5 La Yu Fung
{
	if (top5Books.empty()) {
		cout << "Not found" << endl;
	}
	else {
		for (size_t i = 0; i < top5Books.size(); ++i)
		{
			cout << (i + 1) << ". [" << top5Books[i].getId() << "] "
				<< top5Books[i].getTitle() << " has been borrowed for "
				<< top5Books[i].getBorrowCount() << " times." << endl;
		}
	}
}

void Member_list() {
	cout << "   Student name     Student ID   Class   Tutorial group" << endl;
	cout << "   CHEN Junjie       22017785A    201           C" << endl;
	cout << "  CHENG Wai Shing    22063403A    201           C" << endl;
	cout << " CHEUNG Chun Sing    22176446A    204           C" << endl;
	cout << "    LA Yu Fung       22177271A    204           D" << endl;
}

	int main() {
		string filename;
		string filename_borrow;
		char answer;
		int mode;
		borrower* borrowers = nullptr;
		int numBorrowers = 0;

		cout << "Import book list from file? [Y/N]: ";
		cin >> answer;
		cin.ignore();
		if (tolower(answer) == 'y') {
			cout << "Input path to CSV file: ";
			getline(cin, filename);
			readCSV(filename);
		}
		else {
			cout << "No borrower list is imported \n";
		}
		//
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
		vector<book> top5 = top_books(books, books.size());

		while (true) {
			switch (mode) {
			case 1:
				//
				break;
			case 2:
				manageborrower(borrowers, numBorrowers);
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
				cout << "*********************************\n";
				cout << "*********Borrow book(s)**********\n";
				borrowBook(books, borrowers, numBorrowers);
				cout << "*********************************\n";
				break;
			case 4:
				returnbook(filename_borrow, borrowers, numBorrowers, books, numOfBooksRead);
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
				cout << "Top 5 most popular books: " << endl;
				print_top_books(top5);
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
				cout << "*********************************\n";
				cout << "***********Member list***********\n";
				Member_list();
				cout << "*********************************\n";
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
			case 7:
				char confirmExit;
				do {
					cout << "Are you sure you want to exit? (Y/N): ";
					cin >> confirmExit;
					confirmExit = tolower(confirmExit);
					if (confirmExit == 'y') {
						exit(0);
					}
					else if (confirmExit == 'n') {
						cout << "********************************* \n"
							"Option(1 - 7) :";
						cin >> mode;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}
					else {
						cout << "Invalid input. Please enter 'Y' or 'N'.\n";
					}
				} while (confirmExit != 'y' && confirmExit != 'n');
				break;
			default:
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
