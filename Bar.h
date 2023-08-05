// =========== beginning of my code ============

#pragma once
#include <iostream>

class Bar {
    private:
        std::string time;
        double amount;
    public:
        Bar(std::string time, double amount);
        double getAmount() const { return amount; }
        std::string getTime() const { return time; }
        
        std::string plot(double step) const;
};

// =========== end of my code ============