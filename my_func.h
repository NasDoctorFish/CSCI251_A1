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

std::tuple<int, int> coord_to_int(const std::string& coord_input);



void displayCoordinate(const int &start_x, const int &end_x, const int &start_y, const int &end_y, const vector<Info> &infos, const bool &isLMH = false);



vector<Info> readCityLocation(ifstream& inFile);

vector<Info> readElse(ifstream &inFile, const string& content_type);


std::array<int,2> process_X_Y_str(const string& line);


string *ConfigFileReader(const string &inputFilename);


void displayMainMenu();


#endif
