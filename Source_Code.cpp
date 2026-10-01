/* Program: EECS 348 Assignment 3

Description: Uses a MaxHeap to prioritize emails based on the
sender category and date. Higher priority senders are placed
first, and newer emails have higher priority when the sender
categories are the same.

Inputs: Takes EMAIL, COUNT, NEXT, and READ commands. EMAIL
commands include the sender category, subject, and date.

Output: Displays the number of emails, the next highest
priority email, and removes emails when they are read.

Collaborators: None

Other Sources: ChatGPT

Author: Lane Lamping

Creation Date: 10/1/26

Revision Date: 10/1/26

Revisions: Changed MaxHeap storage from a list to a vector,
improved EMAIL command checking and whitespace trimming,
added checks for missing commas, and added comments
*/

#include <iostream> // Include input/output functionality
#include <fstream> // Include file stream functionality
#include <sstream> // Include string stream functionality
#include <string> // Include string functionality
#include <vector>   // REVISED: Changed from <list> to <vector> for direct index access.
#include <cctype>   // REVISED: Added <cctype> so isspace() can check different types of whitespace.

using namespace std; // Use standard namespace

// ChatGPT generated
// Class to hold individual email information
class Email {
private:
	string sender; // Store the sender category
	string subject; // Store the email subject
	string date; // Store the email date

public:
	Email() { // Default email construtor
		sender = ""; // Start sender as empty
		subject = ""; // Start subject as empty
		date = ""; // Start date as empty
	}

	Email(string senderCategory, string subjectLine, string emailDate) { // Constructor for a new email
		sender = senderCategory; // Save the sender category
		subject = subjectLine; // Save the subject
		date = emailDate; // Save the date
	}

	string getSender() const { // Get the sender category
		return sender; // Return the sender
	}

	string getSubject() const { // Get the subject
		return subject; // Return the subject
	}

	string getDate() const { // Get the date
		return date; // Return the date
	}

	// ChatGPT generated
	// Get the priority number for the sender category
	int getSenderPriority() const {
		if (sender == "Boss") // Check if the sender is the boss
			return 5; // Set Boss to highest priority
		else if (sender == "Subordinate") // Check if the sender is a subordinate
			return 4; // Set Subordinate to second highest priority
		else if (sender == "Peer") // Check if the sender is a peer
			return 3; // Set Peer to middle priority
		else if (sender == "ImportantPerson") // Check if the sender is an important person
			return 2; // Set ImportantPerson to second lowest priority
		else // Anything else is OtherPerson
			return 1; // Set OtherPerson to lowest priority
	}

	// ChatGPT generated
	// Convert the date to a number that can be compared
	int getDatePriority() const {
		int month = stoi(date.substr(0, 2)); // Get the month
		int day = stoi(date.substr(3, 2)); // Get the day
		int year = stoi(date.substr(6, 4)); // Get the year

		return year * 10000 + month * 100 + day; // Return the date as YYYYMMDD
	}

	// ChatGPT generated
	// Compare the priority of two emails
	bool hasHigherPriorityThan(const Email& other) const {
		if (getSenderPriority() != other.getSenderPriority()) { // Check if the sender priorities are different
			return getSenderPriority() > other.getSenderPriority(); // Return which sender has higher priority
		}

		return getDatePriority() > other.getDatePriority(); // If sender priorities match, compare dates
	}

	// ChatGPT generated, revised by me
	// Display the email information
	void display() const {
		cout << "\tSender: " << sender << endl; // REVISED: Prints the sender, tab added
		cout << "\tSubject: " << subject << endl; // REVISED: Prints the subject, tab added
		cout << "\tDate: " << date << endl; // REVISED: Prints the date, tab added
	}
};

// ChatGPT generated, revised by me
// Class for the MaxHeap priority queue
class MaxHeap {
private:
	vector<Email> heap;   // REVISED: Changed heap storage from list to vector for direct indexing.

	// REVISED: Removed getIterator() because vector elements can be accessed directly by index.

	// ChatGPT generated, revised by me
	// Swap two emails in the heap
	void swapEmails(int first, int second) {
		Email temp = heap[first];       // REVISED: Directly accesses the first vector element instead of using an iterator.
		heap[first] = heap[second];      // REVISED: Directly accesses both vector positions instead of dereferencing iterators.
		heap[second] = temp;             // REVISED: Directly stores the temporary Email at the second vector position.
	}

