#include "package.hpp"

Package::Package() {
    if(! freed_IDs.empty()){
        id_ = *freed_IDs.begin();
        freed_IDs.erase(freed_IDs.begin());
        assigned_IDs.insert(id_);
    }
    else{
        int max = 0;
        for (auto elem : assigned_IDs)
        {
            if(elem > max){
                max = elem;
            }
        }
        id_ = max + 1;
        assigned_IDs.insert(id_);
    }

}

Package::~Package() {
    assigned_IDs.erase(id_);
    freed_IDs.insert(id_);

}

Package::Package(ElementID id) {
    id_ = id;
    assigned_IDs.insert(id_);
}
