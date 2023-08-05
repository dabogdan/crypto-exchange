// =========== beginning of my code ============

#include <iostream>
#include <vector>
#include <iomanip>

#include "CandleChart.h"
#include "Candlestick.h"
#include "CSVReader.h"

CandleChart::CandleChart(std::string _name) : name(_name) {};

void CandleChart::drawChart(const std::vector<Candlestick>& candlesticks, const std::vector<std::string>& uniqueTimestamps){
    
    //figure out the chart ticks
    double lowest = candlesticks[0].getLow();
    double highest = candlesticks[0].getHigh();
    for (const Candlestick& candle : candlesticks){
        if (lowest > candle.getLow()) {
            lowest = candle.getLow();
        }
        if (highest < candle.getHigh()) {
            highest = candle.getHigh();
        };
    }

    double diff = highest - lowest;
    int stepAmount = 20;
    double tick = diff / (double)stepAmount;


    //drawing the chart
    std::cout << name << std::endl;
    
    std::string green = "\033[32m";
    std::string reset = "\033[0m";
    std::string red = "\033[31m";
    for (int i = stepAmount; i >= 0; i--){
        double step = lowest + tick * i;
        double prevStep = lowest + tick * (i + 1);
        double nextStep;
        i == 0 ? nextStep = 0 : nextStep = lowest + tick * (i - 1);
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        std::cout << step;
        std::cout << "\t";
        for (int j = 0; j < 15; ++j) {
            std::string color = candlesticks[j].getOpen() < candlesticks[j].getClose() ? green : red;            
            std::cout << color << candlesticks[j].plot(step, prevStep) << reset;
        }
        std::cout << "" << std::endl;    
    }
    
    //drawing the bottom of the chart
    std::cout << "         \t";

    for (std::string time : uniqueTimestamps){
        std::vector<std::string> tokens = CSVReader::tokenise(time, ' ');
        tokens = CSVReader::tokenise(tokens[1], '.');
        tokens = CSVReader::tokenise(tokens[0], ':');
        std::cout << tokens[0] << ":" << tokens[1] << ":" << tokens[2];
        std::cout << " ";
    }
    std::cout << "" << std::endl;
};

// =========== end of my code ============
