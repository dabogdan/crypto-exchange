// =========== beginning of my code ============

#include <iostream>
#include <vector>
#include "Candlestick.h"

class CandleChart {
    private:
        std::string name;
    public: 
        CandleChart(std::string _name);
        void drawChart(const std::vector<Candlestick>& candlesticks, const std::vector<std::string>& uniqueTimestamps);
};

// =========== end of my code ============