#ifndef NET_SIMULATION_SIMULATION_HPP
#define NET_SIMULATION_SIMULATION_HPP

#include "factory.hpp"
#include "types.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf);


#endif
