	#include <iostream>
	#include <fstream>	// for file input/output
	#include <string>
	#include <vector>

	using namespace std;


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
		fstream inFile;				
		string line;				
		char fields[10][101] = {};	
		int numFields;				
		int countRecords = 0;		

		inFile.open(filename);
		if (!inFile.is_open()) {
			cout << "Cannot open file \"" << filename << "\"\n";
			return;
		}

		while (getline(inFile, line, '\n')) {				
			numFields = extractFields(line, fields);		

			for (int i = 0; i < numFields; i++)				
				cout << i << ": " << fields[i] << endl;		
			cout << "\n\n";									

			countRecords++;
		}

		cout << countRecords << " Record(s) imported.\n";
		inFile.close();
	}

	class Borrower {
	private:
		string id;
		string name;
		int quota;
		vector<string> borrowedBooks;

	public:
		Borrower(string i_d, string n_ame) {
			i_d = id;
			n_ame = name;
			quota = 5; // Initialize quota to 5
		}

		// Get borrower ID
		string getId() {
			return id;
		}

		// Get borrower name
		string getName() {
			return name;
		}

		// Check if borrower has quota to borrow more books
		bool canBorrow() {
			return quota > 0;
		}

		// Get number of books borrowed by borrower
		int getNumBorrowed() {
			return borrowedBooks.size();
		}

		// Borrow a book
		void borrowBook(string bookId) {
			borrowedBooks.push_back(bookId);
			quota--;
		}

		// Print list of borrowed books
		void printBorrowedBooks() {
			cout << "Borrowed books:" << endl;
			for (string bookId : borrowedBooks) {
				cout << bookId << endl;
			}
		}
	};

	void return_book() {
		string borrowerID;
		cout << "Enter the borrower's ID: ";
		getline(cin, borrowerID);

		// Check if the borrower ID is valid and if the borrower has borrowed any books
		// If not, display an appropriate message and return

		string bookID;
		char userChoice;
		do {
			cout << "Enter the book ID: ";
			getline(cin, bookID);

			// Check if the borrower has borrowed the book
			// If yes, update the book's availability and remove the book from the borrower's list of borrowed books

			cout << "Do you want to enter another book ID? (Y/N): ";
			cin >> userChoice;
			cin.ignore();
		} while (tolower(userChoice) == 'y');
	}


	int main() {
		string filename;
		char answer;

		cout << "Import book list from file? [Y/N]: ";
		cin >> answer;
		cin.ignore();
		if (tolower(answer) == 'y') {
			cout << "Input path to CSV file: ";
			getline(cin, filename);
			readCSV(filename);
		}
		else {
			cout << "No borrower list is imported";
		}
		return 0;
	}


