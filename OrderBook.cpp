#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <iostream>

/** construct reading a csv data file */
OrderBook::OrderBook(std::string filename){
    orders = CSVReader::readCSV (filename);
};
/** return verctor of all the products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts(){
    std::vector<std::string> products;

    std::map<std::string,bool> prodMap;

    for (OrderBookEntry& e : orders){
        prodMap[e.product] = true;
    }

    //now flatten the map to a vector of strings
    for(auto const& e: prodMap){
        products.push_back(e.first);
    }

    return products;
};
/** return vector of orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, std::string product, std::string timestamp){
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders) {
        if ((e.orderBookType == type && e.product == product) && (timestamp == "all" || e.timestamp == timestamp))
            orders_sub.push_back(e);    
    }
        return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders){
    double max = orders[0].price;
    for (OrderBookEntry& e : orders){
        if(e.price > max) max = e.price;
    }
    return max;
};

double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders){
    double min = orders[0].price;
    for(OrderBookEntry e : orders){
        if(e.price < min) min = e.price;
    }
    return min;
}

double OrderBook::getMeanPrice(std::vector<OrderBookEntry>& orders){
    double mean;
    double totalPrice = 0.0;
    double totalAmount = 0.0;
    for (OrderBookEntry& e : orders){
        totalPrice += (e.price * e.amount);
        totalAmount += e.amount;
    }
    mean = (double) totalPrice / (double) totalAmount;
    
    return mean;
}

std::string OrderBook::getEarliestTime(){
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp){
    std::string next_timestamp = "";
    for(OrderBookEntry& e : orders) {
        if (e.timestamp > timestamp){
            next_timestamp = e.timestamp;
            break;
        }
    }
    if(next_timestamp == ""){
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

// =========== bedinning of my code ============

std::vector<std::string> OrderBook::getUniqueTimestamps(std::vector<OrderBookEntry>& orders, int howMany){
    std::vector<std::string> allTimes;
    std::string timestampComparer = "";
    int counter = 0;

    for (OrderBookEntry& entry : orders) {
        if (entry.timestamp != timestampComparer) {
            allTimes.push_back(entry.timestamp);
            timestampComparer = entry.timestamp;
            counter++;
        }

        if (counter >= howMany) {
            break;
        }
    }
    return allTimes;
}

double OrderBook::getVolume(std::vector<OrderBookEntry>& orders){
    double volume = 0.0;
    for (const OrderBookEntry& order : orders){
        volume = (double) (order.amount * order.price);
        // std::cout << "OrderBook::getAmount volume: " << order.amount << std::endl;
    }
    
    return volume;
}

std::vector<Candlestick> OrderBook::computeCandlestickData(std::vector<std::string>& uniqueTimestamps, 
                                                OrderBook& orderBook, 
                                                OrderBookType type,
                                                std::string product) {
    std::vector<Candlestick> candlesticks;
    double prevClose = orderBook.getOrders(type, product, uniqueTimestamps[0])[0].price;
    double high, low, open, close;
    for (const std::string& timestamp : uniqueTimestamps) {
        std::vector<OrderBookEntry> sliceOfEntries = orderBook.getOrders(type, product, timestamp);
        high = orderBook.getHighPrice(sliceOfEntries);   
        low = orderBook.getLowPrice(sliceOfEntries);
        close = getMeanPrice(sliceOfEntries);
        open = prevClose;
        Candlestick candlestick{timestamp, open, high, low, close};
        candlesticks.push_back(candlestick);
        prevClose = close;
    }
    return candlesticks;
}

std::vector<Bar> OrderBook::computeBarData(std::vector<std::string>& uniqueTimestamps, 
                                                        OrderBook& orderBook, 
                                                        OrderBookType type,
                                                        std::string product) { 
    std::vector<Bar> bars;
    double amount;
    for (const std::string& timestamp : uniqueTimestamps){
        std::vector<OrderBookEntry> sliceOfEntries = orderBook.getOrders(type, product, timestamp);
        amount = orderBook.getVolume(sliceOfEntries);
        Bar bar{timestamp, amount};
        bars.push_back(bar);
    }
    for (const Bar& bar : bars){
        std::cout << bar.getAmount() << ", time: " << bar.getTime() << std::endl;
    }
    return bars;
}

// =========== end of my code ============

void OrderBook::insertOrder(OrderBookEntry& order){
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp){
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);
    std::vector<OrderBookEntry> sales;

    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    for(OrderBookEntry& ask : asks){
        for(OrderBookEntry& bid : bids){
            if(bid.price >= ask.price){
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

                if (bid.username == "simuser"){
                    sale.username = "simuser";
                    sale.orderBookType = OrderBookType::bidsale;;
                } else if (ask.username == "simuser"){          
                    sale.username = "simuser";
                    sale.orderBookType = OrderBookType::asksale;
                }
            
                if (bid.amount == ask.amount){
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                } else if (bid.amount > ask.amount){
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = bid.amount - ask.amount;
                    break;
                } else if (bid.amount < ask.amount && bid.amount > 0){
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0;
                    continue;
                }
            }

        }
    }
    return sales;
}