	// ChatGPT generated, revised by me
	// Move an emil upward if it has higher priority
	void heapifyUp(int index) {
		while (index > 0) { // Keep looping while the email has a parent
			int parent = (index - 1) / 2; // Find the parent position

			if (heap[index].hasHigherPriorityThan(heap[parent])) {   // REVISED: Compares child and parent directly by vector index instead of using iterators.
				swapEmails(index, parent); // Swap the email with its parent
				index = parent; // Move to the parent position
			}
			else {
				break; // Stop if the email is in the correct position
			}
		}
	}

	// ChatGPT generated, revised by me
	// Move an email downward if it has lower priority
	void heapifyDown(int index) {
		int size = heap.size(); // Store the number of emails in the heap

		while (true) { // Keep looping until the email is in the right place
			int leftChild = 2 * index + 1; // Find the left child
			int rightChild = 2 * index + 2; // Find the right child

			int highest = index;   // REVISED: Formatting changed by separating this declaration from the child calculations.

			if (leftChild < size) { // Check if the left child exists
				if (heap[leftChild].hasHigherPriorityThan(heap[highest])) {   // REVISED: Directly compares vector elements instead of creating list iterators.
					highest = leftChild; // Make the left child the highest priority
				}
			}

			if (rightChild < size) { // Check if the right child exists
				if (heap[rightChild].hasHigherPriorityThan(heap[highest])) {   // REVISED: Directly compares vector elements instead of creating list iterators.
					highest = rightChild; // Make the right child the highest priority
				}
			}

			if (highest != index) { // Check if a child has higher priority
				swapEmails(index, highest); // Swap with the higher priority child
				index = highest; // Keep checking from the new position
			}
			else {
				break; // Stop when the email is in the correct position
			}
		}
	}

public:
	MaxHeap() { // MaxHeap constructor
	}

	// ChatGPT generated
	// Insert a new email into the heap
	void insert(const Email& email) {
		heap.push_back(email); // Add the email to the end of the heap

		int newIndex = heap.size() - 1; // Get the new email's position

		heapifyUp(newIndex); // Move the email up if needed
	}

	// ChatGPT generated, revised by me
	// Get the highest priority email without removing it
	Email* getMax() {
		if (heap.empty()) { // Check if the heap is empty
			return nullptr; // Return nothing if there are no emails
		}

		return &heap[0];   // REVISED: Uses vector index 0 instead of list front() to access the root.
	}

	// ChatGPT generated, revised by me
	// Remove the highst priority email
	void removeMax() {
		if (heap.empty()) { // Check if the heap is empty
			return; // Do nothing if there are no emails
		}

		if (heap.size() == 1) { // Check if there is only one email
			heap.pop_back();   // REVISED: Uses pop_back() because the heap is now stored in a vector.
			return; // Stop after removing the email
		}

		heap[0] = heap.back();   // REVISED: Replaces the root directly using vector index 0 instead of list front().

		heap.pop_back(); // Remove the last email

		heapifyDown(0); // Move the new top email down if needed
	}

	// ChatGPT generated
	// Get the number of emails in the heap
	int getCount() const {
		return heap.size(); // Return the number of emails
	}

	// ChatGPT generated
	// Check if the heap is empty
	bool isEmpty() const {
		return heap.empty(); // Return whether there are no emails
	}
};

// ChatGPT generated, revised by me
// Class to handle email commands and the inbox
class EmailPrioritySystem {
private:
	MaxHeap inbox; // Heap that stores the emails

	// ChatGPT generated, revised by me
	// Remove extra whitespace from the beginning and end
	string trim(string text) {
		while (!text.empty() &&                                      // REVISED: Condition is split across lines and now checks all whitespace.
		        isspace(static_cast<unsigned char>(text.front()))) {   // REVISED: Uses isspace() instead of checking only for the ' ' character.
			text.erase(text.begin()); // Remove whitespace from the beginning
		}

		while (!text.empty() &&                                     // REVISED: Condition is split across lines and now checks all whitespace.
		        isspace(static_cast<unsigned char>(text.back()))) {   // REVISED: Uses isspace() instead of checking only for the ' ' character.
			text.pop_back(); // Remove whitespace from the end
		}

		return text; // Return the trimmed text
	}

