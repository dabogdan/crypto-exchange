// =========== beginning of my code ============


#pragma once
#include <string>
#include <vector>

class Candlestick {
    private:
        std::string date;
        double open;
        double high;
        double low;
        double close;

    public:
        Candlestick(std::string _date, double _open, double _high, double _low, double _close);
        std::string plot(double& step, double& prevStep) const;

        std::string getDate() const { return date; }
        double getOpen() const { return open; }
        double getHigh() const { return high; }
        double getLow() const { return low; }
        double getClose() const { return close; }
};

// =========== end of my code ============
