#include "simulation.hpp"

void simulate(Factory &f, TimeOffset d, std::function<void (Factory&, Time)> rf) {
    if(!f.is_consistent()){throw std::logic_error("Not consistent");}
    int t = 1;
    while(d >= t){
        f.do_deliveries(t);
        f.do_package_passing();
        f.do_work(t);
        rf(f, t);

        t++;
    }
}