	// ChatGPT generated, revised by me
	// Process an EMAIL command
	void processEmail(string line) {
		string information = line.substr(6); // Remove "EMAIL " from the line

		size_t firstComma = information.find(',');   // REVISED: Uses size_t for the position returned by find().

		if (firstComma == string::npos) {   // REVISED: Checks whether the first required comma was actually found.
			return;                         // REVISED: Stops processing malformed input if the first comma is missing.
		}                                   // REVISED: Ends the new first-comma validation block.

		size_t secondComma = information.find(',', firstComma + 1);   // REVISED: Uses size_t for the second comma position.

		if (secondComma == string::npos) {   // REVISED: Checks whether the second required comma was actually found.
			return;                          // REVISED: Stops procesing malformed input if the second comma is missing.
		}                                    // REVISED: Ends the new second-comma validation block.

		string sender = information.substr(0, firstComma); // Get the sender category

		string subject = information.substr( // Get the subject
		                     firstComma + 1, // Start after the first comma
		                     secondComma - firstComma - 1 // Stop before the second comma
		                 );

		string date = information.substr(secondComma + 1); // Get the date

		sender = trim(sender); // Remove extra whitespace from the sender
		subject = trim(subject); // Remove extra whitespace from the subject
		date = trim(date); // Remove extra whitespace from the date

		Email newEmail(sender, subject, date); // Create a new email

		inbox.insert(newEmail); // Add the email to the heap
	}

	// ChatGPT generated
	// Process a NEXT command
	void processNext() {
		if (inbox.isEmpty()) { // Check if the inbox is empty
			return; // Do nothing if there are no emails
		}

		Email* nextEmail = inbox.getMax(); // Get the highest priority email

		cout << "Next email:" << endl; // Print the header

		nextEmail->display(); // Display the highest priority email

		cout << endl; // Print an extra line
	}

	// ChatGPT generated
	// Process a READ command
	void processRead() {
		if (inbox.isEmpty()) { // Check if the inbox is empty
			return; // Do nothing if there are no emails
		}

		inbox.removeMax(); // Remove the highest priority email
	}

	// ChatGPT generated
	// Process a COUNT command
	void processCount() {
		cout << "There are " // Start the count output
		     << inbox.getCount() // Print the number of emails
		     << " emails to read." // Finish the count output
		     << endl // Move to the next line
		     << endl; // Add an extra blank line
	}

public:
	EmailPrioritySystem() { // EmailPrioritySystem constructor
	}

	// ChatGPT generated, revised by me
	// Determine which command was entered
	void processCommand(string line) {
		if (!line.empty() && line.back() == '\r') { // Check for a carriage return
			line.pop_back(); // Remove the carriage return
		}

		if (line.rfind("EMAIL ", 0) == 0) {   // REVISED: Requires the line to begin with exactly "EMAIL " instead of accepting anything beginning with "EMAIL".
			processEmail(line); // Process the EMAIL command
		}
		else if (line == "NEXT") { // Check if the command is NEXT
			processNext(); // Process the NEXT command
		}
		else if (line == "READ") { // Check if the command is READ
			processRead(); // Process the READ command
		}
		else if (line == "COUNT") { // Check if the command is COUNT
			processCount(); // Process the COUNT command
		}
	}

	// ChatGPT generated
	// Read and process commands from a file
	void processFile(string filename) {
		ifstream inputFile(filename); // Open the input file

		if (!inputFile.is_open()) { // Check if the file opened
			cout << "Unable to open file." << endl; // Print an error message
			return; // Stop if the file could not open
		}

		string line; // Store each input line

		while (getline(inputFile, line)) { // Read lines until the end of the file
			if (!line.empty()) { // Make sure the line is not empty
				processCommand(line); // Process the command
			}
		}

		inputFile.close(); // Close the input file
	}
};

// ChatGPT generated
// Main function
int main(int argc, char* argv[]) {
	EmailPrioritySystem system; // Create the email priority system

	if (argc > 1) { // Check if a filename was given
		system.processFile(argv[1]); // Process commands from the file
	}
	else { // Otherwise read from standard input
		string line; // Store each input line

		while (getline(cin, line)) { // Keep reding until there is no more input
			if (!line.empty()) { // Make sure the line is not empty
				system.processCommand(line); // Process the command
			}
		}
	}

	return 0; // End the program successfully
}