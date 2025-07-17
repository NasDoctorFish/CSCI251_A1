
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// g++ -std=c++11 readFrFile1.cpp -o readFrFile1.app

void readAFile(string filename);

int main()
{
    cout << endl;
    cout << "Please enter input filename : ";

    string inputFilename;
    int count = 0;
    string *arr = new string[30];

    // when prompted, pls type in 'input.txt'
    cin >> inputFilename;

    fstream inputFile(inputFilename.c_str(), fstream::in);

    string aLine;

    cout << endl;
    while (getline(inputFile, aLine))
    {
        size_t check_comment = aLine.find("//") == string::npos;
        size_t check_empty = !aLine.empty();

        if (check_comment && check_empty) // not comment
        {
            // readAFile (aLine);
            arr[count] = aLine;
            cout << "array[" << count << "] = " << arr[count] << endl;
            count++;
        }
    }
    cout << endl;

    delete[] arr;

    return 0;
}

string removeSpaceAndComment(string input, string delimiter = "//")
{
    size_t pos = 0;
    string token;

    while (((pos = input.find(delimiter)) != string::npos)) // npos: no position founded
    {
        // cut from current position line_start to previous newline
        size_t line_start = input.rfind('\n', pos);
        line_start = (line_start == string::npos) ? 0 : line_start + 1; // 없으면 0부터

        size_t line_end = input.find('\n', pos);
        input.erase(line_start, line_end - line_start);
    }

    return input;
}

void readAFile(string filename)
{
    fstream inputFile(filename.c_str(), fstream::in);

    cout << endl;
    cout << "Reading contents of file : " << filename << endl;
    cout << endl;

    string aLine;

    while (getline(inputFile, aLine))
    {
        cout << aLine << endl;
    }
    cout << endl;
}

// int main() {
//     int size;
//     cout << "Enter array size: ";
//     cin >> size;

//     int* arr = new int[size];  // 동적 할당

//     for (int i = 0; i < size; ++i)
//         arr[i] = i * 10;

//     for (int i = 0; i < size; ++i)
//         cout << arr[i] << " ";

//     delete[] arr;  // 메모리 해제
//     return 0;
// }
