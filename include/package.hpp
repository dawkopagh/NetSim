//
// Created by dkope on 15.12.2021.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <vector>
#include <ostream>
#include <list>

#include "types.hpp"

class Package{
    public:
        Package() = default;
        Package(ElementID elementId) : elementID_(elementId) {}
        Package(Package&) = default;
        Package& operator= (const Package&) = default;
        ElementID get_id() const {return elementID_; };
        ~Package() = default;
    private:
    ElementID elementID_;
};

#endif //NETSIM_PACKAGE_HPP
