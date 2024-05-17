#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

// Key = Price
// Value = First Node is total quantity, Following nodes are orderIDs at that price
std::unordered_map<int, std::vector<std::string> > GFDActiveSellPrices;
std::unordered_map<int, std::vector<std::string> > GFDActiveBuyPrices;

// Key = orderID
// Value = int array with: price, quantity
std::unordered_map<std::string, int[2] > GFDSellOrders;
std::unordered_map<std::string, int[2] > GFDBuyOrders;

// Lists for keeping track of active offer sell and buy prices
std::vector<int> sellPriceList;
std::vector<int> buyPriceList;

bool orderIDInUse(std::string orderID);
void removePriceFromSellList(int price);
int findThisPriceOrCheaperSellPrice(int price);
void removeOrderIDFromActiveSellPrices(std::string orderID, int price, int quantity);
int resolveBuyTrades(int price, int foundPrice, std::string orderID, int quantity);
void addNewGFDBuyOrder(int price, std::string orderID, int quantity);
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

bool orderIDInUse(std::string orderID){
    if(GFDSellOrders.find(orderID) != GFDSellOrders.end() || GFDBuyOrders.find(orderID) != GFDBuyOrders.end()){
        return true; // Found orderID
    }
    else {
        return false; // No orderID found
    }
}

void removePriceFromSellList(int price){
    auto priceIndex = std::find(sellPriceList.begin(), sellPriceList.end(), price);
    sellPriceList.erase(priceIndex);
}

void removeOrderIDFromActiveSellPrices(std::string orderID, int price, int quantity){
    // find orderID in GFDActiveSellPrices and remove it
    auto orderIDIndex = std::find(GFDActiveSellPrices[price].begin(), GFDActiveSellPrices[price].end(), orderID);
    GFDActiveSellPrices[price].erase(orderIDIndex);

    // update quantity
    int originalQuantity = std::stoi(GFDActiveSellPrices[price][0]);
    GFDActiveSellPrices[price][0] = std::to_string(originalQuantity - quantity);
}

int findThisPriceOrCheaperSellPrice(int price){

    if(sellPriceList.empty()){
        return -1; //none Found
    }

    sort(sellPriceList.begin(), sellPriceList.end());

    if(sellPriceList[0] <= price){
        return sellPriceList[0];
    }

    return -1; //none Found
}

int resolveBuyTrades(int price, int foundPrice, std::string orderID, int quantity){
    int possibleTrades = std::min(quantity, std::stoi(GFDActiveSellPrices[foundPrice][0]));
    int tradesDone = possibleTrades;

    bool noOffersLeftOver = false;
    if(stoi(GFDActiveSellPrices[foundPrice][0]) == quantity){
        noOffersLeftOver = true;
    }

    while(possibleTrades != 0){
        std::string orderIDToRemove = GFDActiveSellPrices[foundPrice][1];
        int quantityAvailable = GFDSellOrders[orderIDToRemove][1];
        int quantityToRemove;

        if(possibleTrades > quantityAvailable){
            quantityToRemove = quantityAvailable;
        }
        else{
            quantityToRemove = possibleTrades;
        }

        std::cout << "TRADE " << orderIDToRemove << " " << foundPrice << " " << quantityToRemove
             << " " << orderID << " " << price << " " << quantityToRemove << std::endl;

        possibleTrades = possibleTrades - quantityToRemove;

        if(possibleTrades == 0){
            if(GFDSellOrders[orderIDToRemove][1] - quantityToRemove == 0){
                removeOrderIDFromActiveSellPrices(orderIDToRemove, foundPrice, quantityToRemove);
                GFDSellOrders.erase(orderIDToRemove);
            }
            else{
                GFDActiveSellPrices[foundPrice][0] = std::to_string(std::stoi(GFDActiveSellPrices[foundPrice][0]) - quantityToRemove);
                GFDSellOrders[orderIDToRemove][1] = GFDSellOrders[orderIDToRemove][1] - quantityToRemove;
            }
        }
        else{
             removeOrderIDFromActiveSellPrices(orderIDToRemove, foundPrice, quantityToRemove);
             GFDSellOrders.erase(orderIDToRemove);
        }
    }

    if(noOffersLeftOver || stoi(GFDActiveSellPrices[foundPrice][0]) == 0){
        GFDActiveSellPrices.erase(foundPrice);
        removePriceFromSellList(foundPrice);
    }

    return quantity - tradesDone;
}

void addNewGFDBuyOrder(int price, std::string orderID, int quantity){
    if (GFDActiveBuyPrices.find(price) != GFDActiveBuyPrices.end()){
        // Add new orderID to existing price in GFDActiveBuyPrices
        int oldQuantity = stoi(GFDActiveBuyPrices[price][0]);
        GFDActiveBuyPrices[price][0] = std::to_string(oldQuantity + quantity);
        GFDActiveBuyPrices[price].push_back(orderID);
    }
    else{
        // Add new price to GFDActiveBuyPrices
        GFDActiveBuyPrices[price][0] = std::to_string(quantity);
        GFDActiveBuyPrices[price][1] = orderID;
        buyPriceList.push_back(price);
    }

    // Update GFDBuyOrders with new order
    GFDBuyOrders[orderID][0] = price;
    GFDBuyOrders[orderID][1] = quantity;

    return;
}

void operationBUY(std::string orderType, std::string orderID, int price, int quantity){

   if(price <= 0){
       return;
   }
   if(quantity <= 0){
       return;
   }
   if(orderType != "IOC" && orderType != "GFD"){
       return;
   }
   if(orderIDInUse(orderID)){
       return;
   }

    int goodPrice = findThisPriceOrCheaperSellPrice(price);
    while(goodPrice != -1 && quantity > 0){
       quantity = resolveBuyTrades(price, goodPrice, orderID, quantity);
       goodPrice = findThisPriceOrCheaperSellPrice(price);
    }

    if (quantity > 0 && orderType == "GFD"){
        addNewGFDBuyOrder(price, orderID, quantity);
    }

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
   std::cout << "SELL:" << std::endl;
   std::sort(sellPriceList.begin(), sellPriceList.end(), [](int a, int b){return a > b;});
   for(int i = 0; i < sellPriceList.size(); i++){
       std::cout << sellPriceList[i] << " " << GFDActiveSellPrices[sellPriceList[i]][0] << std::endl;
   }

    std::cout << "BUY:" << std::endl;
    std::sort(buyPriceList.begin(), buyPriceList.end(), [](int a, int b){return a > b;});
    for(int i = 0; i < buyPriceList.size(); i++){
        std::cout << buyPriceList[i] << " " << GFDActiveBuyPrices[buyPriceList[i]][0] << std::endl;
    }
    return;
}
