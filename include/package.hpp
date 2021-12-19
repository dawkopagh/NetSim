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
        Package();
        Package(ElementID elementId);
        Package(const Package&& other) : elementID_(other.get_id()) {}
        Package(const Package& other) : elementID_(other.get_id()) {}


        Package& operator= (Package&& other);

        ElementID get_id() const {return elementID_; };

        ~Package() = default;

    private:
    inline static std::list<ElementID> assigned_ids_ = {0};
    inline static std::list<ElementID> freed_ids_ = {};
    ElementID elementID_;
};

#endif //NETSIM_PACKAGE_HPP
