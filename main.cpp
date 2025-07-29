#include <iostream>
#include <fstream>
#include <string>
#include "my_func.h"
// delete #include below after adding all functions into my_func.cpp

#include <array>
#include <vector>
#include <numeric> // for std::accumulate
#include <iomanip> // for setprecision()

using namespace std; // fstream defined in std, need to mention std
//// MODIFFIED PART ///////////////

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
    string id;
    vector<Coord> locations; // city area of this city
    vector<Coord> surroundings;
    float ACC; // Average Cloud Cover
    float AP;  // Average Pressure
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
        cout << "Average: " << averageData << endl;
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

// printAsciiValue based on rainProbability line matched
void printAsciiVisual(int rainProbability) {
    using std::cout;
    using std::endl;
    using std::setw;

    if (rainProbability == 90) {
        cout << setw(5) << right << "~~~~" << endl;
        cout << setw(5) << right << "~~~~~" << endl;
        cout << setw(5) << right << "\\\\\\\\\\" << endl;    
    }
    else if (rainProbability == 80) {
        cout << setw(5) << right << "~~~~" << endl;
        cout << setw(5) << right << "~~~~~" << endl;
        cout << setw(9) << right << "\\\\\\\\" << endl;
    }
    else if (rainProbability == 70) {
        cout << setw(5) << right << "~~~~" << endl;
        cout << setw(5) << right << "~~~~~" << endl;
        cout << setw(11) << right << "\\\\\\" << endl;
    }
    else if (rainProbability == 60) {
        cout << setw(5) << right << "~~~~" << endl;
        cout << setw(5) << right << "~~~~~" << endl;
        cout << setw(13) << right << "\\\\" << endl;
    }
    else if (rainProbability == 50) {
        cout << setw(5) << right << "~~~~" << endl;
        cout << setw(5) << right << "~~~~~" << endl;
        cout << setw(15) << right << "\\" << endl;    
    }
    else if (rainProbability == 40) {
        cout << setw(5) << right << "~~~~" << endl;
        cout << setw(5) << right << "~~~~~" << endl;
        cout << endl;
    }
    else if (rainProbability == 30) {
        cout << setw(5) << right << "~~~" << endl;
        cout << setw(5) << right << "~~~~" << endl;
        cout << endl;
    }
    else if (rainProbability == 20) {
        cout << setw(5) << right << "~~" << endl;
        cout << setw(5) << right << "~~~" << endl;
        cout << endl;
    }
    else if (rainProbability == 10) {
        cout << setw(5) << right << "~" << endl;
        cout << setw(5) << right << "~~" << endl;
        cout << endl;
    }
    else {
        cout << "(Invalid rainProbability)" << endl;
    }
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
        cout << "asciiIndex: " << asciiIndex << endl;

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

// MODIFIED PART ////////////////////////////////////////////////////

int main()
{
    // data-storing variables
    string options[8] = {
        "Read in and process a configuration file",
        "Display city map",
        "Display cloud coverage map (cloudiness index)",
        "Display cloud coverage map (LMH symbols)",
        "Display atmospheric pressure map (pressure index)",
        "Display atmospheric pressure map (LMH symbols)",
        "Show weather forecast summary report",
        "Quit"};
    std::array<int, 2> x_idx_result;
    std::array<int, 2> y_idx_result;
    vector<Info> cityLocationInfos;
    vector<Info> cloudInfos;
    vector<Info> pressureInfos;
    // fileData dynamic allocation array
    string *fileData = nullptr;

    // Infinite loop
    while (true)
    {
        // step 2-0: display main menu
        displayMainMenu();
        int userInput;

        cin >> userInput; // get user input
        // check if the user didn't yet process config file
        if (userInput >= 2 && userInput <= 7 && fileData == nullptr)
        {
            cout << endl;
            cout << "Config file required. please proceed to step 1." << endl;
            cout << endl;
            continue; // Back to main menu
        }
        cout << endl;
        switch (userInput)
        {
        case 1:
        {
            cout << "[" << options[0] << "]" << endl;
            cout << endl;
            // ------------------------------------ Get user input ------------------------------------
            string file;
            cout << "Please enter config file :";
            cin >> file;
            cin.ignore();

            // string *txtFileReader(const string &inputFilename)
            //  txt file reader
            if (fileData != nullptr)
                delete[] fileData;
            fileData = ConfigFileReader(file);

            // ------------------------------------ Access data from config.txt ------------------------------------
            string x_idxRange, y_idxRange;
            x_idxRange = fileData[0];
            y_idxRange = fileData[1];

            // ------------------------------------ process Grid X and Y ranges ------------------------------------
            cout << "Reading in GridX_IdxRange : ";
            x_idx_result = process_X_Y_str(x_idxRange); // extract the end index number of x
            cout << x_idx_result[0] << "-" << x_idx_result[1] << " ... done!" << endl;
            cout << "Reading in GridY_IdxRange : ";
            y_idx_result = process_X_Y_str(y_idxRange); // extract the end index number of y
            cout << y_idx_result[0] << "-" << y_idx_result[1] << " ... done!" << endl;
            cout << endl;
            cout << "Storing data from input file :" << endl;

            // ------------------------------------ Process CityLocation ------------------------------------
            cout << fileData[2]; // cityLocation
            ifstream inFile(fileData[2]);
            if (!inFile)
            {
                cerr << "Cannot open the file!" << endl;
                return 1;
            }
            // process citylocation()
            cityLocationInfos = readCityLocation(inFile);
            cout << " ... done!" << endl;

            // ------------------------------------ Process CloudCover ------------------------------------
            cout << fileData[3];          // cloudCover
            ifstream inData(fileData[3]); // process cloudcover
            if (!inData)
            {
                cerr << "Cannot open the file!" << endl;
                return 1;
            }
            cloudInfos = readElse(inData, "cloudcover");
            inData.close();
            cout << " ... done!" << endl;

            // ------------------------------------ Process pressure ------------------------------------
            cout << fileData[4]; // pressure
            inData.open(fileData[4]);
            if (!inData)
            {
                cerr << "Cannot open the file!" << endl;
                return 1;
            }
            pressureInfos = readElse(inData, "pressure");
            inData.close();
            cout << " ... done!" << endl;

            // process x_y index range

            cout << endl;
            cout << "All records successfully stored. Going back to main menu ..." << endl; // read config file
            cout << endl;
            break;
        }

        case 2: // display city map
        {
            cout << "[" << options[1] << "]" << endl;
            cout << endl;
            // ------------------------------------ Display CityLocation ------------------------------------
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], cityLocationInfos);
            cout << endl;
            cout << endl;
            break;
        }
        case 3: // display cloudiness index
        {
            cout << "[" << options[2] << "]" << endl;
            cout << endl;
            // ------------------------------------ Display CloudCover ------------------------------------
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], cloudInfos);
            cout << endl;
            cout << endl;
            break;
        }
        case 4: // display cloud LMH symbols
        {
            cout << "[" << options[3] << "]" << endl;
            cout << endl;
            // ------------------------------------ Display CloudCover LMH ------------------------------------
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], cloudInfos, true);
            cout << endl;
            cout << endl;
            break;
        }
        case 5: // display atmospheric pressure index
        {
            cout << "[" << options[4] << "]" << endl;
            cout << endl;
            // ------------------------------------ Display pressure ------------------------------------
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], pressureInfos);
            cout << endl;
            cout << endl;
            break;
        }
        case 6: // display atmospheric LMH symbols
        {
            cout << "[" << options[5] << "]" << endl;
            cout << endl;
            // ------------------------------------ Display pressure LMH ------------------------------------
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], pressureInfos, true);
            cout << endl;
            cout << endl;
            break;
        }
        case 7: // show weather forecast summary report
        {
            cout << "[" << options[6] << "]" << endl;

            // vector<Info> cityLocationInfos;
            // vector<Info> cloudInfos;
            // vector<Info> pressureInfos;

            // 2. identify valid citylocation within range of x and y and save it in vector<City>
            vector<City> cities = readCities(cityLocationInfos, x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1]);

            // 3. Generate surrounding areas of city location and make sure it not overlaps and pushback to cities vector<City>
            vector<City> filteredCities = processSurroundings(cities, x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1]);

            // 4. calculate Average Cloud Cover and Average Pressure and save it into cities
            processACCAP(filteredCities, cloudInfos, pressureInfos);

            // 5. Check if cities are properly saved
            printCities(filteredCities);
            printSummaryReport(filteredCities);
            cout << endl;
            cout << endl;
            cout << endl;
            break;
        }
        case 8: // quit
        {
            // cout << "[" << options[7] << "]" << endl;
            delete[] fileData;
            return 0;
        }
        default:
        {
            cout << endl;
            cout << "Please select correct number between 1 to 8." << endl;
            cout << endl;
            break;
        }
        }
        string pauseLine;
        cout << "Press <enter> to go back to main menu ...";
        cin.ignore();
        getline(cin, pauseLine); // wait for "Enter" input
    }

    return 0;
}