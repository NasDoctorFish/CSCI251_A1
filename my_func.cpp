#include "my_func.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

using namespace std;

std::tuple<int, int> coord_to_int(const std::string &coord_input)
{
    std::string temp = coord_input;
    temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());

    int x, y;
    sscanf(temp.c_str(), "[%d,%d]", &x, &y);
    return {x, y};
}

// 다 하고 다시 체크하기!!!
// void displayMainMenu()
// {
//     // APPendix F Main menu Dev
//     const string studentId = "8761838";
//     const string studentName = "Juwon Lee";

//     cout << left << setw(20) << "Student ID" << " : " << setw(30) << studentId << endl;
//     cout << left << setw(20) << "Student Name" << " : " << setw(30) << studentName << endl;
//     cout << endl;
//     for (int i = 0; i < 50; i++)
//     {
//         cout << "-";
//     }
//     cout << endl;
//     cout << endl;

//     cout << "Welcome to Wether Information Processing System!" << endl;
//     cout << endl;
//     cout << left << setw(6) << "1)" << setw(40) << "Read in and process a configuration file" << endl;
//     cout << left << setw(6) << "2)" << setw(40) << "Display city map" << endl;
//     cout << left << setw(6) << "3)" << setw(40) << "Display cloud coverage map (cloudiness index)" << endl;
//     cout << left << setw(6) << "4)" << setw(40) << "Display cloud coverage map (LMH symbols)" << endl;
//     cout << left << setw(6) << "5)" << setw(40) << "Display atmospheric pressure map (pressure index)" << endl;
//     cout << left << setw(6) << "6)" << setw(40) << "Display atmospheric pressure map (LMH symbols)" << endl;
//     cout << left << setw(6) << "7)" << setw(40) << "Show weather forecast summary report" << endl;
//     cout << left << setw(6) << "8)" << setw(40) << "Quit" << endl;
//     cout << endl;
//     cout << left << setw(30) << "Please enter your choice : ";
//     int selectedNum;
//     cin >> selectedNum;
// }

// can use for citylocation, cloudcover, "cloudcover_LMH", "pressure_LMH"
void displayCoordinate(const int &start_x, const int &end_x, const int &start_y, const int &end_y, const vector<Info> &infos, const bool &isLMH)
{
    // int end_x = 12;
    // int end_y = 12;
    vector<vector<string>> grid(end_y + 1, vector<string>(end_x + 1, " "));

    // Put the number character into grid
    for (const Info &info : infos)
    {
        grid[info.coord_y_int][info.coord_x_int] = info.number;
    }

    int chars_in_block = 5; // space width per block
    const int offset = 2;   // extra offset for border
    int x_gap = end_x - start_x;
    int y_gap = end_y - start_y;
    int border_left = offset - 1;
    int border_right = x_gap + offset + 1;
    int border_bottom = offset - 1;
    int border_top = y_gap + offset + 1;

    // Loop from top (y_gap + offset + 1) to bottom (0)
    for (int y = y_gap + offset + 1; y >= 0; --y)
    {
        // Loop from left (0) to right (x_gap + offset + 1)
        for (int x = 0; x <= x_gap + offset + 1; ++x)
        {

            // Bottom-left corner cell (empty)
            if (x == 0 && y == 0)
            {
                cout << setw(chars_in_block) << " ";
            }
            // Y-axis index labels on the left
            else if (x == 0 && y >= offset && y - offset <= y_gap)
            {
                cout << setw(chars_in_block) << (start_y + y - offset);
            }
            // X-axis index labels at the bottom
            else if (y == 0 && x >= offset && x - offset <= x_gap)
            {
                cout << setw(chars_in_block) << (start_x + x - offset);
            }
            // Border lines
            else if (x == border_left || x == border_right || y == border_bottom || y == border_top)
            {
                cout << setw(chars_in_block) << "#";
            }
            // Inner grid cells with data
            else if (x >= offset && y >= offset && x - offset <= x_gap && y - offset <= y_gap)
            {
                string num_str = grid[start_y + y - offset][start_x + x - offset];
                int temp_num = 0;
                try
                {
                    if (!num_str.empty()) // if num_str is not empty, convert is into integer
                        temp_num = stoi(num_str);
                }
                catch (const invalid_argument &e)
                {
                    // 잘못된 값은 그냥 0으로 둠
                    temp_num = 0;
                }

                // when infos are CloudCover data
                if (infos[0].name == "cloudcover")
                // the value of name in every CloudCover data is "CloudCover"
                {
                    if (isLMH) {
                        if (temp_num >= 0 && temp_num < 35) {
                            cout << setw(chars_in_block) << "L";
                        } //현재 traverse 된 temp_num의 값을 확인 if/else 문 통과
                        else if (temp_num >= 35 && temp_num < 65) {
                            cout << setw(chars_in_block) << "M";
                        } //현재 traverse 된 infos의 값을 확인 if/else 문 통과
                        else if (temp_num >= 65 && temp_num < 100) {
                            cout << setw(chars_in_block) << "H";
                        } //현재 traverse 된 infos의 값을 확인 if/else 문 통과
                    }
                    else {
                        cout << setw(chars_in_block) << temp_num / 10; // cloud cover index e.g. 41 => 4
                    }
                }
                else if (infos[0].name == "pressure") // when the content_type is pressure
                {
                    if (isLMH) {
                        if (temp_num >= 0 && temp_num < 35) {
                            cout << setw(chars_in_block) << "L";
                        } //현재 traverse 된 temp_num의 값을 확인 if/else 문 통과
                        else if (temp_num >= 35 && temp_num < 65) {
                            cout << setw(chars_in_block) << "M";
                        } //현재 traverse 된 infos의 값을 확인 if/else 문 통과
                        else if (temp_num >= 65 && temp_num < 100) {
                            cout << setw(chars_in_block) << "H";
                        } //현재 traverse 된 infos의 값을 확인 if/else 문 통과
                    }
                    else {
                        cout << setw(chars_in_block) << temp_num / 10; // cloud cover index e.g. 41 => 4
                    }
                }
                else
                { // when infos are citylocation data
                    cout << setw(chars_in_block) << temp_num;
                }
            }
            // Other blank spaces
            else
            {
                cout << setw(chars_in_block) << " ";
            }
        }
        cout << endl;
    }
}

