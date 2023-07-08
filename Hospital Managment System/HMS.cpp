#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class Person {
protected:
    string name;

public:
    Person(const string& name) : name(name) {}

    const string& getName() const {
        return name;
    }

    virtual void displayDetails() const {
        cout << "Name: " << name << endl;
    }
};

class Doctor : public Person {
private:
    vector<bool> availability;
    double price; // Price per appointment

public:
    Doctor(const string& name, const vector<bool>& availability, double price) : Person(name), availability(availability), price(price) {}

    bool isAvailable(int time) const {
        return availability[time - 1];
    }

    void bookAppointment(int time) {
        availability[time - 1] = false;
    }

    double getPrice() const {
        return price;
    }

    void displayDetails() const override {
        cout << "Name: " << name << endl;
        cout << "Availability: ";
        for (bool avail : availability) {
            cout << (avail ? "1" : "0") << " ";
        }
        cout << endl;
        cout << "Price per appointment: " << price << endl;
    }
};

class Bed {
private:
    string infection;
    int availableBeds;
    double price; // Price per day
    static int totalBeds;

public:
    Bed(const string& infection, int availableBeds, double price) : infection(infection), availableBeds(availableBeds), price(price) {}

    const string& getInfection() const {
        return infection;
    }

    int getAvailableBeds() const {
        return availableBeds;
    }

    void setTotalBeds(int totalBed){
        totalBeds = totalBed;
    }

    void bookBed() {
        availableBeds--;
    }

    double getPrice() const {
        return price;
    }

    void displayDetails() const {
        cout << "Infection: " << infection << endl;
        cout << "Available beds: " << availableBeds << endl;
        cout << "Price per day: " << price << endl;
    }
    friend class Hospital;
};

class Hospital {
private:
    static vector<Doctor> doctors;
    static vector<Bed> beds;
    static vector<pair<int, string>> bookedAppointments; // <time, doctorName>

public:
    static void displayAvailableDoctors(int time) {
        if (time >= 1 && time <= 10) {
            cout << "Available doctors at " << time << " pm:" << endl;
            for (const auto& doctor : doctors) {
                if (doctor.isAvailable(time)) {
                    cout << doctor.getName() << endl;
                }
            }
        } else {
            cout << "Invalid time! Please enter a time between 1 pm and 10 pm." << endl;
        }
    }

    static void bookDoctorAppointment(int time, const string& doctorName) {
        if (time >= 1 && time <= 10) {
            for (auto& doctor : doctors) {
                if (doctor.getName() == doctorName && doctor.isAvailable(time)) {
                    doctor.bookAppointment(time);
                    bookedAppointments.push_back(make_pair(time, doctorName));
                    cout << "Appointment booked with " << doctor.getName() << " at " << time << " pm." << endl;
                    return;
                }
            }
            cout << "Doctor not found or not available at the specified time." << endl;
        } else {
            cout << "Invalid time! Please enter a time between 1 pm and 10 pm." << endl;
        }
    }

    static void displayAvailableBeds(const string& infection) {
        for (const auto& bed : beds) {
            if (bed.getInfection() == infection) {
                cout << "Available beds for " << infection << ": " << bed.getAvailableBeds() << endl;
                return;
            }
        }
        cout << "No beds found for the specified infection." << endl;
    }

    static void bookBed(const string& infection) {
        for (auto& bed : beds) {
            if (bed.getInfection() == infection && bed.getAvailableBeds() > 0) {
                bed.bookBed();
                cout << "Bed booked for " << infection << "." << endl;
                return;
            }
        }
        cout << "No beds available for the specified infection." << endl;
    }

    static double calculateAppointmentBill(int days) {
        double totalBill = 0.0;
        if (bookedAppointments.empty()) {
            cout << "Please book a Doctor to calculate your bill." << endl;
            return totalBill;
        }

        for (const auto& appointment : bookedAppointments) {
            for (const auto& doctor : doctors) {
                if (doctor.getName() == appointment.second) {
                    cout << "There was an Appointment fixed with " << appointment.second << " at time " << appointment.first << " pm" << endl;
                    totalBill += doctor.getPrice();
                    break;
                }
            }
        }

        totalBill *= days;
        return totalBill;
    }

