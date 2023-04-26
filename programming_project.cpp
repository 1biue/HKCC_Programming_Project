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
    vector<Book> books = {
			{"CH6717212", "10 Little Indians", "Agatha Christie", "Pocket", 1982, true},
			{"NY8167261", "102 School Cafeteria Jokes", "Morgan Matthews", "Troll Communications Llc", 1992, true},
			{"MJ1586212", "2nd Chance", "James Patterson", "Time Warner Audio Major", 2002, true},
			{"MJ8118129", "52 Ways to Make a Difference", "Lynn Gordon", "Chronicle Books", 1996, true},
		};

		// Display books
	int main() {displayBooks(books);
		return 0;
	} 
	// Function to search for books based on keywords
	vector<Book> search_books(vector<Book>& books, string keywords) {
		vector<Book> result;

		// Split the input keywords into individual keywords
		vector<string> search_terms;
		string term;
		for (char c : keywords) {
			if (c == ' ' && !term.empty()) {
				search_terms.push_back(term);
				term = "";
			}
			else if (c == '"' && term.empty()) {
				// Phrase enclosed in double quotes
				auto end_pos = keywords.find('"', 1);
				if (end_pos == string::npos) {
					// If there is no ending double quote, treat the whole input as a single keyword
					term = keywords.substr(1);
				}
				else {
					term = keywords.substr(1, end_pos - 1);
				}
				search_terms.push_back(term);
				term = "";
				keywords.erase(0, end_pos + 1);
			}
			else {
				term += c;
			}
		}
		if (!term.empty()) {
			search_terms.push_back(term);
		}

		// Search for books that match the keywords
		for (auto& book : books) {
			bool is_match = false;
			for (auto& term : search_terms) {
				if (term.front() == '"' && term.back() == '"') {
					// Exact phrase search
					term = term.substr(1, term.length() - 2);
					if (book.id == term || book.title == term || book.author == term || book.publisher == term) {
						is_match = true;
						break;
					}
				}
				else {
					// Case insensitive keyword search
					transform(term.begin(), term.end(), term.begin(), ::tolower);
					if (book.id.find(term) != string::npos || book.title.find(term) != string::npos || book.author.find(term) != string::npos || book.publisher.find(term) != string::npos) {
						is_match = true;
						break;
					}
				}
			}
			if (is_match) {
				result.push_back(book);
			}
		}

		return result;
	}

	vector<Book> bookList;

	bool isIdUnique(string id) {
		for (Book book : bookList) {
			if (book.id == id) {
				return false;
			}
		}
		return true;
	}

	bool isYearPositive(int year) {
		if (year <= 0) {
			return false;
		}
		return true;
	}

	void addBook() {
		Book book;
		cout << "Enter ID (max 10 characters): ";
		cin >> book.id;
		if (!isIdUnique(book.id)) {
			cout << "Error: ID already exists.\n";
			return;
		}
		cout << "Enter title (max 100 characters): ";
		cin.ignore();
		getline(cin, book.title);
		cout << "Enter author(s) (max 50 characters each, separated by ';'): ";
		string authorsStr;
		getline(cin, authorsStr);
		string author;
		size_t pos = 0;
		while ((pos = authorsStr.find(";")) != string::npos) {
			author = authorsStr.substr(0, pos);
			if (author.length() > 50) {
				cout << "Error: Author name is too long.\n";
				return;
			}
			book.authors.push_back(author);
			authorsStr.erase(0, pos + 1);
		}
		if (authorsStr.length() > 50) {
			cout << "Error: Author name is too long.\n";
			return;
		}
		book.authors.push_back(authorsStr);
		cout << "Enter publisher (max 50 characters): ";
		getline(cin, book.publisher);
		cout << "Enter year: ";
		cin >> book.year;
		if (!isYearPositive(book.year)) {
			cout << "Error: Year must be a positive integer.\n";
			return;
		}
		book.available = true;
		bookList.push_back(book);
		cout << "Book added successfully!\n";
	}

	int main() {
		addBook();
		return 0;
	}



	int main() {
		// Example data
		vector<Book> books = {
			{"001", "C++ Programming", "John Smith", "ABC Publishing"},
			{"002", "Introduction to C++", "Jane Doe", "XYZ Press"},
			{"003", "Japanese Language for Beginners", "Tomoko Yamada", "PQR Books"},
			{"004", "C++ Language for Beginners", "Sam Lee", "XYZ Press"},
			{"005", "Introduction to Python", "Alice Wong", "ABC Publishing"}
		};

		// Ask the user to input the search keywords
		string keywords;
		cout << "Enter one or more keywords separated by space, and with or without double quotes:" << endl;
		getline(cin, keywords);

		// Search for books based on the search keywords
		auto result = search_books(books, keywords);

		// Display the search results
		cout << "Search results for \"" << keywords << "\":" << endl;
		for (auto& book : result) {
			cout << book.id << " | " << book.title << " | " << book.author << " | " << book.publisher << endl;
		}


	}
	
	Book* findBookById(string id) {
		for (Book& book : bookList) {
			if (book.id == id) {
				return &book;
			}
		}
		return nullptr;
	}

	void removeBook() {
		string id;
		cout << "Enter ID of book to remove: ";
		cin >> id;
		Book* bookPtr = findBookById(id);
		if (bookPtr == nullptr) {
			cout << "Error: Book not found.\n";
			return;
		}
		Book book = *bookPtr;
		if (!book.available) {
			cout << "Error: Book is not available.\n";
			return;
		}
		cout << "Book found!\n";
		cout << "ID: " << book.id << endl;
		cout << "Title: " << book.title << endl;
		cout << "Author(s): ";
		for (size_t i = 0; i < book.authors.size(); i++) {
			cout << book.authors[i];
			if (i != book.authors.size() - 1) {
				cout << "; ";
			}
		}
		cout << endl;
		cout << "Publisher: " << book.publisher << endl;
		cout << "Year: " << book.year << endl;
		char choice;
		cout << "Are you sure you want to delete this book? (Y/N): ";
		cin >> choice;
		if (choice == 'Y' || choice == 'y') {
			bookList.erase(bookList.begin() + (bookPtr - &bookList[0]));
			cout << "Book removed successfully!\n";
		}
		else {
			cout << "No book removed.\n";
		}
	}

	int main() {
		// assume bookList has been initialized with some books
		removeBook();
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
				exit(0);
				break;
			default:
				cout << "Enter number between 1-7 only \n"
					"********************************* \n"
					"Option(1 - 7) :";
				cin >> mode;
			}
		}
		return 0;
	}


