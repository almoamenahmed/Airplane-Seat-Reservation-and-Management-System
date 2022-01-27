#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
class Seat {
private:
	int row;
	int col;
	string value = "";
	string location = "";
public:
	// Default constructor/destructor
	Seat() {};
	~Seat() {};

	// Purpose: Constructor that takes a value and int/row
	Seat(const string v, const int r, const int c) {
		value = v;
		location = v;
		row = r;
		col = c;
	};

	// Purpose: Checks if the seat is reserved or not
	bool isReserved() const {
		if (value == "X") {
			return true;
		}
		return false;
	};

	// Purpose: Reserves a seat by marking it with an "X"
	void reserve() {
		value = "X";
	}

	// Purpose: getter function that returns the seat's location
	string getLocation() const {
		return location;
	}

	// Purpose: getter function that returns the seat's value
	string getValue() const {
		return value;
	}

	// Purpose: setter function that sets the seat's value
	void setValue(string v) {
		value = v;
	}

	// Purpose: getter function that returns the row of the seat object
	int getRow() const {
		return row;
	}

	// Purpose:  getter function that returns the col of the seat object
	int getCol() const {
		return col;
	}

	// Purpose: setter function that sets the row of the seat
	void setRow(const int r) {
		row = r;
	}

	// Purpose: setter function that sets the col of the seat
	void setCol(const int c) {
		col = c;
	}
};
class Plane {
private:
	const int ROWS = 15;
	const int COLS = 4;
	const int FILE_COLS = 10;
	// vector to hold seating data
	vector<vector<Seat>> data;

	// Purpose: Checks if a seat is not reserved, and if it isn't, reserves it. If not available, output a message to the user
	void reserve(Seat seat) {
		int i = seat.getRow();
		int j = seat.getCol();
		// check that it is not reserved
		if (!(data[i][j].isReserved())) {
			data[i][j].reserve();
			cout << "This seat is now reserved. Thank you!" << endl;
		}
		else { // if it is reserved
			cout << "Sorry, this seat is not available." << endl;
		}
	}

	Seat enterSeat() {
		string seat_str;
		Seat seat;
		seat.setRow(-1);
		seat.setCol(-1);
		cout << "Please enter a seat: " << endl;
		cin >> seat_str;
		// loop the 2d array
		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data[i].size(); j++) {
				// stop looping once we get to the seat that the user entered
				if (data[i][j].getLocation() == seat_str) {
					seat.setRow(i);
					seat.setCol(j);
					break;
				}
			}
		}
		return seat;
	}

