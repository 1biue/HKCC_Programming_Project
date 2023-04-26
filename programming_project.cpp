	#include <iostream>
	#include <fstream>	// for file input/output
	#include <string>
	#include <vector>
    #include <algorithm>
    #include <sstream>

	using namespace std;
	
	struct Book {
		string id;
		string title;
		string author;
		string publisher;
		int year;
		bool available;
	};
	bool compareTitle(const Book& book1, const Book& book2) {
		return book1.title < book2.title;
	}
	void displayBooks(const vector<Book>& books) {
		cout << setw(12) << "ID" << setw(50) << "Book Details" << setw(15) << "Availability" << endl;
		for (const auto& book : books) {
			cout << setw(12) << book.id << setw(50) << book.title + " by " + book.author + ", " + book.publisher + ", " + to_string(book.year) << setw(15) << (book.available ? "Available" : "Not available") << endl;
		}
	}

	int main() {
		vector<Book> books = {
			{"CH6717212", "10 Little Indians", "Agatha Christie", "Pocket", 1982, true},
			{"NY8167261", "102 School Cafeteria Jokes", "Morgan Matthews", "Troll Communications Llc", 1992, true},
			{"MJ1586212", "2nd Chance", "James Patterson", "Time Warner Audio Major", 2002, true},
			{"MJ8118129", "52 Ways to Make a Difference", "Lynn Gordon", "Chronicle Books", 1996, true},
		};

		// Display books
		displayBooks(books);

		return 0;
	}

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

	int readline(string filename, string info) {//modify form above
		fstream inFile;
		string line;
		char fields[10][101] = {};
		int numFields;
		int countRecords = 0;
		inFile.open(filename);
		if (!inFile.is_open()) {
			cout << "Cannot open file \"" << filename << "\"\n";
		}
		while (getline(inFile, line, '\n')) {
			numFields = extractFields(line, fields);
			if (numFields > 0 && strcmp(fields[2], info.c_str()) == 0) {
				for (int i = 0; i < numFields; i++)
					cout << i << ": " << fields[i] << endl;
				cout << "\n";
				countRecords++;
			}
		}
		if (countRecords == 0) {
			cout << "No_record";
		}
		else {
			printf(" %i record(s) found by %s \n", countRecords, info.c_str());
		}
		inFile.close();
		return countRecords;
	}

	void add_data(const string& adding_info, const string& user_id, const string& filename) {
		fstream inFile;
		fstream outFile;
		string line;
		char fields[10][101] = {};
		int numFields;
		bool found = false;

		inFile.open(filename);
		if (!inFile.is_open()) {
			cout << "Cannot open file \"" << filename << "\"\n";
			return;
		}

		string outputFilename = "temp_" + filename;
		outFile.open(outputFilename, ios::out);
		if (!outFile.is_open()) {
			cout << "Cannot create temporary file \"" << outputFilename << "\"\n";
			inFile.close();
			return;
		}

		while (getline(inFile, line, '\n')) {
			numFields = extractFields(line, fields);
			if (numFields > 0 && strcmp(fields[2], user_id.c_str()) == 0) {
				found = true;
				line += "," + adding_info;
			}
			outFile << line << "\n";
		}

		inFile.close();
		outFile.close();

		if (!found) {
			cout << "No record found with user ID: " << user_id << "\n";
			remove(outputFilename.c_str());
		}
		else {
			remove(filename.c_str());
			if (rename(outputFilename.c_str(), filename.c_str()) != 0) {
				cout << "Failed to update the file with the new data.\n";
			}
			else {
				cout << "Added data to user ID: " << user_id << "\n";
			}
		}

	}

	void remove_data(const string& user_input, const string& user_id, const string& filename) {
		fstream inFile;
		fstream outFile;
		string line;
		char fields[10][101] = {};
		int numFields;
		bool found = false;

		inFile.open(filename);
		if (!inFile.is_open()) {
			cout << "Cannot open file \"" << filename << "\"\n";
			return;
		}

		string outputFilename = "temp_" + filename;
		outFile.open(outputFilename, ios::out);
		if (!outFile.is_open()) {
			cout << "Cannot create temporary file \"" << outputFilename << "\"\n";
			inFile.close();
			return;
		}

		while (getline(inFile, line, '\n')) {
			numFields = extractFields(line, fields);
			string updatedLine = "";
			if (numFields > 0 && strcmp(fields[2], user_id.c_str()) == 0) {
				for (int i = 0; i < numFields; i++) {
					if (strcmp(fields[i], user_input.c_str()) != 0) {
						updatedLine += string(fields[i]) + (i < numFields - 1 ? "," : "");
					}
				}
				if (updatedLine.length() != line.length()) {
					found = true;
				}
			}
			else {
				updatedLine = line;
			}
			outFile << updatedLine << "\n";
		}

		inFile.close();
		outFile.close();

		if (!found) {
			cout << "No record found with user ID: " << user_id << " and user input: " << user_input << "\n";
			remove(outputFilename.c_str());
		}
		else {
			remove(filename.c_str());
			if (rename(outputFilename.c_str(), filename.c_str()) != 0) {
				cout << "Failed to update the file with the new data.\n";
			}
			else {
				cout << "Removed data from user ID: " << user_id << "\n";
			}
		}
	}

	/// <summary>
	/// file reading and editing
	/// </summary>

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

	void return_book(string file) {
		string borrowerID;
		cout << "Enter the borrower's ID: ";
		cin >> borrowerID;

		// Check if the borrower ID is valid and if the borrower has borrowed any books
		int numRecords = readline(file, borrowerID);
		if (numRecords == 0) {
			cout << "No record found for borrower ID " << borrowerID << endl;
			return;
		}
		// If not, display an appropriate message and return

		string bookID;
		char userChoice;
		do {
			cout << "Enter the returning book ID: ";
			cin >> bookID;

			// Check if the borrower has borrowed the book
			remove_data(bookID, borrowerID, file);
			// If yes, update the book's availability and remove the book from the borrower's list of borrowed books

			cout << "Do you want to enter another book ID? (Y/N): ";
			cin >> userChoice;
			cin.ignore();
		} while (tolower(userChoice) == 'y');
	}


	int main() {
		string filename;
		string filename_borrow;
		char answer;
		int mode;

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
			readCSV(filename_borrow);
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
				//
				break;
			case 3:
				//
				break;
			case 4:
				return_book(filename_borrow);
				cout << "********************************* \n"
					"Option(1 - 7) :";
				cin >> mode;
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


