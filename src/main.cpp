#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
    std::ifstream inputFile(argv[1]);

    if(!inputFile.is_open()){
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    while(std::getline(inputFile, line)){
        std::cout << line << std::endl;
    }

    inputFile.close();

    return 0;
}
