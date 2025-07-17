#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio> //sscanf
#include "my_func.h"

using namespace std; //fstream defined in std, need to mention std


// tuple<int, int> coord_to_int(string& coord_input)
// {
//     string temp;
//     int x,y;
//     stringstream ss(coord_input);
//     ss >> temp >> x >> temp >> y >> temp;
//     return {x, y};
// }

tuple<int, int> coord_to_int(const string& coord_input)
{
    int x, y;
    sscanf(coord_input.c_str(), "[%d, %d]", &x, &y); //can I use this?: traditional C string scan formatted
    return {x, y};
}
// in mac don't use .app; OS recognizes as app bundle file, executing in terminal => kill



int main()
{
    tuple<int, int> coord =  coord_to_int("[1, 1]");
    int x = get<0>(coord);
    int y = get<1>(coord);
    cout << "x: " << x << "y: " << y << endl;
    return 0;
}