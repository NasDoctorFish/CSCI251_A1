#include <iostream>
#include <fstream>
#include <string>
#include "my_func.h"

using namespace std; // fstream defined in std, need to mention std

int main()
{
    // get user input
    string file;
    cout << "Please enter config file :";
    cin >> file;
    cin.ignore();

    //string *txtFileReader(const string &inputFilename)
    // txt file reader
    string* fileData = ConfigFileReader(file);

    // 2. Access data from config.txt
    string temp;
    string fileName, x_idxRange, y_idxRange;
    x_idxRange = fileData[0];  
    y_idxRange = fileData[1];
    fileName = fileData[2];    //cityLocation
    cout << temp << "x_Range line: " << x_idxRange << endl;
    cout << temp << "y_Range line: " << y_idxRange << endl;
    // 2-1. process all the data
    
    std::array<int, 2> x_idx_result = process_X_Y_str(x_idxRange); // extract the end index number of x
    std::array<int, 2> y_idx_result = process_X_Y_str(y_idxRange); // extract the end index number of y
    // (temp) 2-2. test the x and y index num
    cout << "x_idxRange: " << x_idx_result[0] << " ~ " << x_idx_result[1]<< endl;
    cout << "y_idxRange: " << y_idx_result[0] << " ~ " << y_idx_result[1] << endl;

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
    displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], infos);

    // final. delete fileData []
    delete[] fileData;

    return 0;
}