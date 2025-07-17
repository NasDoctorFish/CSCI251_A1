#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

struct Info {
    std::string coord;
    int coord_x_int;
    int coord_y_int;
    std::string number;
    std::string name;
};

int main()
{
    vector<Info> infos = {
        {"[1, 1]", 1, 1, "3", "Big_City"},
        {"[1, 2]", 1, 2, "3", "Big_City"},
        {"[1, 3]", 1, 3, "3", "Big_City"},
        {"[2, 1]", 2, 1, "3", "Big_City"},
        {"[2, 2]", 2, 2, "3", "Big_City"},
        {"[2, 3]", 2, 3, "3", "Big_City"},
        {"[2, 7]", 2, 7, "2", "Mid_City"},
        {"[2, 8]", 2, 8, "2", "Mid_City"},
        {"[3, 1]", 3, 1, "3", "Big_City"},
        {"[3, 2]", 3, 2, "3", "Big_City"},
        {"[3, 3]", 3, 3, "3", "Big_City"},
        {"[3, 7]", 3, 7, "2", "Mid_City"},
        {"[3, 8]", 3, 8, "2", "Mid_City"},
        {"[7, 7]", 7, 7, "1", "Small_City"}
    };

    int max_x = 12;
    int max_y = 12;
    vector<vector<char>> grid(max_y + 1, vector<char>(max_x + 1, ' '));

    // Put the number character into grid
    for (const Info& info : infos) {
        grid[info.coord_y_int][info.coord_x_int] = info.number[0];
    }

    int chars_in_block = 5;  // space width per block
    const int offset = 2;    // extra offset for border
    int border_left = offset - 1;
    int border_right = max_x + offset + 1;
    int border_bottom = offset - 1;
    int border_top = max_y + offset + 1;

    // Loop from top (max_y + offset + 1) to bottom (0)
    for (int y = max_y + offset + 1; y >= 0; --y) {
        // Loop from left (0) to right (max_x + offset + 1)
        for (int x = 0; x <= max_x + offset + 1; ++x) {

            // Bottom-left corner cell (empty)
            if (x == 0 && y == 0) {
                cout << setw(chars_in_block) << " ";
            }
            // Y-axis index labels on the left
            else if (x == 0 && y >= offset && y - offset <= max_y) {
                cout << setw(chars_in_block) << (y - offset);
            }
            // X-axis index labels at the bottom
            else if (y == 0 && x >= offset && x - offset <= max_x) {
                cout << setw(chars_in_block) << (x - offset);
            }
            // Border lines
            else if (x == border_left || x == border_right || y == border_bottom || y == border_top) {
                cout << setw(chars_in_block) << "#";
            }
            // Inner grid cells with data
            else if (x >= offset && y >= offset && x - offset <= max_x && y - offset <= max_y) {
                cout << setw(chars_in_block) << grid[y - offset][x - offset];
            }
            // Other blank spaces
            else {
                cout << setw(chars_in_block) << " ";
            }
        }
        cout << endl;
    }

    return 0;
}
