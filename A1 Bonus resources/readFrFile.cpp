
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// g++ -std=c++11 readFrFile.cpp -o readFrFile.app

int main ()
{ 
	cout << endl;
    cout << "Please enter input filename : ";

    string inputFilename;
    cin >> inputFilename;

    fstream inputFile (inputFilename.c_str(), fstream::in); //old days fstream cannot take string itself. instead they used const char* type, so .c_str() converts into const char*.
  
    string aLine;

    cout << endl;
    while (getline (inputFile, aLine))
    {
		cout << aLine << endl;

	}
	cout << endl;

	return (0);
}



