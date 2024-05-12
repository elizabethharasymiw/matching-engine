#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]){
    std::ifstream inputFile(argv[1]);

    if(!inputFile.is_open()){
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string operation;
    std::string line;

    std::getline(inputFile, line);
    std::istringstream iss(line);
    iss >> operation;

    while(operation != "PRINT"){
        if(operation == "BUY"){
            std::cout << "BUY found" << std::endl;
        }

        std::getline(inputFile, line);
        iss.clear();
        iss.str(line);
        iss >> operation;
    }

    inputFile.close();

    return 0;
}
