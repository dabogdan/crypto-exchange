// =========== beginning of my code ============

#include <iostream>
#include <vector>
#include "Bar.h"

class BarChart {
    private:
        std::string name;
    public: 
        BarChart(std::string _name);
        void drawChart(const std::vector<Bar>& bars, const std::vector<std::string>& uniqueTimestamps) const;
};



// =========== end of my code ============