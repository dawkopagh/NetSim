//
// Created by dkope on 15.12.2021.
//

#include "package.hpp"

Package::Package(){
    if(freed_ids_.empty()){
        elementID_ = assigned_ids_.back() + 1;
    } else {
        elementID_ = freed_ids_.front();
        freed_ids_.pop_front();
    }
    assigned_ids_.push_back(elementID_);
}

Package &Package::operator=(Package&& other) {
    elementID_ = other.get_id();
    return *this;
}

Package::Package(ElementID elementId){

}