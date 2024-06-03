#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class MarketOrders{
private:
   // Key = Price
   // Value = First Node is total quantity, Following nodes are orderIDs at that price
   unordered_map<int, vector<string>> GFDActiveSellPrices;
   unordered_map<int, vector<string>> GFDActiveBuyPrices;

   // Key = orderID
   // Value = int array with: price, quantity
   unordered_map<string, int[2]> GFDSellOrders;
   unordered_map<string, int[2]> GFDBuyOrders;

   // Lists for keeping track of active offer sell and buy prices
   vector<int> sellPriceList;
   vector<int> buyPriceList;

   /*
    * @brief Function to check if the given orderID is already in use
    * @param orderID: The orderID being checked.
    */
   bool orderIDInUse(string orderID){
       if(GFDSellOrders.find(orderID) != GFDSellOrders.end() || GFDBuyOrders.find(orderID) != GFDBuyOrders.end()){
           return true; // Found orderID
       }
       else {
           return false; // No orderID found
       }
   }

   /*
    * @brief Function to remove a given price from the list keeping track of
    *.       active sell prices.
    * @param price: The price to remove from the active sell prices list.
    */
   void removePriceFromSellList(int price){
       auto priceIndex = std::find(sellPriceList.begin(), sellPriceList.end(), price);
       sellPriceList.erase(priceIndex);
   }

   /*
    * @brief Function to remove a given price from the list keeping track of
    *.       active buy prices.
    * @param price: The price to remove from the active buy prices list.
    */
   void removePriceFromBuyList(int price){
       auto priceIndex = std::find(buyPriceList.begin(), buyPriceList.end(), price);
       buyPriceList.erase(priceIndex);
   }

   /*
    * @brief Function to remove an order from active sell prices list.
    * @param orderID: The orderID to be removed.
    * @param price: The price to be removed.
    * @param quantity: The quantity of contracts associated with that order.
    */
   void removeOrderIDFromActiveSellPrices(string orderID, int price, int quantity){
       // find orderID in GFDActiveSellPrices and remove it
       auto orderIDIndex = std::find(GFDActiveSellPrices[price].begin(), GFDActiveSellPrices[price].end(), orderID);
       GFDActiveSellPrices[price].erase(orderIDIndex);

       // update quantity
       int originalQuantity = stoi(GFDActiveSellPrices[price][0]);
       GFDActiveSellPrices[price][0] = to_string(originalQuantity - quantity);
   }

   /*
    * @brief Function to remove an order from active buy prices list.
    * @param orderID: The orderID to be removed.
    * @param price: The price to be removed.
    * @param quantity: The quantity of contracts associated with that order.
    */
   void removeOrderIDFromActiveBuyPrices(string orderID, int price, int quantity){
       // find orderID in GFDActiveBuyPrices and remove it
       auto orderIDIndex = std::find(GFDActiveBuyPrices[price].begin(), GFDActiveBuyPrices[price].end(), orderID);
       GFDActiveBuyPrices[price].erase(orderIDIndex);

       // update quantity
       int originalQuantity = stoi(GFDActiveBuyPrices[price][0]);
       GFDActiveBuyPrices[price][0] = to_string(originalQuantity - quantity);
   }

   /*
    * @brief Function to find the lowest price at or under the given price in sell offer list
    * @param price: The price to look for or cheaper
    * @return The number if found, -1 if not found
    */
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

   /*
    * @brief Function to find the highest price at or above the given price in buy offer list
    * @param price: The price to look for or higher
    * @return The number if found, -1 if not found
    */
   int findThisPriceOrHigherBuyPrice(int price){

       if(buyPriceList.empty()){
           return -1; //none Found
       }

       sort(buyPriceList.begin(), buyPriceList.end(), [](int a, int b){return a > b;});

       if(buyPriceList[0] >= price){
           return buyPriceList[0];
       }

       return -1; //none Found
   }

   /*
    * @brief Function to add new GFD BUY order.
    * @param price: The price of the new buy order.
    * @param orderID: The orderID of the new buy order.
    * @param quantity: The quantity of the new buy order.
    */
   void addNewGFDBuyOrder(int price, string orderID, int quantity){
       if (GFDActiveBuyPrices.find(price) != GFDActiveBuyPrices.end()){
           // Add new orderID to existing price in GFDActiveBuyPrices
           int oldQuantity = stoi(GFDActiveBuyPrices[price][0]);
           GFDActiveBuyPrices[price][0] = to_string(oldQuantity + quantity);
           GFDActiveBuyPrices[price].push_back(orderID);
       }
       else{
           // Add new price to GFDActiveBuyPrices
           GFDActiveBuyPrices[price] = {to_string(quantity), orderID};
           buyPriceList.push_back(price);
       }

       // Update GFDBuyOrders with new order
       GFDBuyOrders[orderID][0] = price;
       GFDBuyOrders[orderID][1] = quantity;
   }

   /*
    * @brief Function to add new GFD SELL order.
    * @param price: The price of the new sell order.
    * @param orderID: The orderID of the new sell order.
    * @param quantity: The quantity of the new sell order.
    */
   void addNewGFDSellOrder(int price, string orderID, int quantity){
       if (GFDActiveSellPrices.find(price) != GFDActiveSellPrices.end()){
           // Add new orderID to existing price in GFDActiveSellPrices
           int oldQuantity = stoi(GFDActiveSellPrices[price][0]);
           GFDActiveSellPrices[price][0] = to_string(oldQuantity + quantity);
           GFDActiveSellPrices[price].push_back(orderID);
       }
       else{
           // Add new price to GFDActiveSellPrices
           GFDActiveSellPrices[price] = {to_string(quantity), orderID};
           sellPriceList.push_back(price);
       }

       // Update GFDBuyOrders with new order
       GFDSellOrders[orderID][0] = price;
       GFDSellOrders[orderID][1] = quantity;
   }

   /*
    * @brief Function to resolve trades with possible buy trades using currently
    *        available GFD sell offers.
    * @param price: Price of trades we are working with.
    * @param orderID: The orderID of the current request we are trying to fulfill.
    * @param quantity: The number of contracts we are trying to close.
    */
   int resolveBuyTrades(int price, int foundPrice, string orderID, int quantity){
       int possibleTrades = min(quantity, stoi(GFDActiveSellPrices[foundPrice][0]));
       int tradesDone = possibleTrades;

       bool noOffersLeftOver = false;
       if(stoi(GFDActiveSellPrices[foundPrice][0]) == quantity){
           noOffersLeftOver = true;
       }

       while(possibleTrades != 0){
           string orderIDToRemove = GFDActiveSellPrices[foundPrice][1];
           int quantityAvailable = GFDSellOrders[orderIDToRemove][1];
           int quantityToRemove;

           if(possibleTrades > quantityAvailable){
               quantityToRemove = quantityAvailable;
           }
           else{
               quantityToRemove = possibleTrades;
           }

           cout << "TRADE " << orderIDToRemove << " " << foundPrice << " " << quantityToRemove
                   << " " << orderID << " " << price << " " << quantityToRemove << endl;

           possibleTrades = possibleTrades - quantityToRemove;

           if(possibleTrades == 0){
               if(GFDSellOrders[orderIDToRemove][1] - quantityToRemove == 0){
                   removeOrderIDFromActiveSellPrices(orderIDToRemove, foundPrice, quantityToRemove);
                   GFDSellOrders.erase(orderIDToRemove);
               }
               else{
                   GFDActiveSellPrices[foundPrice][0] = to_string(stoi(GFDActiveSellPrices[foundPrice][0]) - quantityToRemove);
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

   /*
    * @brief Function to resolve trades with possible sell trades using currently
    *        available GFD buy offers.
    * @param price: Price of trades we are working with.
    * @param orderID: The orderID of the current request we are trying to fulfill.
    * @param quantity: The number of contracts we are trying to close.
    */
   int resolveSellTrades(int price, int foundPrice, string orderID, int quantity){
       int possibleTrades = min(quantity, stoi(GFDActiveBuyPrices[foundPrice][0]));

       int tradesDone = possibleTrades;

       bool noOffersLeftOver = false;
       if(stoi(GFDActiveBuyPrices[foundPrice][0]) == quantity){
           noOffersLeftOver = true;
       }

       while(possibleTrades != 0){
           string orderIDToRemove = GFDActiveBuyPrices[foundPrice][1];
           int quantityAvailable = GFDBuyOrders[orderIDToRemove][1];
           int quantityToRemove;

           if(possibleTrades > quantityAvailable){
               quantityToRemove = quantityAvailable;
           }
           else{
               quantityToRemove = possibleTrades;
           }

           cout << "TRADE " << orderIDToRemove << " " << foundPrice << " " << quantityToRemove
                   << " " << orderID << " " << price << " " << quantityToRemove << endl;

           possibleTrades = possibleTrades - quantityToRemove;

           if(possibleTrades == 0){
               if(GFDBuyOrders[orderIDToRemove][1] - quantityToRemove == 0){
                   removeOrderIDFromActiveBuyPrices(orderIDToRemove, foundPrice, quantityToRemove);
                   GFDBuyOrders.erase(orderIDToRemove);
               }
               else{
                   GFDActiveBuyPrices[foundPrice][0] = to_string(stoi(GFDActiveBuyPrices[foundPrice][0]) - quantityToRemove);
                   GFDBuyOrders[orderIDToRemove][1] = GFDBuyOrders[orderIDToRemove][1] - quantityToRemove;
               }
           }
           else{
               removeOrderIDFromActiveBuyPrices(orderIDToRemove, foundPrice, quantityToRemove);
               GFDBuyOrders.erase(orderIDToRemove);
           }
       }
      
       if(noOffersLeftOver || stoi(GFDActiveBuyPrices[foundPrice][0]) == 0){
           GFDActiveBuyPrices.erase(foundPrice);
           removePriceFromBuyList(foundPrice);
       }

       return quantity - tradesDone;
   }

public:
   MarketOrders(){};
   ~MarketOrders(){}; // double check for memory leaks

   /*
    * @brief Function to create a new buy order.
    * @param orderType: IOC or GFD
    * @param orderID: The ID for this order.
    * @param price: The price the order will work with.
    * @param quantity: The number of contracts this order is for.
    * STDIN Input Example: BUY IOC/GFD price quantity orderID
    */
   void operationBUY(string orderType, string orderID, int price, int quantity){

       //TEST CODE:
       //cout << "_\nBUY " << orderType << " " << price << " " << quantity << " " << orderID << endl;
       //operationPRINT();

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
   }


   /*
    * @brief Function to create a new sell order.
    * @param orderType: IOC or GFD
    * @param orderID: The ID for this order.
    * @param price: The price the order will work with.
    * @param quantity: The number of contracts this order is for.
    * STDIN Input Example: SELL IOC/GFD price quantity orderID
    */
   void operationSELL(string orderType, string orderID, int price, int quantity){
      
       //TEST CODE:
       //cout << "_\nSELL " << orderType << " " << price << " " << quantity << " " << orderID << endl;
       //operationPRINT();
      
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

       int goodPrice = findThisPriceOrHigherBuyPrice(price);
       while(goodPrice != -1 && quantity > 0){
           quantity = resolveSellTrades(price, goodPrice, orderID, quantity);
           goodPrice = findThisPriceOrHigherBuyPrice(price);
       }

       if (quantity > 0 && orderType == "GFD"){
           addNewGFDSellOrder(price, orderID, quantity);
       }
   }

   /*
    * @brief Function to cancel a active buy or sell order based on its' orderID.
    * @param orderID: The orderID of the order that will be canceled.
    * STDIN Input Example: CANCEL orderID
    */
   void operationCANCEL(string orderID){

       if(!orderIDInUse(orderID)){
           return;
       }

       // Remove any Sell orders with the given orderID
       if (GFDSellOrders.find(orderID) != GFDSellOrders.end()){
           int price = GFDSellOrders[orderID][0];
           int quantity = GFDSellOrders[orderID][1];
          
           GFDSellOrders.erase(orderID);
          
           if (stoi(GFDActiveSellPrices[price][0]) == quantity){
               GFDActiveSellPrices.erase(price);
               removePriceFromSellList(price);
           }
           else{
               removeOrderIDFromActiveSellPrices(orderID, price, quantity);
           }
       }

       // Remove any Buy orders with the given orderID
       if (GFDBuyOrders.find(orderID) != GFDBuyOrders.end()){
           int price = GFDBuyOrders[orderID][0];
           int quantity = GFDBuyOrders[orderID][1];

           GFDBuyOrders.erase(orderID); // TODO: seg fault here?

           if (stoi(GFDActiveBuyPrices[price][0]) == quantity){
               GFDActiveBuyPrices.erase(price);
               removePriceFromBuyList(price);
           }
           else{
               removeOrderIDFromActiveBuyPrices(orderID, price, quantity);
           }
       }
   }


   /*
    * @brief Function to change an existing orders' type, price, and quantity.
    * @param orderID: The ID for this order.
    * @param orderType: BUY or SELL
    * @param newPrice: The price the new order will be working with.
    * @param newQuantity: The new number of contracts this order is for.
    * STDIN Input Example: MODIFY orderID BUY/SELL newPrice newQuantity
    */
   void operationMODIFY(string orderID, string orderType, int newPrice, int newQuantity){

       if(newPrice <= 0){
           return;
       }
       if(newQuantity <= 0){
           return;
       }
       if(orderType != "BUY" && orderType != "SELL"){
           return;
       }

       if(orderType == "BUY"){
           operationCANCEL(orderID);
           operationBUY("GFD", orderID, newPrice, newQuantity);
       }
       else if(orderType == "SELL"){
           operationCANCEL(orderID);
           operationSELL("GFD", orderID, newPrice, newQuantity);
       }
   }

   /*
    * @brief Function that prints remaining GFD sell and buy quantities at
    *        each price in price descending order.
    * STDIN Input Example: PRINT
    */
   void operationPRINT(){
       cout << "SELL:" << endl;
       sort(sellPriceList.begin(), sellPriceList.end(), [](int a, int b){return a > b;});
       for(int i = 0; i < sellPriceList.size(); i++){
           cout << sellPriceList[i] << " " << GFDActiveSellPrices[sellPriceList[i]][0] << endl;
       }

       cout << "BUY:" << endl;
       sort(buyPriceList.begin(), buyPriceList.end(), [](int a, int b){return a > b;});
       for(int i = 0; i < buyPriceList.size(); i++){
           cout << buyPriceList[i] << " " << GFDActiveBuyPrices[buyPriceList[i]][0] << endl;
       }
   }
};

int main() {

   MarketOrders myMarket;

   string operation;
   string line;
   getline(cin, line);
   istringstream iss(line);
   iss >> operation;

   while(operation != "PRINT"){
       if(operation == "BUY"){
           string orderType, orderID;
           int price, quantity; 
           iss >> orderType >> price >> quantity >> orderID;
           myMarket.operationBUY(orderType, orderID,  price, quantity);
       }
       else if(operation == "SELL"){
           string orderType, orderID;
           int price, quantity;
           iss >> orderType >> price >> quantity >> orderID;
           myMarket.operationSELL(orderType, orderID,  price, quantity);
       }
       else if(operation == "CANCEL"){
           string orderID;
           iss >> orderID;
           myMarket.operationCANCEL(orderID);
       }
       else if(operation == "MODIFY"){
           string orderID;
           string orderType;
           int newPrice;
           int newQuantity;
           iss >> orderID >> orderType >> newPrice >> newQuantity;
           myMarket.operationMODIFY(orderID, orderType, newPrice, newQuantity);
       }

       string line;
       getline(cin, line);
       iss.clear(); // Clear any errors from bad prior input lines
       iss.str(line);
       iss >> operation;
   }

   myMarket.operationPRINT();

   return 0;
}
