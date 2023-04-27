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

    string getId()
    {
        return bookid;
    }

    string getTitle()
    {
        return title;
    }

    string getAuthor()
    {
        return author;
    }

    string getPublisher()
    {
        return publisher;
    }

    int getYear()
    {
        return year;
    }

    bool isAvailable()
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
				<< "Lastname: " << borrowers[i].lastname
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

	void returnbook(const string& file, borrower* borrowers, int numBorrowers, vector<book>& books, int numofbook) {//R4
		string userId;
		string bookid;
		cout << "*********************************\n"
			<< "*** Book Returning ***\n"
			<< "Please input your borrower_ID: ";
		cin >> userId;

		int index = findBorrowerByUserId(borrowers, numBorrowers, userId);

		if (index >= 0) {
			cout << "Borrower with user ID " << userId << " found: " << endl;
			cout << "Borrower_ID:" << borrowers[index].borrowid
				<< "Lastname: " << borrowers[index].lastname
				<< ", Firstname: " << borrowers[index].firstname
				<< ", Number: " << borrowers[index].number
				<< ", Borrow: " << borrowers[index].borrow << endl;
		}
		else {
			cout << "Borrower with user ID " << userId << " not found." << endl;
		}
		cout << "Enter the returning book ID: ";
		cin >> bookid;

		// Check if the borrower has borrowed the book
		vector<book>::iterator it;
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
				return;
			}
		}

		cout << "Do you want to enter another book ID? (Y/N): ";
		string ynn;
		cin >> ynn;
		if (ynn == "n") {
			return;
		}
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
	

	while (true) {
		switch (answer) {
		case 1:
			//
			break;
		case 2: {
			cout << "*********************************\n"
				"Search brower"
				"Borrower_ID";
			cin >> searchid;
			int index = findBorrowerByUserId(borrowers, numBorrowers, searchid);

			if (index >= 0) {
				cout << "Borrower with user ID " << searchid << " found: " << endl;
				cout << "Borrower_ID:" << borrowers[index].borrowid
					<< "Lastname: " << borrowers[index].lastname
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
			break;
		}
		case 3:

			break;
		case 4:
			cout << "*********************************\n"
				"Careful!!!Borrower Removing Mode \n "
				"Input the borrowerID:";
			cin >> im;
			removeBorrowerById(borrowers, numBorrowers, im);
			cout << "*** Manage Borrowers *** \n"
				"[1] Display borrowers \n"
				"[2] Search borrower \n"
				"[3] Add borrower \n"
				"[4] Remove borrower \n"
				"[5] Back \n"
				"************************ \n"
				"Option (1 - 5):";
			break;
		case 5:
		//
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
			"[5] Useful feature(s) added \n"
			"[6] Member List \n"
			"[7] Exit \n"
			"********************************* \n"
			"Option(1 - 7) :";
		cin >> mode;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		while (true) {
			switch (mode) {
			case 1:
				//
				break;
			case 2:
				manageborrower(borrowers, numBorrowers);
				break;
			case 3:
				cout << "*********************************\n";
				"Borrow book(s)";
				borrowBook(books, borrowers, numBorrowers);
				break;
			case 4:
				returnbook(filename_borrow, borrowers, numBorrowers, books, numOfBooksRead);
				break;
			case 5:
				//
				break;
			case 6:
				//
				break;
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

