#include "my_func_copy.h"
#include <iostream>
#include <numeric> // for std::accumulate
#include <sstream> // stringstream

using namespace std;

std::tuple<int, int> coord_to_int(const std::string &coord_input)
{
    std::string temp = coord_input;
    temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());

    int x, y;
    sscanf(temp.c_str(), "[%d,%d]", &x, &y);
    return {x, y};
}

// can use for citylocation, cloudcover, "cloudcover_LMH", "pressure_LMH"
// display locations
void displayCoordinate(int &start_x, int &end_x, int &start_y, int &end_y, Info *&infos, int &count, bool isLMH)
{
    int width = end_x + 1;
    int height = end_y + 1;
    string **grid = new string *[height];
    for (int i = 0; i < height; ++i)
    {
        grid[i] = new string[width];
        for (int j = 0; j < width; ++j)
        {
            grid[i][j] = " ";
        }
    }

    for (int i = 0; i < count; ++i)
    {
        int y = infos[i].coord_y_int;
        int x = infos[i].coord_x_int;
        if (y < height && x < width)
        {
            grid[y][x] = infos[i].number;
        }
    }

    int chars_in_block = 5;
    const int offset = 2;
    int x_gap = end_x - start_x;
    int y_gap = end_y - start_y;
    int border_left = offset - 1;
    int border_right = x_gap + offset + 1;
    int border_bottom = offset - 1;
    int border_top = y_gap + offset + 1;

    for (int y = y_gap + offset + 1; y >= 0; --y)
    {
        for (int x = 0; x <= x_gap + offset + 1; ++x)
        {
            if (x == 0 && y == 0)
                cout << setw(chars_in_block) << " ";
            else if (x == 0 && y >= offset && y - offset <= y_gap)
                cout << setw(chars_in_block) << (start_y + y - offset);
            else if (y == 0 && x >= offset && x - offset <= x_gap)
                cout << setw(chars_in_block) << (start_x + x - offset);
            else if (x == border_left || x == border_right || y == border_bottom || y == border_top)
                cout << setw(chars_in_block) << "#";
            else if (x >= offset && y >= offset && x - offset <= x_gap && y - offset <= y_gap)
            {
                string num_str = grid[start_y + y - offset][start_x + x - offset];
                int temp_num = 0;
                try
                {
                    if (!num_str.empty())
                        temp_num = stoi(num_str);
                }
                catch (...)
                {
                    temp_num = 0;
                }

                string name = infos[0].name;
                if (name == "cloudcover" || name == "pressure")
                {
                    if (isLMH)
                    {
                        if (temp_num < 35)
                            cout << setw(chars_in_block) << "L";
                        else if (temp_num < 65)
                            cout << setw(chars_in_block) << "M";
                        else
                            cout << setw(chars_in_block) << "H";
                    }
                    else
                    {
                        cout << setw(chars_in_block) << temp_num / 10;
                    }
                }
                else
                {
                    cout << setw(chars_in_block) << temp_num;
                }
            }
            else
            {
                cout << setw(chars_in_block) << " ";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < height; ++i)
        delete[] grid[i];
    delete[] grid;
}

// }
/*
    readCityLocation(ifstream& inFile)
    : Used when you get citylocation.txt input as ifstream object
    and read the coordinates saved inside.
*/
Info *readCityLocation(ifstream &inFile, int &count)
{
    string line, coordinate, city, cityId;
    int capacity = 100;
    Info *infos = new Info[capacity];

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

        if (count < capacity)
            infos[count++] = temp;
    }

    return infos;
}

// READ CLOUDCOVER.txt format
Info *readElse(ifstream &inFile, const string &content_type, int &count) // content_type = "cloudcover" or "pressure"
{
    string line, coordinate, number;
    int capacity = 100;
    Info *infos = new Info[capacity];

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

        if (count < capacity)
            infos[count++] = temp;
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

// vector<Coord> findSurroundings(City &city)
// {
//     vector<Coord> temp_coords;
// };

// generate correct surrounding area data with no overlap and return as array
Coord *generateSurroundingCoord(City &city, int &min_x, int &max_x, int &min_y, int &max_y, int &surroundingCount)
{
    int capacity = 100;
    Coord *coords = new Coord[capacity];

    for (int j = 0; j < city.locationCount; ++j)
    {
        for (int i = -1; i <= 1; ++i)
        {
            for (int k = -1; k <= 1; ++k)
            {
                int x = city.locations[j].x + i;
                int y = city.locations[j].y + k;
                if ((i == 0 && k == 0) || x < min_x || x > max_x || y < min_y || y > max_y)
                    continue;

                bool isOverlapped = false;

                // traverse through city.surroundings to see if there's pre-existing data
                for (int m = 0; m < surroundingCount; ++m)
                {
                    if (coords[m].x == x && coords[m].y == y)
                    {
                        isOverlapped = true;
                        break;
                    }
                }

                bool isCityLocation = false;
                for (int c = 0; c < city.locationCount; ++c)
                {
                    if (city.locations[c].x == x && city.locations[c].y == y)
                    {
                        isCityLocation = true;
                        break;
                    }
                }

                if (!isOverlapped && !isCityLocation)
                {
                    coords[surroundingCount++] = {x, y};
                }
            }
        }
    }

    return coords;
}

City *readCities(Info *&cityLocInfos, int &min_x, int &max_x, int &min_y, int &max_y, int &cityLocInfosCount, int &cityCount)
{
    int capacity = 100;
    City *cities = new City[capacity];

    // location traverse
    for (int i = 0; i < cityLocInfosCount; ++i)
    {
        Info info = cityLocInfos[i];
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
            for (int j = 0; j < cityCount; ++j)
            {
                City &city = cities[j];
                if (city.name == name) // if city name exists print the coordinate
                {
                    // Found, add the city data to city
                    tempCoord.x = coord_x_int;
                    tempCoord.y = coord_y_int;
                    city.locations[city.locationCount++] = tempCoord;
                    found = true;
                    // for testing
                    // cout << "Found: "
                    //      << city.locations[0].x << ", "
                    //      << city.locations[0].y << endl;
                    break;
                }
            }
            if (!found && cityCount < capacity) // create new city struct and pushback to cities
            {
                City tempCity;
                tempCity.name = name;
                tempCity.id = number;
                tempCoord.x = coord_x_int;
                tempCoord.y = coord_y_int;
                tempCity.locations[tempCity.locationCount++] = tempCoord; // add tempCoord to tempCity.locations arrray
                // And ++ tempCity.locationCount
                // error prevention if the data saved more than required
                cities[cityCount] = tempCity;
                cityCount++;
                // for testing
                int lastIndex = tempCity.locationCount - 1;
                cout << "Not Found: "
                     << tempCity.locations[lastIndex].x << ", "
                     << tempCity.locations[lastIndex].y << endl;
            }
            found = false;
        }
    }

    return cities;
}

City *processSurroundings(City *&cities, int &cityCount, int &min_x, int &max_x, int &min_y, int &max_y, int &surroundingCount)
{
    for (int i = 0; i < cityCount; ++i)
    {
        City city = cities[i];
        // generate surrounding data with no overlap with other locations of the same city
        Coord *surrounding_data = generateSurroundingCoord(city, min_x, max_x, min_y, max_y, surroundingCount);
        // pushback to cities vector<City>
        for (int j = 0; j < surroundingCount; ++j)
        {
            city.surroundings[j] = surrounding_data[j];
        }
        // check if it overlaps with other location data of the same city
        // different city, same surrounding data is possible
        // cout << "Surrounding_data just saved to " << city.name << ".surroundings!!!" << endl; // for testing

        delete[] surrounding_data; // delete array
    }

    return cities;
}

// get the city structure and infos either pressure or cloudcover and return the Average data
// an ACC AP averaging functions when get cloudInfos, pressureInfos
// type: "cloudcover", "pressure"
// return: <float> type
float getAverage(const City &city, const Info *infos, int infoCount, const string &type)
{
    int capacity = 100;
    float *filteredData = new float[capacity];
    int filteredCount = 0;

    for (int i = 0; i < infoCount; ++i)
    {
        const Info &info = infos[i];
        if (info.name != type)
            continue;

        int info_x_val = info.coord_x_int;
        int info_y_val = info.coord_y_int;

        bool isCityLoc = false;
        for (int j = 0; j < city.locationCount; ++j)
        {
            if (city.locations[j].x == info_x_val && city.locations[j].y == info_y_val)
            {
                isCityLoc = true;
                break;
            }
        }

        bool isSurrounding = false;
        for (int j = 0; j < city.surroundingCount; ++j)
        {
            if (city.surroundings[j].x == info_x_val && city.surroundings[j].y == info_y_val)
            {
                isSurrounding = true;
                break;
            }
        }

        if (isCityLoc || isSurrounding)
        {
            float measuredData = stof(info.number);
            if (filteredCount < capacity)
            {
                filteredData[filteredCount++] = measuredData;
            }
        }
    }

    float averageData = 0.0f;
    if (filteredCount > 0)
    {
        float sum = 0.0f;
        for (int i = 0; i < filteredCount; ++i)
        {
            sum += filteredData[i];
        }
        averageData = sum / filteredCount;
    }
    else
    {
        cout << type << " data vector in getAverage is empty!" << endl;
    }

    delete[] filteredData;
    return averageData;
}

// process ACC and AP, then updata cities vector<City>
// param: cities, cloudInfos, pressureInfos
City *processACCAP(City *&cities, int &cityCount, Info *cloudCoverInfos, int &cloudInfosCount, Info *&pressureInfos, int &pressureInfosCount)
{
    for (int i = 0; i < cityCount; ++i)
    {
        // get ACC and AP
        City city = cities[i];
        float ACC = getAverage(cities[i], cloudCoverInfos, cloudInfosCount, "cloudcover");
        float AP = getAverage(cities[i], pressureInfos, pressureInfosCount, "pressure");
        city.ACC = ACC;
        city.AP = AP;
    }

    return cities;
}

// for cities dataset testing purpose
void printCities(City *cities, int &cityCount)
{
    for (int i = 0; i < cityCount; ++i)
    {
        City city = cities[i];
        cout << fixed << setprecision(2);
        cout << city.name << endl;
        cout << "Locations: ";
        for (int i = 0; i < city.locationCount; ++i)
        {
            cout << "(" << city.locations[i].x << " , " << city.locations[i].y << ")" << " ";
        }

        cout << "Surroundings: ";
        for (int j = 0; j < city.surroundingCount; ++j)
        {
            cout << "(" << city.surroundings[j].x << " , " << city.surroundings[j].y << ")" << " ";
        }

        cout << "ACC: ";
        cout << city.ACC << endl;

        cout << "AP: ";
        cout << city.AP << endl;

        cout << "Finish" << endl;
    }
}

// return rain probability value as int
int getRainProbability(string acc, string ap)
{
    if (ap == "L" && acc == "H")
        return 90;
    if (ap == "L" && acc == "M")
        return 80;
    if (ap == "L" && acc == "L")
        return 70;
    if (ap == "M" && acc == "H")
        return 60;
    if (ap == "M" && acc == "M")
        return 50;
    if (ap == "M" && acc == "L")
        return 40;
    if (ap == "H" && acc == "H")
        return 30;
    if (ap == "H" && acc == "M")
        return 20;
    if (ap == "H" && acc == "L")
        return 10;

    // Exception handling
    return -1; // Invalid input
}

// print the final summary report after finishing city reading
void printSummaryReport(City *&cities, int &cityCount)
{
    std::string asciiArray[9] = {
        "~~~~\n~~~~~\n\\\\\\\\\\", // Row 1
        "~~~~\n~~~~~\n \\\\\\\\",  // Row 2
        "~~~~\n~~~~~\n   \\\\\\",  // Row 3
        "~~~~\n~~~~~\n    \\\\",   // Row 4
        "~~~~\n~~~~~\n    \\",     // Row 5
        "~~~~\n~~~~~\n\n",         // Row 6
        "~~~\n~~~~\n\n",           // Row 7
        "~~\n~~~\n\n",             // Row 8
        "~\n~~\n\n"                // Row 9
    };
    cout << "Weather Forecast Summary Report" << endl;
    for (int i = 0; i < 50; ++i)
    {
        cout << "-";
    }
    cout << endl;
    // traverse through infos and cities and print all the summary
    for (int i = 0; i < cityCount; ++i)
    {
        // L,M,H 조건식만들고 probability도 조건식과 그에 맞는 ASCII 코드 매칭
        City city = cities[i];
        float ACC_DATA = city.ACC;
        float AP_DATA = city.AP;
        float rainProbability;
        string ACC_LMH;
        string AP_LMH;
        int asciiIndex;

        // set Average Cloud Cover L, M, H notation
        if (ACC_DATA >= 0 && ACC_DATA < 35)
        {
            ACC_LMH = "L";
        }
        else if (ACC_DATA >= 35 && ACC_DATA < 65)
        {
            ACC_LMH = "M";
        }

        else if (ACC_DATA >= 65 && ACC_DATA < 100)
        {
            ACC_LMH = "H";
        }

        // set Average Pressure L, M, H notation
        if (AP_DATA >= 0 && AP_DATA < 35)
        {
            AP_LMH = "L";
        }
        else if (AP_DATA >= 35 && AP_DATA < 65)
        {
            AP_LMH = "M";
        }

        else if (AP_DATA >= 65 && AP_DATA < 100)
        {
            AP_LMH = "H";
        }

        // set if/else statement for Probability of Rain
        rainProbability = getRainProbability(ACC_LMH, AP_LMH);

        asciiIndex = (int)(90 - rainProbability) / 10;
        // cout << "asciiIndex: " << asciiIndex << endl; // for testing

        // 바로 asciiArray에서 조회할 수 있게 index에 맞게 맵핑하기
        cout << endl;
        cout << fixed << setprecision(2);
        cout << setw(10) << "City Name" << " : " << city.name << endl;
        cout << setw(10) << "City ID" << " : " << city.id << endl;
        cout << setw(40) << "Ave. Cloud Cover (ACC)" << " : "
             << setw(7) << city.ACC << " (" << ACC_LMH << ")" << endl;
        cout << setw(40) << "Ave. Pressure (AP)" << " : "
             << setw(7) << city.AP << " (" << AP_LMH << ")" << endl;
        cout << setw(40) << "Probability of Rain (%)" << " : "
             << setw(7) << rainProbability << endl;
        cout << asciiArray[asciiIndex] << endl;
        // printAsciiVisual(rainProbability);
    }
}