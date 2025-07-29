#ifndef MY_FUNC_H
#define MY_FUNC_H

#include <tuple>
#include <string>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <array>
#include <vector>
#include <numeric> // for std::accumulate
#include <iomanip> // for setprecision()

using namespace std;


// For displayCoordinate()
struct Info {
    std::string coord;
    int coord_x_int;
    int coord_y_int;
    std::string number;
    std::string name;
};


// represent coordinate dataset
struct Coord
{
    int x;
    int y;

    // set/map arrange condition
    bool operator<(const Coord &other) const
    {
        return tie(x, y) < tie(other.x, other.y);
    }
};


// represent city dataset including locations and surrounding locations
struct City
{
    string name;
    string id;
    vector<Coord> locations; // city area of this city
    vector<Coord> surroundings;
    float ACC; // Average Cloud Cover
    float AP;  // Average Pressure
};

// Function definitions
tuple<int, int> coord_to_int(const string &coord_input);
vector<Info> readCityLocation(ifstream &inFile);
vector<Info> readElse(ifstream &inFile, const string &content_type);
array<int, 2> process_X_Y_str(const string &line);
string *ConfigFileReader(const string &inputFilename);
void displayCoordinate(const int &start_x, const int &end_x, const int &start_y, const int &end_y, const vector<Info> &infos, const bool &isLMH = false);
void displayMainMenu();

vector<Coord> generateSurroundingCoord(City &city, int &min_x, int &max_x, int &min_y, int &max_y);
vector<City> readCities(vector<Info> &cityLocInfos, int &min_x, int &max_x, int &min_y, int &max_y);
vector<City> processSurroundings(vector<City> &cities, int &min_x, int &max_x, int &min_y, int &max_y);

float getAverage(const City &city, const vector<Info> &infos, const string &type);
vector<City> processACCAP(vector<City> &cities, vector<Info> cloudCoverInfos, vector<Info> pressureInfos);

void printCities(vector<City> cities);
int getRainProbability(string acc, string ap);
void printSummaryReport(const vector<City> &cities);


#endif