// quotient = dividend / divisor

// //How to use displayCoordinate
// int main(){
//     vector<Info> infos = {
//         {"[1, 1]", 1, 1, "3", "Big_City"},
//         {"[1, 2]", 1, 2, "3", "Big_City"},
//         {"[1, 3]", 1, 3, "3", "Big_City"},
//         {"[2, 1]", 2, 1, "3", "Big_City"},
//         {"[2, 2]", 2, 2, "3", "Big_City"},
//         {"[2, 3]", 2, 3, "3", "Big_City"},
//         {"[2, 7]", 2, 7, "2", "Mid_City"},
//         {"[2, 8]", 2, 8, "2", "Mid_City"},
//         {"[3, 1]", 3, 1, "3", "Big_City"},
//         {"[3, 2]", 3, 2, "3", "Big_City"},
//         {"[3, 3]", 3, 3, "3", "Big_City"},
//         {"[3, 7]", 3, 7, "2", "Mid_City"},
//         {"[3, 8]", 3, 8, "2", "Mid_City"},
//         {"[7, 7]", 7, 7, "1", "Small_City"}
//     };

//     displayCoordinate(8, 8, infos);

//     return 0;
// }

/*
    readCityLocation(ifstream& inFile)
    : Used when you get citylocation.txt input as ifstream object
    and read the coordinates saved inside.
*/
vector<Info> readCityLocation(ifstream &inFile)
{
    string line, coordinate, city, cityId;
    vector<Info> infos;

    while (getline(inFile, line))
    {
        stringstream ss(line);

        Info temp;
        getline(ss, temp.coord, '-');
        getline(ss, temp.number, '-');
        getline(ss, temp.name);

        if (temp.coord.empty())
            continue; // wrong line skip

        tuple<int, int> coord_temp = coord_to_int(temp.coord); // convert str to two int variable
        temp.coord_x_int = get<0>(coord_temp);
        temp.coord_y_int = get<1>(coord_temp);

        infos.push_back(temp);
    }

    return infos;
}

// How to use readCityLocation()
// ifstream inFile("citylocation.txt"); // Actual file name
//     if (!inFile) {
//         cerr << "Cannot open the file!!" << endl;
//         return 1;
//     }
//     vector<Info> infos = readCityLocation(inFile);
//     // output result
//     for (const auto& info : infos) {
//         std::cout << "coord: " << info.coord
//                   << ", number: " << info.number
//                   << ", name: " << info.name
//                   << ", coord_x: " << info.coord_x_int << "(" << typeid(info.coord_x_int).name() << ")"
//                   << ", coord_y: " << info.coord_y_int << "(" << typeid(info.coord_y_int).name() << ")"
//                   << std::endl;
//     }

// READ CLOUDCOVER.txt format
vector<Info> readElse(ifstream &inFile, const string &content_type) // content_type = "cloudcover" or "pressure"
{
    string line, coordinate, number;
    vector<Info> infos;

    while (getline(inFile, line))
    {
        stringstream ss(line);

        Info temp;
        getline(ss, temp.coord, '-');
        getline(ss, temp.number);

        if (temp.coord.empty())
            continue; // wrong line skip

        tuple<int, int> coord_temp = coord_to_int(temp.coord); // convert str to two int variable
        temp.coord_x_int = get<0>(coord_temp);
        temp.coord_y_int = get<1>(coord_temp);
        temp.name = content_type;

        infos.push_back(temp);
    }

    return infos;
}

std::array<int, 2> process_X_Y_str(const string &line)
{
    // extract '='
    size_t eqPos = line.find('=');
    string rangePart = line.substr(eqPos + 1); // "0-8"
    // seperate based on '-'
    size_t dashPos = rangePart.find('-');
    string startStr = rangePart.substr(0, dashPos); // "0"
    string endStr = rangePart.substr(dashPos + 1);  // "8"
    // convert into number if necessary (stoi)
    int start = stoi(startStr);
    int end = stoi(endStr);
    array<int, 2> arr = {start, end};
    // return start and end
    return arr;
}

string *ConfigFileReader(const string &inputFilename)
{
    int count = 0;
    string *arr = new string[50];

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

void displayMainMenu()
{ 
    // Appendix F Main menu
    const string studentId = "8761838";
    const string studentName = "Juwon Lee";

    cout << left << setw(20) << "Student ID" << " : " << setw(30) << studentId << endl;
    cout << left << setw(20) << "Student Name" << " : " << setw(30) << studentName << endl;
    cout << endl;
    for (int i = 0; i < 50; i++)
    {
        cout << "-";
    }
    cout << endl;
    cout << endl;

    cout << "Welcome to Weather Information Processing System!" << endl;
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
}