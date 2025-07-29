#ifndef MY_FUNC_COPY_H
#define MY_FUNC_COPY_H

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
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};


// represent city dataset including locations and surrounding locations
struct City
{
    string name;
    string id;
    
    Coord locations[100]; // city area of this city
    int locationCount;
    Coord surroundings[100];
    int surroundingCount;

    float ACC; // Average Cloud Cover
    float AP;  // Average Pressure
};

// Function definitions
std::tuple<int, int> coord_to_int(const std::string &coord_input);
void displayCoordinate(int &start_x, int &end_x, int &start_y, int &end_y, Info *&infos, int &count, bool isLMH = false);
Info *readCityLocation(ifstream &inFile, int &count);
Info *readElse(ifstream &inFile, const string &content_type, int &count);
std::array<int, 2> process_X_Y_str(const string &line);
string *ConfigFileReader(const string &inputFilename);
void displayMainMenu();
Coord *generateSurroundingCoord(City &city, int &min_x, int &max_x, int &min_y, int &max_y, int &count);
City *readCities(Info *&cityLocInfos, int &min_x, int &max_x, int &min_y, int &max_y, int &cityLocInfosCount, int &cityCount);

float getAverage(const City &city, const Info *infos, int infoCount, const string &type = "cloudcover");
City *processACCAP(City *&cities, int &cityCount, Info *cloudCoverInfos, int &cloudInfosCount, Info *&pressureInfos, int &pressureInfosCount);
void printCities(City *cities, int &cityCount);
int getRainProbability(string acc, string ap);
City *processSurroundings(City* &cities, int &cityCount, int &min_x, int &max_x, int &min_y, int &max_y, int &surroundingCount);
void printSummaryReport(City* &cities, int &cityCount);


#endif
