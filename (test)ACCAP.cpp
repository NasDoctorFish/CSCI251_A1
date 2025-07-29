#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <numeric> // for std::accumulate

using namespace std;

// For displayCoordinate()
struct Info
{
    std::string coord;
    int coord_x_int; // use this
    int coord_y_int; // use this
    std::string number;
    std::string name;
};

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

struct City
{
    string name;
    vector<Coord> locations; // city area of this city
    vector<Coord> surroundings;
    float ACC; // Average Cloud Cover
    float AP; // Average Pressure
};

// vector<Coord> findSurroundings(City &city)
// {
//     vector<Coord> temp_coords;
// };

// generate correct surrounding area data with no overlap and return as vector<Coord> type
vector<Coord> generateSurroundingCoord(City &city, int &min_x, int &max_x, int &min_y, int &max_y)
{
    int count = 0; // Delete later
    vector<Coord> coords;

    // add possible surrounding data
    for (int j = 0; j < city.locations.size(); ++j)
    {
        for (int i = -1; i < 2; ++i) // represents x-axis traversing
        {
            for (int k = -1; k < 2; ++k) // y-axis
            {
                int x_coord_temp = city.locations[j].x + i;
                int y_coord_temp = city.locations[j].y + k;
                if ((i == 0 && k == 0) ||
                    x_coord_temp < min_x || x_coord_temp > max_x || // check if generated surrounding is within the grid X, Y range
                    y_coord_temp < min_y || y_coord_temp > max_y)
                {
                    continue; // skip this loop, skip the orignial and out-of-grid coordinates
                }
                Coord temp;
                temp.x = x_coord_temp;
                temp.y = y_coord_temp;

                // check overlap here using temp
                bool isOverlap = false;
                for (const Coord tempCoord : coords)
                {
                    if ((x_coord_temp == tempCoord.x) && (y_coord_temp == tempCoord.y)) // when the coordinate already added to vector<Coord>
                    {
                        // cout << "Duplicate Found" << " Continuing" << endl; // break
                        isOverlap = true;
                        break;
                    }
                }

                if (isOverlap)
                {
                    isOverlap = true;
                    continue; // skip this loop, skip the original and out-of-grid coordinates
                }

                // cout << "possible_surr. "
                //      << "x: " << temp.x << " "
                //      << "y: " << temp.y << endl;
                ++count;
                coords.push_back(temp);
            }
        }
    }
    cout << "surrounding count for " << city.name << " : " << count << endl;

    // check if citylocation itself is included
    bool isOverlapOtherCityLoc = false; // use to check if city location itself is included in coords

    // traverse through coords and city parameter to check if city location is added on coords
    coords.erase(remove_if(coords.begin(), coords.end(),
                           [&](const Coord &coord)
                           {
                               for (const Coord &cityLoc : city.locations)
                               {
                                   if (coord.x == cityLoc.x && coord.y == cityLoc.y)
                                       return true;
                               }
                               return false;
                           }),
                 coords.end());

    return coords;
};

vector<City> readCities(vector<Info> &cityLocInfos, int &min_x, int &max_x, int &min_y, int &max_y)
{
    vector<City> cities;

    // location traverse
    for (const Info &info : cityLocInfos)
    {
        string coord = info.coord;
        int coord_x_int = info.coord_x_int; // use this
        int coord_y_int = info.coord_y_int; // use this
        string number = info.number;
        string name = info.name;

        Coord tempCoord;

        // 1. Create City struct and save all extracted city location within the grid
        if (coord_x_int >= min_x && coord_x_int <= max_x && coord_y_int >= min_y && coord_y_int <= max_y)
        {
            bool found = false;
            // if name is not in cities
            for (City &city : cities)
            {
                if (city.name == name) // if city name exists print the coordinate
                {
                    // Found, add the city data to city
                    tempCoord.x = coord_x_int;
                    tempCoord.y = coord_y_int;
                    city.locations.push_back(tempCoord);
                    found = true;
                    cout << "Found: "
                         << city.locations[0].x << ", "
                         << city.locations[0].y << endl;
                    break;
                }
            }
            if (!found) // create new city struct and pushback to cities
            {
                City tempCity;
                tempCity.name = name;
                tempCoord.x = coord_x_int;
                tempCoord.y = coord_y_int;
                tempCity.locations.push_back(tempCoord);
                cities.push_back(tempCity);
                cout << "Not Found: "
                     << tempCity.locations[0].x << ", "
                     << tempCity.locations[0].y << endl;
            }
            found = false;
        }
        cout << endl;
    }

    return cities;
}

