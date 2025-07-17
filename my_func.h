#ifndef MY_FUNC_H
#define MY_FUNC_H

#include <tuple>
#include <string>
#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>

using namespace std;


// For displayCoordinate()
struct Info {
    std::string coord;
    int coord_x_int;
    int coord_y_int;
    std::string number;
    std::string name;
};

inline std::tuple<int, int> coord_to_int(const std::string& coord_input)
{
    std::string temp = coord_input;
    temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());

    int x, y;
    sscanf(temp.c_str(), "[%d,%d]", &x, &y);
    return {x, y};
}



void displayCoordinate(const int& max_x, const int& max_y, const vector<Info>& infos);



vector<Info> readCityLocation(ifstream& inFile);



std::array<int,2> process_X_Y_str(const string& line);



#endif
