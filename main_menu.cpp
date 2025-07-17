#include <iostream>
#include <string>
#include <iomanip> //setw()

using namespace std;

int main()
{
    // APPendix F Main menu Dev
    const string studentId = "8761838";
    const string studentName = "Juwon Lee";

    cout << left << setw(20) << "Student ID" << " : " << setw(30) << studentId << endl;
    cout << left << setw(20) << "Student Name" << " : " << setw(30) << studentName << endl;
    cout << endl;
    for (int i = 0; i < 50; i++) {
        cout << "-";
    }
    cout << endl;
    cout << endl;

    cout << "Welcome to Wether Information Processing System!" << endl;
    cout << endl;
    cout << left << setw(6) << "1)" << setw(40) << "Read in and process a configuration file" << endl;
    cout << left << setw(6) << "2)" << setw(40) << "Display city map" << endl;
    cout << left << setw(6) << "3)" << setw(40) << "Display cloud coverage map (cloudiness index)" << endl;
    cout << left << setw(6) << "4)" << setw(40) << "Display cloud coverage map (LMH symbols)" << endl;
    cout << left << setw(6) << "5)" << setw(40) << "Display atmospheric pressure map (pressure index)" << endl;
    cout << left << setw(6) << "6)" << setw(40) << "Display atmospheric pressure map (LMH symbols)" << endl;
    cout << left << setw(6) << "7)" << setw(40) << "Show weather forecast summary report" << endl;
    cout << left << setw(6) << "8)" << setw(40) << "Quit" << endl;
    cout << endl;
    cout << left << setw(30) << "Please enter your choice : ";
    int selectedNum;
    cin >> selectedNum;

    return 0;

    // APPendix F Main menu Dev
}