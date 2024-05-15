#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>


// Key = Price
// Value = First Node is total quantity, Following nodes are orderIDs at that price
std::unordered_map<int, std::vector<std::string> > GFDActiveSellPrices;
std::unordered_map<int, std::vector<std::string> > GFDActiveBuyPrices;

// Key = orderID
// Value = int array with: price, quantity
std::unordered_map<std::string, int[2]> GFDSellOrders;
std::unordered_map<std::string, int[2]> GFDBuyOrders;

// Lists for keeping track of active offer sell and buy prices
std::vector<int> sellPriceList;
std::vector<int> buyPriceList;

void operationBUY(std::string orderType, std::string orderID, int price, int quantity);
void operationSELL(std::string orderType, std::string orderID, int price, int quantity);
void operationCANCEL(std::string orderID);
void operationMODIFY(std::string orderID, std::string orderType, int newPrice, int newQuantity);
void operationPRINT();

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
           std::string orderType, orderID;
           int price, quantity;
           iss >> orderType >> price >> quantity >> orderID;
           operationBUY(orderType, orderID,  price, quantity);
       }
       else if(operation == "SELL"){
           std::string orderType, orderID;
           int price, quantity;
           iss >> orderType >> price >> quantity >> orderID;
           operationSELL(orderType, orderID,  price, quantity);
       }
       else if(operation == "CANCEL"){
           std::string orderID;
           iss >> orderID;
           operationCANCEL(orderID);
       }
       else if(operation == "MODIFY"){
           std::string orderID, orderType;
           int newPrice, newQuantity;
           iss >> orderID >> orderType >> newPrice >> newQuantity;
           operationMODIFY(orderID, orderType, newPrice, newQuantity);
       }

        std::getline(inputFile, line);
        iss.clear();
        iss.str(line);
        iss >> operation;
    }

    operationPRINT();

    inputFile.close();

    return 0;
}

void operationBUY(std::string orderType, std::string orderID, int price, int quantity){
    std::cout << "Found Buy!" << std::endl;
    return;
}

void operationSELL(std::string orderType, std::string orderID, int price, int quantity){
    return;
}

void operationCANCEL(std::string orderID){
    return;
}

void operationMODIFY(std::string orderID, std::string orderType, int newPrice, int newQuantity){
    return;
}

void operationPRINT(){
    return;
}
