// =========== beginning of my code ============


#include "Bar.h"

Bar::Bar(std::string _time, double _amount) : time(_time), amount(_amount) {};

std::string Bar::plot(double step) const {
    if (step < amount) {
        return "   ---   ";
    }
    return "         ";
}

// =========== end of my code ============