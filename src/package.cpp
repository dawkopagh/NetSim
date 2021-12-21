//
// Created by dkope on 15.12.2021.
//

#include "package.hpp"
#include "types.hpp"
#include <stdexcept>
#include <algorithm>

Package::Package(){
    if(freedIDs_.empty()){
        elementID_ = *assignedIDs_.rbegin() + 1;
    } else {
        elementID_ = *freedIDs_.begin();
        freedIDs_.erase(freedIDs_.begin());
    }
    assignedIDs_.insert(elementID_);
}

Package& Package::operator=(Package&& other) {
    elementID_ = other.get_id();
    return *this;
}

Package::Package(ElementID elementId){
    if(std::find(assignedIDs_.cbegin(), assignedIDs_.cend(), elementId) == assignedIDs_.cend()){
        throw std::invalid_argument("ID already assigned, invalid argument.");
    } else{
        elementID_ = elementId;
        assignedIDs_.insert(elementId);
    }

}

Package::~Package() {
    freedIDs_.insert(elementID_);
    assignedIDs_.erase(elementID_);
}