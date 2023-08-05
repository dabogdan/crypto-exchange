#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader(){

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename){
    std::vector<OrderBookEntry> entries;
    std::ifstream csvFile{csvFilename};
    std::string line;

    if(csvFile.is_open()){
        while(std::getline(csvFile, line)) {
            try {
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            } catch (const std::exception& e) {
                std::cout << "CSVReader::readCSV bad data at line: " << line << std::endl;
            }
        }//end of while
    }

    std::cout << "SCVReader::readCSV read " << entries.size() << " entries" << std::endl;

    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator){
    std::vector<std::string> tokens;
    std::string token;
    signed int start, end;
    start = csvLine.find_first_not_of(separator, 0);
    
    do {
        end = csvLine.find_first_of(separator, start);
        if(start == csvLine.length() || start == end) break;
        if(end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end != std::string::npos);

    return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens){
    double price, amount;
    if(tokens.size() != 5) {
        std::cout << "Bad line" << std::endl; 
        throw std::exception{};
    }
    
    try {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    } catch (const std::exception& e) {
        std::cout << "CSVReader::stringsToOBE Bad float: " << tokens[3] << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float: " << tokens[4] << std::endl;
        throw;
    }

    OrderBookEntry obe {    
        price, 
        amount, 
        tokens[0], 
        tokens[1], 
        OrderBookEntry::stringToOrderBookType(tokens[2])
    };

    return obe;
}

OrderBookEntry CSVReader::stringsToOBE( std::string priceStr,
                                        std::string amountStr,
                                        std::string timestamp,
                                        std::string product,
                                        OrderBookType orderType){

    double price, amount;
    try {
        price = std::stod(priceStr);
        amount = std::stod(amountStr);
    } catch (const std::exception& e) {
        std::cout << "CSVReader::stringsToOBE Bad float: " << priceStr << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float: " << amountStr << std::endl;
        throw;
    }
    
    OrderBookEntry obe{    
        price, 
        amount, 
        timestamp, 
        product, 
        orderType
    };
    return obe;
};