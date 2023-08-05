// =========== beginning of my code ============

#include <iostream>
#include <vector>
#include <iomanip>

#include "BarChart.h"
// #include "Bar.h"
#include "CSVReader.h"

BarChart::BarChart(std::string _name) : name(_name) {};

void BarChart::drawChart(const std::vector<Bar>& bars, const std::vector<std::string>& uniqueTimestamps) const {
    
    //figure out the chart ticks
    double lowest = bars[0].getAmount();
    double highest = bars[0].getAmount();

    for (const Bar& bar : bars){
        if (lowest > bar.getAmount()) {
            lowest = bar.getAmount();
        }
        if (highest < bar.getAmount()) {
            highest = bar.getAmount();
        };
    }

    double diff = highest - lowest;
    int stepAmount = 20;
    double tick = diff / (double)stepAmount;


    //drawing the chart
    std::cout << name << std::endl;
    
    std::string green = "\033[32m";
    std::string reset = "\033[0m";
    

    for (int i = stepAmount; i >= 0; i--){
        double step = lowest + tick * i;
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        std::cout << step;
        std::cout << "\t";
        for (int j = 0; j < 15; ++j) {
            std::cout << green << bars[j].plot(step) << reset;
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
