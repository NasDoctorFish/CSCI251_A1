#include <iostream>
#include <fstream>
#include <string>
#include "my_func.h"

using namespace std; // fstream defined in std, need to mention std

int main()
{
    // get user input
    string file;
    string line;
    cout << "Please enter config file :";
    cin >> file;
    cin.ignore();

    // read user inputed file
    ifstream inData;
    ofstream outData;
    inData.open(file);

    // //(temp) read all content and print
    // cout << "file: " << file;
    // cout << "Content: " << endl;
    // while (getline(inData, line))
    // {
    //     cout << line << endl;
    // }

    // 1. Read each line, clean, and save in array

    inData.clear();            // initialize cursor
    inData.seekg(0, ios::beg); // move cursor to file first line
    vector<string> lines;
    while (getline(inData, line))
    {
        lines.push_back(line);
    }
    for (const string &line : lines)
    {
        cout << line << endl;
    }

    inData.close();

    // 2. Access third data from config.txt
    string temp;
    cout << temp << "Third line: " << lines[2] << endl;
    cout << temp << "Tenth line: " << lines[9] << endl;
    // 2-1. process all the data
    string fileName, x_idxRange, y_idxRange;
    x_idxRange = lines[2];                          // third line
    int end_x_idx = process_X_Y_str(x_idxRange)[1]; // extract the end index number of x
    y_idxRange = lines[6];                          // seventh line
    int end_y_idx = process_X_Y_str(y_idxRange)[1]; // extract the end index number of y
    fileName = lines[9];                            // Tenth line

    // (temp) 2-2. test the x and y index num
    cout << "x_idxRange: " << end_x_idx << endl;
    cout << "y_idxRange: " << end_y_idx << endl;

    // 3. Open the file (Create inFile object)
    ifstream inFile(fileName);
    if (!inFile)
    {
        cerr << "Cannot open the file!" << endl;
        return 1;
    }
    // 4. Process readCityLocation()
    vector<Info> infos = readCityLocation(inFile);

    // 5. displayCoordinate()
    displayCoordinate(end_x_idx, end_y_idx, infos);

    return 0;
}