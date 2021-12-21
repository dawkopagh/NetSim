//
// Created by Wiktor on 19.12.2021.
//

#ifndef NET_SIMULATION_PACKAGE_HPP
#define NET_SIMULATION_PACKAGE_HPP
#include "types.hpp"

#include <set>

class Package{
public:
    Package();
    Package(ElementID id) : _id(id) {};
    Package(Package&& package): _id(std::move(package)._id) {};

    Package& operator=(Package&&) = default;

    ElementID get_id() const { return _id; }

    ~Package();

private:
    ElementID _id;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif

