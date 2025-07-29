#include <iostream>
#include <fstream>
#include <string>
#include "my_func_copy.h"
// delete #include below after adding all functions into my_func.cpp

#include <array>
#include <vector>
#include <numeric> // for std::accumulate
#include <iomanip> // for setprecision()

using namespace std; // fstream defined in std, need to mention std
//// MODIFFIED PART ///////////////

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
    int cityCount = 0;
    int surroundingCount = 0;
    Info *cityInfos = new Info[100];
    int cityInfosCount = 0;
    Info *cloudInfos = new Info[100];
    int cloudInfosCount = 0;
    Info *pressureInfos = new Info[100];
    int pressureInfosCount = 0;

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
            cityInfos = readCityLocation(inFile, cityInfosCount);
            cout << " ... done!" << endl;

            // ------------------------------------ Process CloudCover ------------------------------------
            cout << fileData[3];          // cloudCover
            ifstream inData(fileData[3]); // process cloudcover
            if (!inData)
            {
                cerr << "Cannot open the file!" << endl;
                return 1;
            }
            cloudInfos = readElse(inData, "cloudcover", cloudInfosCount);
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
            pressureInfos = readElse(inData, "pressure", pressureInfosCount);
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
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], cityInfos, cityInfosCount);
            cout << endl;
            cout << endl;
            break;
        }
        case 3: // display cloudiness index
        {
            cout << "[" << options[2] << "]" << endl;
            cout << endl;
            // ------------------------------------ Display CloudCover ------------------------------------
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], cloudInfos, cloudInfosCount);
            cout << endl;
            cout << endl;
            break;
        }
        case 4: // display cloud LMH symbols
        {
            cout << "[" << options[3] << "]" << endl;
            cout << endl;
            // ------------------------------------ Display CloudCover LMH ------------------------------------
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], cloudInfos, cloudInfosCount, true);
            cout << endl;
            cout << endl;
            break;
        }
        case 5: // display atmospheric pressure index
        {
            cout << "[" << options[4] << "]" << endl;
            cout << endl;
            // ------------------------------------ Display pressure ------------------------------------
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], pressureInfos, pressureInfosCount);
            cout << endl;
            cout << endl;
            break;
        }
        case 6: // display atmospheric LMH symbols
        {
            cout << "[" << options[5] << "]" << endl;
            cout << endl;
            // ------------------------------------ Display pressure LMH ------------------------------------
            displayCoordinate(x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], pressureInfos, pressureInfosCount, true);
            cout << endl;
            cout << endl;
            break;
        }
        case 7: // show weather forecast summary report
        {
            cout << "[" << options[6] << "]" << endl;

            // vector<Info> cityInfos;
            // vector<Info> cloudInfos;
            // vector<Info> pressureInfos;
            cout << "Before readCities" << endl;
            // 2. identify valid citylocation within range of x and y and save it in vector<City>
            City *cities = readCities(cityInfos, x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], cityInfosCount, cityCount);
            cout << "Before filteredCities" << endl;
            // 3. Generate surrounding areas of city location and make sure it not overlaps and pushback to cities vector<City>
            City *filteredCities = processSurroundings(cities, cityCount, x_idx_result[0], x_idx_result[1], y_idx_result[0], y_idx_result[1], surroundingCount);
            cout << "=== Before summary ===" << endl;
            cout << "cityCount: " << cityCount << endl;
            cout << "first city name: " << filteredCities[0].name << endl;

            cout << "Before processACCAP" << endl;
            // 4. calculate Average Cloud Cover and Average Pressure and save it into cities
            processACCAP(filteredCities, cityCount, cloudInfos, cloudInfosCount, pressureInfos, pressureInfosCount);

            // 5. Check if cities are properly saved
            // printCities(filteredCities); // for testing

            if (filteredCities != nullptr && cityCount > 0)
            {
                printSummaryReport(filteredCities, cityCount);
            }
            else
            {
                cerr << "Error: No filtered cities available." << endl;
            }

            cout << endl;
            cout << endl;
            cout << endl;
            delete[] cities;
            delete[] filteredCities;
            break;
        }
        case 8: // quit
        {
            // cout << "[" << options[7] << "]" << endl;
            delete[] fileData;
            delete[] cityInfos; // delete arrays
            delete[] cloudInfos;
            delete[] pressureInfos;
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