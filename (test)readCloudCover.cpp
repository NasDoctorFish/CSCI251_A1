#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

using namespace std;


// For every txt data type read
struct Info {
    std::string coord;
    int coord_x_int;
    int coord_y_int;
    std::string number;
    std::string name;
};

std::tuple<int, int> coord_to_int(const std::string& coord_input)
{
    std::string temp = coord_input;
    temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());

    int x, y;
    sscanf(temp.c_str(), "[%d,%d]", &x, &y);
    return {x, y};
}

vector<Info> readCloudCover(ifstream& inFile)
{	
    string line, coordinate, number;
    vector<Info> infos;

    while (getline(inFile, line))
    {
        stringstream ss(line);

        Info temp;
        getline(ss, temp.coord, '-');
        getline(ss, temp.number);

        if (temp.coord.empty()) continue;  // wrong line skip


        tuple<int, int> coord_temp= coord_to_int(temp.coord); //convert str to two int variable
        temp.coord_x_int = get<0>(coord_temp);
        temp.coord_y_int = get<1>(coord_temp);
        temp.name = "CloudCover";

        infos.push_back(temp);
    }

    return infos;
}

// can use both for citylocation and cloudcover
void displayCoordinate(const int &start_x, const int &end_x, const int &start_y, const int &end_y, const vector<Info> &infos)
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
                string num_str = grid[y - offset][x - offset];
                int temp_num = (!num_str.empty()) ? stoi(num_str) : 0; // if num_str is not empty, convert is into integer

                // when infos are CloudCover data
                if (infos[0].name == "CloudCover")
                // the value of name in every CloudCover data is "CloudCover"
                {
                    cout << setw(chars_in_block) << temp_num / 10; // Clouscanns
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

int main()
{
    ifstream inData("cloudcover.txt");
    vector<Info> infos = readCloudCover(inData);
    cout << "===== Print Info lists =====" << endl;
    for (size_t i = 0; i < infos.size(); ++i) {
        const Info& info = infos[i];
        cout << "[" << i << "] "
             << "coord: " << info.coord << ", "
             << "x: " << info.coord_x_int << ", "
             << "y: " << info.coord_y_int << ", "
             << "number: " << info.number << ", "
             << "name: " << info.name << endl;
    }
    cout << "===========================" << endl;
    

    displayCoordinate(0, 8, 0, 8, infos);

    return 0;
}