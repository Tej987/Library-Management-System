#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Book {
private:
    int bookID;
    string title;
    string author;
    bool issued;

public:
    // Constructor
    Book(int id, string t, string a, bool isIssued = false) {
        bookID = id;
        title = t;
        author = a;
        issued = isIssued;
    }

    // Getters
    int getBookID() const { return bookID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }

    // Issue Book
    void issueBook() {
        if (!issued) {
            issued = true;
            cout << "Book issued successfully!\n";
        } else {
            cout << "Book is already issued!\n";
        }
    }

    // Return Book
    void returnBook() {
        if (issued) {
            issued = false;
            cout << "Book returned successfully!\n";
        } else {
            cout << "This book was not issued!\n";
        }
    }

    // Display Book Details
    void displayBook() const {
        cout << "Book ID: " << bookID << " | Title: " << title << " | Author: " << author
             << " | " << (issued ? "Issued" : "Available") << endl;
    }

    // Save to File
    void saveToFile(ofstream &outfile) const {
        outfile << bookID << "," << title << "," << author << "," << issued << endl;
    }

    // Load from File
    static Book loadFromFile(ifstream &infile) {
        int id;
        string title, author;
        bool issued;
        infile >> id;
        infile.ignore();
        getline(infile, title, ',');
        getline(infile, author, ',');
        infile >> issued;
        return Book(id, title, author, issued);
    }
};

// Function to check if book ID is unique
bool isBookIDUnique(vector<Book> &books, int id) {
    for (const auto &b : books) {
        if (b.getBookID() == id) {
            return false;
        }
    }
    return true;
}

// Function to find a book by ID
Book* findBook(vector<Book> &books, int id) {
    for (auto &b : books) {
        if (b.getBookID() == id) {
            return &b;
        }
    }
    return nullptr;
}

// Save books to file
void saveBooksToFile(vector<Book> &books) {
    ofstream outfile("books.txt", ios::trunc);
    for (const auto &b : books) {
        b.saveToFile(outfile);
    }
    outfile.close();
}

// Load books from file
void loadBooksFromFile(vector<Book> &books) {
    ifstream infile("books.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int id;
        string title, author;
        bool issued;

        infile >> id;
        infile.ignore();
        if (getline(infile, title, ',') && getline(infile, author, ',') && infile >> issued) {
            books.push_back(Book(id, title, author, issued));
        }
    }
    infile.close();
}

int main() {
    vector<Book> books;
    loadBooksFromFile(books);

    int choice;
    while (true) {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Add Book\n";
        cout << "2. View Books\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Search Book\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string title, author;
            cout << "Enter Book ID: ";
            cin >> id;
            cin.ignore();
            
            if (!isBookIDUnique(books, id)) {
                cout << "Error! Book ID already exists.\n";
                continue;
            }

            cout << "Enter Book Title: ";
            getline(cin, title);
            cout << "Enter Author Name: ";
            getline(cin, author);

            books.push_back(Book(id, title, author));
            saveBooksToFile(books);
            cout << "Book added successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== Available Books =====\n";
            for (const auto &b : books) {
                b.displayBook();
            }

        } else if (choice == 3) {
            int id;
            cout << "Enter Book ID to issue: ";
            cin >> id;
            Book* book = findBook(books, id);

            if (book) {
                book->issueBook();
                saveBooksToFile(books);
            } else {
                cout << "Book not found!\n";
            }

        } else if (choice == 4) {
            int id;
            cout << "Enter Book ID to return: ";
            cin >> id;
            Book* book = findBook(books, id);

            if (book) {
                book->returnBook();
                saveBooksToFile(books);
            } else {
                cout << "Book not found!\n";
            }

        } else if (choice == 5) {
            int id;
            cout << "Enter Book ID to search: ";
            cin >> id;
            Book* book = findBook(books, id);

            if (book) {
                book->displayBook();
            } else {
                cout << "Book not found!\n";
            }

        } else if (choice == 6) {
            cout << "Exiting Library System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
