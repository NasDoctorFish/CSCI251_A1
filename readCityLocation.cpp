#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "my_func.h" // coord_to_int, 

using namespace std; //fstream defined in std, need to mention std

/*
    citylocation format
    [1, 1]-3-Big_City
*/

struct Info {
    std::string coord;
    int coord_x_int; //no int in std namespace
    int coord_y_int;
    std::string number;
    std::string name;
};


vector<Info> readCityLocation(ifstream& inFile)
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



int main()
{
    ifstream inFile("citylocation.txt"); // 실제 파일명으로 바꿔주세요
    if (!inFile) {
        cerr << "파일을 열 수 없습니다." << endl;
        return 1;
    }
    vector<Info> infos = readCityLocation(inFile);
    // output result
    for (const auto& info : infos) {
        std::cout << "coord: " << info.coord 
                  << ", number: " << info.number 
                  << ", name: " << info.name 
                  << ", coord_x: " << info.coord_x_int << "(" << typeid(info.coord_x_int).name() << ")"
                  << ", coord_y: " << info.coord_y_int << "(" << typeid(info.coord_y_int).name() << ")"
                  << std::endl;
    }
}


    #   #   #   #   #   #   #