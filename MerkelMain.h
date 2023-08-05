#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "CandleChart.h"
#include "BarChart.h"

class MerkelMain {
    public:
        MerkelMain();
        void init();
    
    private:
        void printMenu();
        int getUserInput();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void goToNextTimeFrame();
        void drawExchangeData();
        void drawVolume();
        void processUserInput (int userInput);

        std::string currentTime;

        OrderBook orderBook{"20200601.csv"};

        Wallet wallet;
};