vector<City> processSurroundings(vector<City> &cities, int &min_x, int &max_x, int &min_y, int &max_y)
{
    for (City &city : cities)
    {
        // generate surrounding data with no overlap with other locations of the same city
        vector<Coord> surrounding_data = generateSurroundingCoord(city, min_x, max_x, min_y, max_y);
        // pushback to cities vector<City>
        city.surroundings = surrounding_data;
        // check if it overlaps with other location data of the same city
        // different city, same surrounding data is possible
        cout << "Surrounding_data just saved to " << city.name << ".surroundings!!!" << endl;
    }

    return cities;
}

// get the city structure and infos either pressure or cloudcover and return the Average data
// type: "cloudcover", "pressure"
// return: <float> type
float getAverage(const City &city, const vector<Info> &infos, const string &type = "cloudcover")
{

    vector<float> filteredCloudData;
    vector<Info> citylocs = city.locations;
    vector<Info> surroundinglocs = city.surroundings;
    bool isCorrect = info.name == type;
    for (const Info &info : infos)
    {
        int info_x_val = info.coord_x_int;
        int info_y_val = info.coord_y_int;
        bool isCityLoc = false;
        bool isSurrounding = false;

        // traverse through citylocation
        for (const Info &cityloc : citylocs)
        {
            // if info_x_val and info_y_val same as info coordinate in surrounding or citylocation
            if (isCorrect && info_x_val == info.coord_x_int && info_y_val == info.coord_y_int)
            {
                isCityLoc == true; // city location found
            }
        }

        // traverse through surrounding location
        for (const Info &surrounding : surroundinglocs)
        {
            // if info_x_val and info_y_val same as info coordinate in surrounding or citylocation
            if (isCorrect && info_x_val == info.coord_x_int && info_y_val == info.coord_y_int)
            {
                isSurrounding = true; // surrounding location found
            }
        }
        float measuredData = info.number; // add to filteredCloudData
        filteredCloudData.push_back(measuredData);
    }

    // calculate average of data in filteredCloudData
    float averageCloudCover;
    if (!data.empty())
    {
        float sum = std::accumulate(data.begin(), data.end(), 0.0f);
        averageCloudCover = sum / data.size();
        cout << "Average: " << averageCloudCover << endl;
    }
    else
    {
        cout << "Cloud Cover data vector in getAverageCloudCover is empty!" << endl;
    }

    return averageCloudCover;
}

vector<City> processACCAP(vector<City> &cities, vector<Info> cloudCoverInfos, vector<Info> pressureInfos)
{
    for (City &city: cities)
    {
        // get ACC and AP
        float ACC = getAverage(city, cloudCoverInfos, "cloudcover");
        float AP = getAverage(city, pressureInfos, "pressure");
        city.ACC = ACC;
        city.AP = AP;
    }

    return cities;
}

void printCities(vector<City> cities)
{
    for (const City &city : cities)
    {
        cout << city.name << endl;
        cout << "Locations: ";
        for (int i = 0; i < city.locations.size(); ++i)
        {
            cout << "(" << city.locations[i].x << " , " << city.locations[i].y << ")" << " ";
        }

        cout << "Surroundings: ";
        for (int i = 0; i < city.surroundings.size(); ++i)
        {
            cout << "(" << city.surroundings[i].x << " , " << city.surroundings[i].y << ")" << " ";
        }

        cout << "ACC: ";
        cout << city.ACC;

        cout << "AP: ";
        cout << city.AP;

        cout << "Finish" << endl;
    }
}

