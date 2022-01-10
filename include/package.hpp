#ifndef NET_SIMULATION_PACKAGE_HPP
#define NET_SIMULATION_PACKAGE_HPP

#include <set>
#include <types.hpp>

class Package{
private:
    inline static std::set<ElementID> assigned_IDs;
    inline static std::set<ElementID> freed_IDs;
    ElementID id_;
public:
    Package();
    Package(ElementID id);
    Package(Package&&)=default;
    Package& operator=(Package&&)=default;
    ~Package();

    ElementID get_id() const {return id_;};

};


#endif
