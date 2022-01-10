#ifndef NET_SIMULATION_REPORTS_HPP
#define NET_SIMULATION_REPORTS_HPP

#include "factory.hpp"


void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

class IntervalReportNotifier{
private:
    TimeOffset to_;
public:
    IntervalReportNotifier(TimeOffset to){to_=to;}
    bool should_generate_report(Time t);
};

class SpecificTurnsReportNotifier{
private:
    std::set<Time> turns_;
public:
    SpecificTurnsReportNotifier(std::set<Time> turns){turns_=turns;}
    bool should_generate_report(Time t);
};

#endif