int main()
{

    /*
        Calculate ACC and AP by finding the surrounding area, using the given citylocation Infos data
        일단 traversing을 전체 grid 범위로 해야함.
    */

    // ------------------------------------ Read Data Necessary ------------------------------------

    // 1. get infos as input
    vector<Info> tempInfos;

    // Manual data for testing(manually insert test data)
    tempInfos.push_back({"[1, 1]", 1, 1, "3", "Big_City"});
    tempInfos.push_back({"[1, 2]", 1, 2, "3", "Big_City"});
    // tempInfos.push_back({"[1, 3]", 1, 3, "3", "Big_City"});

    tempInfos.push_back({"[2, 1]", 2, 1, "2", "Mid_City"});
    // tempInfos.push_back({"[2, 2]", 2, 2, "2", "Mid_City"});
    // tempInfos.push_back({"[2, 3]", 2, 3, "2", "Mid_City"});

    // tempInfos.push_back({"[3, 3]", 3, 3, "1", "Small_City"});
    // tempInfos.push_back({"[3, 4]", 3, 4, "1", "Small_City"});

    // tempInfos.push_back({"[0, 0]", 0, 0, "4", "Tiny_City"});
    // tempInfos.push_back({"[0, 1]", 0, 1, "4", "Tiny_City"});

    tempInfos.push_back({"[4, 4]", 4, 4, "5", "Mega_City"});
    // tempInfos.push_back({"[4, 3]", 4, 3, "5", "Mega_City"});
    // tempInfos.push_back({"[3, 4]", 3, 4, "5", "Mega_City"});

    // 2. get Grid X and Y range
    int min_x, min_y, max_x, max_y;
    min_x = 0; // min and max inclusive
    min_y = 0;
    max_x = 4;
    max_y = 3;

    // 3. Generate surrounding areas of city location and make sure it not overlaps and pushback to cities vector<City>
    vector<City> cities = readCities(tempInfos, min_x, max_x, min_y, max_y);
    vector<City> filteredCities = processSurroundings(cities, min_x, max_x, min_y, max_y);
    // 1-1. Check if cities are properly saved
    printCities(filteredCities);

    return 0;
}




// 1. calculate ACC and AP
// cityLocationInfos, cloudInfos, pressureInfos
// need a ACC AP averaging functions when get cloudInfos, pressureInfos
// after processing city reading
void printSummaryReport(const City &city, const vector<Info> &infos)
{
    std::string asciiArray[9] = {
        "~~~~\n~~~~~\n\\\\\\\\\\", // Row 1
        "~~~~\n~~~~~\n  \\\\\\\\", // Row 2
        "~~~~\n~~~~~\n    \\\\\\", // Row 3
        "~~~~\n~~~~~\n      \\\\", // Row 4
        "~~~~\n~~~~~\n        \\", // Row 5
        "~~~~\n~~~~~\n\n",         // Row 6
        "~~~\n~~~~\n\n",           // Row 7
        "~~\n~~~\n\n",             // Row 8
        "~\n~~\n\n"                // Row 9
    };
    cout << "Weather Forecast Summary Report";
    for (int i = 0; i < 50; ++i)
    {
        cout << "-";
    }
    // traverse through infos and cities and print all the summary

    // cout << endl;
    // cout << setw(10) << "City Name" << " : " << city.name << endl;
    // cout << setw(10) << "City ID" << " : " << city.id << endl;
    // cout << setw(40) << "Ave. Cloud Cover (ACC)" << " : "
    //      << setw(7) << avgcloudcover << " (" << ACC_LMH << ")" << endl;
    // cout << setw(40) << "Ave. Pressure (AP)" << " : "
    //      << setw(7) << avgpressure << " (" << ACC_LMH << ")" << endl;
    // cout << setw(40) << "Probability of Rain (%)" << " : "
    //      << setw(7) << probability << endl;
    // cout << asciiArray[0] << endl;
}
