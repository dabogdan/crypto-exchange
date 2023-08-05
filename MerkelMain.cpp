#include "MerkelMain.h"
#include <iostream>
#include <map>
#include <vector>
#include "CSVReader.h"


MerkelMain::MerkelMain(){

}

/** Call this to start the app*/
void MerkelMain::init(){    
    currentTime = orderBook.getEarliestTime();

    int input;

    wallet.insertCurrency("BTC", 10);

    while (true) {
        printMenu();
        input = getUserInput();
        processUserInput(input);
    }
}
void MerkelMain::printMenu(){
    std::cout << "===========================" << std::endl;
    std::cout << "1. Print help." << std::endl;
    std::cout << "2. Print exchange stats." << std::endl;
    std::cout << "3. Make an offer." << std::endl;
    std::cout << "4. Make an bid." << std::endl;
    std::cout << "5. Print a wallet." << std::endl;
    std::cout << "6. Continue." << std::endl;
    std::cout << "7. Draw exchange data." << std::endl;
    std::cout << "8. Draw exchange volume." << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "Current time is: " << currentTime << std::endl;
}

int MerkelMain::getUserInput(){
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-8" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    } catch (const std::exception& e){
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::printHelp(){
    std::cout << "Help - your aim is to make money. Analyse market and make offers and bids!" << std::endl;
}
            
void MerkelMain::printMarketStats(){

    for (std::string const& p : orderBook.getKnownProducts()){      
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
        std::cout << "Asks seen: " << askEntries.size() << std::endl;
        std::cout << "Max asks: " << OrderBook::getHighPrice(askEntries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(askEntries) << std::endl;
        std::cout << "Bids seen: " << bidEntries.size() << std::endl;
        std::cout << "Max bid: " << OrderBook::getHighPrice(bidEntries) << std::endl;
        std::cout << "Min bid: " << OrderBook::getLowPrice(bidEntries) << std::endl;
    }
}
            
void MerkelMain::enterAsk(){
    std::cout << "Make an ask - enter the amount: product,price,amount, eg ETH/BTC,200,0.5" << std::endl;    
    std::string input;    
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if(tokens.size() != 3) {
        std::cout << "Bad input! " << input << std::endl;
    } else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask
            );

            obe.username = "simuser";

            if(wallet.canFulfilOrder(obe)){
                std::cout << "Wallet looks good." << std::endl;
                orderBook.insertOrder(obe);
            } else
                std::cout << "Wallet has insufficient funds." << std::endl;
            
        } catch (const std::exception& e){
            std::cout << "MerkelMain::enterAsk Bad input: " << input << std::endl;
        }
    }

    std::cout << "Your ask is entered: " << input << std::endl;
}
    
void MerkelMain::enterBid(){
    std::cout << "Make a bid - enter the amount: product,price,amount, eg ETH/BTC,200,0.5" << std::endl;    
    std::string input;    
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if(tokens.size() != 3) {
        std::cout << "MerkelMain::enterBid : bad input! " << input << std::endl;
    } else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid
            );

            obe.username = "simuser";

            if(wallet.canFulfilOrder(obe)){
                std::cout << "Wallet looks good." << std::endl;
                orderBook.insertOrder(obe);
            } else
                std::cout << "Wallet has insufficient funds." << std::endl;
        } catch (const std::exception& e){
            std::cout << "MerkelMain::enterBid bBad input: " << input << std::endl;
        }
    }

    std::cout << "Your bid is entered: " << input << std::endl;
}

void MerkelMain::printWallet(){
    std::cout << "Your wallet is: " << wallet.toString() << std::endl;
}

// =========== beginning of my code ============

void MerkelMain::goToNextTimeFrame(){
    std::cout << "Going to the next time frame." << std::endl;
    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookEntry& sale : sales) {
        std::cout << "Sale price: " << sale.price << " amount: " << sale.amount << std::endl;
        if(sale.username == "simuser") {
            //update the waller
            wallet.processSale(sale);
        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}

void MerkelMain::drawExchangeData(){

    std::cout << "Enter the product and sales type, eg ETH/BTC,bids" << std::endl;    
    std::string input;    
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if(tokens.size() != 2) {
        std::cout << "MerkelMain::drawExchangeData: bad input! " << input << std::endl;
    } else {
        try {

            OrderBookType type;
            if (tokens[1] == "ask"){
                type = OrderBookType::ask;
            }

            if (tokens[1] == "bid"){
                type = OrderBookType::bid;
            }
            
            //obtain the slice of entries for the candlestick
            std::vector<OrderBookEntry> entries = orderBook.getOrders(type, tokens[0]);

            // getting vector of unique timestamps
            std::vector<std::string> uniqueTimestamps; 
            uniqueTimestamps = orderBook.getUniqueTimestamps(entries, 15);

            //create vector of candlestick objects for each timestamp
            std::vector<Candlestick> candlesticks = orderBook.computeCandlestickData(uniqueTimestamps, orderBook, type, tokens[0]);
            
            // draw the chart
            CandleChart chart{"Candlestick chart for " + tokens[0]};
            chart.drawChart(candlesticks, uniqueTimestamps);

        } catch (const std::exception& e){
            std::cout << "MerkelMain::drawExchangeData bad input: " << input << std::endl;
        }
    }
}

void MerkelMain::drawVolume() {
    std::cout << "Enter the product type, eg ETH/BTC,bids" << std::endl;    
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if(tokens.size() != 2) {
        std::cout << "MerkelMain::drawVolume: bad input! " << input << std::endl;
    } else {
        try {            
            OrderBookType type;
            if (tokens[1] == "ask"){
                type = OrderBookType::ask;
            }

            if (tokens[1] == "bid"){
                type = OrderBookType::bid;
            }
            //obtain the slice of entries for the candlestick
            std::vector<OrderBookEntry> entries = orderBook.getOrders(type, tokens[0]);

            // getting vector of unique timestamps
            std::vector<std::string> uniqueTimestamps; 
            uniqueTimestamps = orderBook.getUniqueTimestamps(entries, 15);

            // //create vector of bars objects for each timestamp
            std::vector<Bar> bars = orderBook.computeBarData(uniqueTimestamps, orderBook, type, tokens[0]);
            
            // // draw the chart
            BarChart chart{"Bar Chart for " + tokens[0] + ", "  + tokens[1] + "total financial volume (amount x price) per time interval."};
            chart.drawChart(bars, uniqueTimestamps);

        } catch (const std::exception& e){
            std::cout << "MerkelMain::drawVolume bad input: " << input << std::endl;
        }
    }
}
// =========== end of my code ============

void MerkelMain::processUserInput (int userInput) {
    switch(userInput){
        case 1: printHelp(); break;
        case 2: printMarketStats(); break;
        case 3: enterAsk(); break;
        case 4: enterBid(); break;
        case 5: printWallet(); break;
        case 6: goToNextTimeFrame(); break;
        case 7: drawExchangeData(); break;
        case 8: drawVolume(); break;
        default:
            std::cout << "Error: unknown input. Type 1-8." << std::endl;
    }
}