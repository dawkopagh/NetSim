//
// Created by dkope on 15.12.2021.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <vector>
#include <ostream>
#include <list>
#include <set>

#include "types.hpp"

class Package{
    public:
        Package();
        Package(ElementID elementId);
        Package(const Package&& other) : elementID_(other.get_id()) {}
        Package(const Package& other) : elementID_(other.get_id()) {}


        Package& operator= (Package&& other);

        ElementID get_id() const {return elementID_; };

        ~Package();

    private:
    inline static std::set<ElementID> assignedIDs_ = {0};
    inline static std::set<ElementID> freedIDs_= {};
    ElementID elementID_;
};

#endif //NETSIM_PACKAGE_HPP
