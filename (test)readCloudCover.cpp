#include "my_func.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

vector<Info> readCloudCover(ifstream& inFile)
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

        tuple<int, int> coord_temp= coord_to_int(temp.coord); //convert str to two int variable
        temp.coord_x_int = get<0>(coord_temp);
        temp.coord_y_int = get<1>(coord_temp);

        infos.push_back(temp);
    }

    return infos;
}