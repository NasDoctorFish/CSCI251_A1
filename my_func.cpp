#include "my_func.h"
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
    // cout << "surrounding count for " << city.name << " : " << count << endl; // for testing

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
                    // for testing
                    // cout << "Found: "
                    //      << city.locations[0].x << ", "
                    //      << city.locations[0].y << endl;
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
                // for testing
                // cout << "Not Found: "
                //      << tempCity.locations[0].x << ", "
                //      << tempCity.locations[0].y << endl;
            }
            found = false;
        }
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
        // cout << "Surrounding_data just saved to " << city.name << ".surroundings!!!" << endl; // for testing
    }

    return cities;
}

// get the city structure and infos either pressure or cloudcover and return the Average data
// an ACC AP averaging functions when get cloudInfos, pressureInfos
// type: "cloudcover", "pressure"
// return: <float> type
float getAverage(const City &city, const vector<Info> &infos, const string &type = "cloudcover")
{

    vector<float> filteredData;
    vector<Coord> citylocs = city.locations;           // Coord type
    vector<Coord> surroundinglocs = city.surroundings; // Coord type

    // traverse through infos and find out either cloudcover or pressure info
    for (const Info &info : infos)
    {
        int info_x_val = info.coord_x_int;
        int info_y_val = info.coord_y_int;
        bool isCorrect = info.name == type;
        bool isCityLoc = false;
        bool isSurrounding = false;

        // traverse through citylocation
        for (const Coord &cityloc : citylocs)
        {
            // if info_x_val and info_y_val same as info coordinate in surrounding or citylocation
            if (isCorrect && (info_x_val == cityloc.x) && (info.coord_y_int == cityloc.y))
            {
                isCityLoc = true; // city location found
            }
        }

        // traverse through surrounding location
        for (const Coord &surrounding : surroundinglocs)
        {
            // if info_x_val and info_y_val same as info coordinate in surrounding or citylocation
            if (isCorrect && (info_x_val == surrounding.x) && (info.coord_y_int == surrounding.y))
            {
                isSurrounding = true; // surrounding location found
            }
        }
        if (isCityLoc || isSurrounding)
        {
            float measuredData = stof(info.number); // info.number string type to float
            filteredData.push_back(measuredData);   // add to filteredData
        }
    }

    // calculate average of data in filteredData
    float averageData;
    if (!filteredData.empty())
    {
        float sum = std::accumulate(filteredData.begin(), filteredData.end(), 0.0f);
        averageData = sum / filteredData.size();
        // cout << "Average: " << averageData << endl; // for testing 
    }
    else
    {
        cout << type << " data vector in getAverage is empty!" << endl;
        return 0.00;
    }

    return averageData;
}

// process ACC and AP, then updata cities vector<City>
// param: cities, cloudInfos, pressureInfos
vector<City> processACCAP(vector<City> &cities, vector<Info> cloudCoverInfos, vector<Info> pressureInfos)
{
    for (City &city : cities)
    {
        // get ACC and AP
        float ACC = getAverage(city, cloudCoverInfos, "cloudcover");
        float AP = getAverage(city, pressureInfos, "pressure");
        city.ACC = ACC;
        city.AP = AP;
    }

    return cities;
}


// for cities dataset testing purpose
void printCities(vector<City> cities)
{
    for (const City &city : cities)
    {
        cout << fixed << setprecision(2);
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
void printSummaryReport(const vector<City> &cities)
{
    std::string asciiArray[9] = {
        "~~~~\n~~~~~\n\\\\\\\\\\", // Row 1
        "~~~~\n~~~~~\n \\\\\\\\", // Row 2
        "~~~~\n~~~~~\n   \\\\\\", // Row 3
        "~~~~\n~~~~~\n    \\\\", // Row 4
        "~~~~\n~~~~~\n    \\", // Row 5
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
    for (const City &city : cities)
    {
        // L,M,H 조건식만들고 probability도 조건식과 그에 맞는 ASCII 코드 매칭
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