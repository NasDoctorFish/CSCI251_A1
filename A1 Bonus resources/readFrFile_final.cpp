
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// g++ -std=c++11 readFrFile1.cpp -o readFrFile1.app

// Need to delete[] from main()
string *txtFileReader(const string &inputFilename)
{
    int count = 0;
    string *arr = new string[30];

    fstream inputFile(inputFilename.c_str(), fstream::in);

    string aLine;

    cout << endl;
    while (getline(inputFile, aLine))
    {
        size_t check_comment = aLine.find("//") == string::npos;
        size_t check_empty = !aLine.empty();

        if (check_comment && check_empty) // not comment
        {
            // arr[count] saves the line data 'aLine'
            arr[count] = aLine;
            // cout << "array[" << count << "] = " << arr[count] << endl;
            count++;
        }
    }
    // cout << endl;

    return arr;
}

int main()
{
    /*
    *******COPY THIS*******
    string* arr = txtFileReader(inputFilename);
    delete[] arr;
    */
    cout << endl;

    string inputFilename = "config_two.txt";
    cout << "filename : " << inputFilename << endl;
    int size = 5;
    string* arr = txtFileReader(inputFilename);
    
    for (int i = 0; i < size; ++i)
    {
        cout << "arr[" << i << "] = " << arr[i] << endl;
    }
    // cout << endl;

    delete[] arr;

    return 0;
}

