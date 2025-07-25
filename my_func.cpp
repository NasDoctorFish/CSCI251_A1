#include "my_func.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

using namespace std;

void displayCoordinate(const int& max_x, const int& max_y, const vector<Info>& infos)
{
    // int max_x = 12;
    // int max_y = 12;
    vector<vector<char>> grid(max_y + 1, vector<char>(max_x + 1, ' '));

    // Put the number character into grid
    for (const Info& info : infos) {
        grid[info.coord_y_int][info.coord_x_int] = info.number[0];
    }

    int chars_in_block = 5;  // space width per block
    const int offset = 2;    // extra offset for border
    int border_left = offset - 1;
    int border_right = max_x + offset + 1;
    int border_bottom = offset - 1;
    int border_top = max_y + offset + 1;

    // Loop from top (max_y + offset + 1) to bottom (0)
    for (int y = max_y + offset + 1; y >= 0; --y) {
        // Loop from left (0) to right (max_x + offset + 1)
        for (int x = 0; x <= max_x + offset + 1; ++x) {

            // Bottom-left corner cell (empty)
            if (x == 0 && y == 0) {
                cout << setw(chars_in_block) << " ";
            }
            // Y-axis index labels on the left
            else if (x == 0 && y >= offset && y - offset <= max_y) {
                cout << setw(chars_in_block) << (y - offset);
            }
            // X-axis index labels at the bottom
            else if (y == 0 && x >= offset && x - offset <= max_x) {
                cout << setw(chars_in_block) << (x - offset);
            }
            // Border lines
            else if (x == border_left || x == border_right || y == border_bottom || y == border_top) {
                cout << setw(chars_in_block) << "#";
            }
            // Inner grid cells with data
            else if (x >= offset && y >= offset && x - offset <= max_x && y - offset <= max_y) {
                cout << setw(chars_in_block) << grid[y - offset][x - offset];
            }
            // Other blank spaces
            else {
                cout << setw(chars_in_block) << " ";
            }
        }
        cout << endl;
    }
}


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
vector<Info> readCityLocation(ifstream& inFile)
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

        tuple<int, int> coord_temp= coord_to_int(temp.coord); //convert str to two int variable
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



std::array<int,2> process_X_Y_str(const string& line){
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