public:

	Plane() {
		data.resize(15);
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				// initialize with empty seats
				data[i].push_back(Seat("", i, j));
			}
		}
	};
	~Plane() {};

	void readInChart() {
		string line;
		ifstream file("chartIn.txt");
		int i = 0;
		int row_num;
		if (file.is_open()) {
			while (getline(file, line)) {
				// slit up the line so that we get the number for the start
				// and the letters from each seat
				row_num = stoi(line.substr(0, 2));
				for (int j = 0; j < FILE_COLS; j++) { // always 10 cols
					if (j == 3) {
						data[i][0] = Seat(to_string(row_num) + line[j], i, 0);
					}
					if (j == 5) {
						data[i][1] = Seat(to_string(row_num) + line[j], i, 1);
					}
					if (j == 7) {
						data[i][2] = Seat(to_string(row_num) + line[j], i, 2);
					}
					if (j == 9) {
						data[i][3] = Seat(to_string(row_num) + line[j], i, 3);
					}
				}
				i++;
			}
		}
		file.close();
	};

	void displaySeatChart() {
		string seat;
		for (int i = 0; i < data.size(); i++) {
			// print num
			cout << i + 1 << " ";
			if (i < 9) {
				cout << " ";
			}
			// print letters
			for (int j = 0; j < data[i].size(); j++) {
				// cout << "SEAT: " << data[i][j].getValue() << endl;
				seat = data[i][j].getValue();
				// grab letter at the end
				seat = seat[seat.length() - 1];
				cout << seat;
				if (j != data[i].size()) {
					cout << " ";
				}
			}
			cout << endl;
		}
	};

	// Purpose: Marks a seat on the plane as reserved that is entered by the user
	void reserveSeat() {
		Seat seat = enterSeat();
		int i = seat.getRow();
		int j = seat.getCol();
		// check to make sure the user didn't try access a seat outside of the plane
		if (i == -1 && j == -1) {
			cout << "Oops! That seat does not exist. Please view the seating chart to determine what seats are available." << endl;
			return;
		}
		reserve(seat);
	}

	// Purpose: Marks a seat as no longer reserved, given a seat entered by the user
	void cancelReservation() {
		Seat seat = enterSeat();
		int i = seat.getRow();
		int j = seat.getCol();
		if (i == -1 && j == -1) {
			cout << "Oops! That seat does not exist. Please view the seating chart to determine what seats are available." << endl;
			return;
		}
		if (!(data[i][j].isReserved())) {
			cout << "Sorry, that seat has not already been reserved." << endl;
		}
		else {
			string letter = "";
			// get the letter for the index
			if (j == 0) { letter = "A"; }
			if (j == 1) { letter = "B"; }
			if (j == 2) { letter = "C"; }
			if (j == 3) { letter = "D"; }
			data[i][j].setValue((to_string(i) + letter));
			cout << "Your seat reservation has been canceled." << endl;
		}
	}

	// Purpose: Outputs the plane's data vector to a file in the same format as initially recieved
	void saveChartToFile() {
		string filename;
		string line = "";
		cout << "Please enter a filename: ";
		cin >> filename;
		ofstream file;
		file.open(filename);
		string seat;
		for (int i = 0; i < data.size(); i++) {
			// print the beginning number for the row
			file << i + 1 << " ";
			if (i < 9) {
				file << " ";
			}
			// print letters for each seat
			for (int j = 0; j < data[i].size(); j++) {
				seat = data[i][j].getValue();
				// grab letter at the end
				seat = seat[seat.length() - 1];
				file << seat;
				if (j != data[i].size()) {
					file << " "; // print space between letters
				}
			}
			file << '\n'; // print new line after each row
		}
		file.close();
		cout << "Successfully saved to " << filename << endl;
	}

	// Purpose: Prints to the console a group of stats based around how many seats have been reserved by the user.
	void displayStats() {
		int total_num_seats = data.size() * data[0].size(); // 15 * 4 = 60

		int total_num_available = total_num_seats;
		int window_seats_available = 0;
		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data[i].size(); j++) {
				if (data[i][j].isReserved()) { // if reserved
					total_num_available--;
				}
				else { // if available
					if (j == 0 || j == 3) { // first or last seat in row
						window_seats_available++;
					}
				}
			}
		}
		int isle_seats_available = total_num_available - window_seats_available;
		// Percent reserved, NOT AVAILABLE
		int total_num_reserved = total_num_seats - total_num_available;
		float percentage_reserved = (1.0 * total_num_reserved / total_num_seats) * 100;
		cout << "Number of available seats: " << total_num_available << endl;
		cout << "Percentage of seats that are reserved: " << percentage_reserved << endl;
		cout << "List of window seats that are available: " << window_seats_available << endl;
		cout << "List of isle seats that are available: " << isle_seats_available << endl;
	}
};

int main() {
	Plane plane;
	plane.readInChart();
	int input = -1;
	do {
		cout << "-------------------- Menu --------------------" << endl;
		cout << "1. Display Seat Chart" << endl;
		cout << "2. Reserve Seat" << endl;
		cout << "3. Cancel Reservation" << endl;
		cout << "4. Save Seat Chart to File" << endl;
		cout << "5. Statistics" << endl;
		cout << "6. Help" << endl;
		cout << "7. Quit" << endl;
		cout << "----------------------------------------------" << endl;
		cout << "Please Enter Your Choice (1-7):";
		cin >> input;
		switch (input)
		{
		case 1:
			plane.displaySeatChart();
			break;
		case 2:
			plane.reserveSeat();
			break;
		case 3:
			plane.cancelReservation();
			break;
		case 4:
			plane.saveChartToFile();
			break;
		case 5:
			plane.displayStats();
			break;
		case 6:
			cout << endl;
			cout << "How to use this program:" << endl;
			cout << endl;
			cout << "To use this program, please begin by selecting an option from the menu." << endl;
			cout << "You can choose from any of the options below" << endl;
			cout << "1. This option lets you see a seating chart of the airplane" << endl;
			cout << "2. This option will let you reserve a seat on the airplane" << endl;
			cout << "3. This option will allow you to cancel a previous reservation based on the seat number" <<
				endl;
			cout << "4. This option will let you save the seating chart to a file. (useful for printing for your flight!)"
				<< endl;
			cout << "5. This option lets you know useful details, like how many seats are remaining on the plane"
				<< endl;
			cout << "6. This option will bring up this help menu again!" << endl;
			cout << "7. Quit. This option will terminate this program." << endl;
			cout << endl;
			break;
		case 7:
			cout << "Good bye!" << endl;
			break;
		default:
			cout << "Invalid menu option!" << endl;
			break;
		}
	} while (input != 7);
	return 0;
}