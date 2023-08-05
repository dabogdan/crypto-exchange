#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include "Bar.h"
#include <string>
#include <vector>

class OrderBook {
    public:
    /** construct reading a csv data file */
        OrderBook(std::string filename);
    /** return verctor of all the products in the dataset*/
        std::vector<std::string> getKnownProducts();
    /** return vector of orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type, std::string product, std::string timestamp = "all");
    /** returns the earliest time in the orderbook */
        std::string getEarliestTime();
    /** returns the mean price amongst the orders */
        double getMeanPrice(std::vector<OrderBookEntry>& orders);
    /** returns the next time after the sent time in the orderbook. If there is no timestamp it wraps around to the start */
        std::string getNextTime(std::string timestamp);
    /** returns unique datastamps of the slice of orders*/ 
        std::vector<std::string> getUniqueTimestamps(std::vector<OrderBookEntry>& orders, int howMany = 1);
        double getVolume(std::vector<OrderBookEntry>&);
    /** returns vector of computed candlestick objects*/ 
        std::vector<Candlestick> computeCandlestickData(std::vector<std::string>& uniqueTimestamps, 
                                                        OrderBook& orderBook, 
                                                        OrderBookType type,
                                                        std::string product);
        /** returns vector of computed bar objects*/ 
        std::vector<Bar> computeBarData(std::vector<std::string>& uniqueTimestamps, 
                                                        OrderBook& orderBook, 
                                                        OrderBookType type,
                                                        std::string product);


        void insertOrder(OrderBookEntry& order);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);
        
    
    private:
        std::vector<OrderBookEntry> orders;
        


};