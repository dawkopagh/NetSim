//
// Created by dkope on 15.12.2021.
//

#include "package.hpp"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package(){
    if (not freed_IDs.empty()){
        _id = *freed_IDs.begin();
        assigned_IDs.insert(_id);
        freed_IDs.erase(freed_IDs.begin());
    }
    else if (not assigned_IDs.empty()){
        _id = *(assigned_IDs.end()--) + 1;
        assigned_IDs.insert(_id);
    }
    else{
        _id = 1;
        assigned_IDs.insert(_id);
    }
}

Package::~Package()
{
    freed_IDs.insert(_id);
    assigned_IDs.erase(_id);
}