    static double calculateBedBill(int days) {
        double totalBill = 0.0;
        bool bedBooked = false;

        for (const auto& bed : beds) {
            if (bed.getAvailableBeds() < bed.totalBeds) {
                totalBill += bed.getPrice();
                bedBooked = true;
            }
        }

        if (!bedBooked) {
            cout << "Please book a Bed to calculate your bill." << endl;
            return totalBill;
        }

        totalBill *= days;
        return totalBill;
    }

    static void displayMenu() {
        cout << "========== Hospital Management System ==========" << endl;
        cout << "1. Display available doctors at a specific time" << endl;
        cout << "2. Book a doctor appointment" << endl;
        cout << "3. Display available beds for a specific infection" << endl;
        cout << "4. Book a bed" << endl;
        cout << "5. Calculate and display the bill" << endl;
        cout << "6. Display details of availability" << endl;
        cout << "7. Exit" << endl;
    }

    static void displayAllDetails() {
        cout << "========== Hospital Details ==========" << endl;
        cout << "Doctors:" << endl;
        for (const auto& doctor : doctors) {
            doctor.displayDetails();
            cout << endl;
        }
        cout << "Beds:" << endl;
        for (const auto& bed : beds) {
            bed.displayDetails();
            cout << endl;
        }
    }
};

// Initialize the static vectors in Hospital
vector<Doctor> Hospital::doctors = {
    Doctor("Dr. Aryan", {true, true, false, true, false, true, true, true, true, true}, 1000.0),
    Doctor("Dr. Jay", {true, false, true, false, true, true, true, true, true, true}, 1500.0),
    Doctor("Dr. Praveen", {true, true, true, true, true, true, true, true, true, true}, 2000.0),
    Doctor("Dr.Atharva", {true, false, true, false, true, true, false, true, true, false}, 2500.0)
};

vector<Bed> Hospital::beds = {
    Bed("Infection A", 10, 500.0),
    Bed("Infection B", 10, 750.0),
    Bed("Infection C", 10, 1000.0)
};

int Bed::totalBeds = 10;

vector<pair<int, string>> Hospital::bookedAppointments;

int main() {
    Hospital hospital;

    int choice;
    do {
        hospital.displayMenu();
        cout << "Enter your choice (1-7): ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int time;
                cout << "Enter the time (1-10 pm): ";
                cin >> time;
                hospital.displayAvailableDoctors(time);
                break;
            }
            case 2: {
                int time;
                string doctorName;
                cout << "Enter the time (1-10 pm): ";
                cin >> time;
                cout << "Enter the doctor's name: ";
                cin.ignore();
                getline(cin, doctorName);
                hospital.bookDoctorAppointment(time, doctorName);
                break;
            }
            case 3: {
                string infection;
                cout << "Enter the infection: ";
                cin.ignore();
                getline(cin, infection);
                hospital.displayAvailableBeds(infection);
                break;
            }
            case 4: {
                string infection;
                cout << "Enter the infection: ";
                cin.ignore();
                getline(cin, infection);
                hospital.bookBed(infection);
                break;
            }
            case 5: {
                int days;
                cout << "Enter the number of days: ";
                cin >> days;

                double appointmentBill = Hospital::calculateAppointmentBill(days);
                double bedBill = Hospital::calculateBedBill(days);

                cout << "Appointment bill: Rs." << appointmentBill << endl;
                cout << "Bed bill: Rs." << bedBill << endl;
                cout << "Your total bill would be Rs." << appointmentBill+bedBill << endl;
                break;
            }
            case 6: {
                hospital.displayAllDetails();
                break;
            }
            case 7:
                cout << "Exiting... Thank you!" << endl;
                break;
            default:
                cout << "Invalid choice! Please enter a number between 1 and 7." << endl;
        }
    } while (choice != 7);

    return 0;
}