// =========== beginning of my code ============


#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

#include "Candlestick.h"
#include "OrderBook.h"

Candlestick::Candlestick(std::string _date, double _open, double _high, double _low, double _close)
    : date(_date), open(_open), high(_high), low(_low), close(_close) {}

std::string Candlestick::plot(double& step, double& prevStep) const {
    if (open < close){
        //green
        if ((step >= open && step <= close) || (prevStep >= open && step <= open)) return "   +-+   ";
        if ((step < high && step > close) || (step > low && step < open)) return "    |    ";
    } else if (open > close) {
        //red
        if ((step <= open && step >= close) || (prevStep >= open && step <= open)) return "   +-+   ";
        if ((step > open && step < high) || (step > low && step < close)) return "    |    ";
    }
    return "         ";
}

// =========== end of my code ============