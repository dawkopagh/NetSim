#ifndef NET_SIMULATION_STORAGE_TYPES_HPP
#define NET_SIMULATION_STORAGE_TYPES_HPP

#include <list>

enum class PackageQueueType {
    FIFO, LIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package&& package)=0;
    virtual size_t size()=0;
    virtual bool empty()=0;
    virtual const_iterator cbegin()=0;
    virtual const_iterator cend()=0;
    virtual ~IPackageStockpile() = default;

};

class IPackageQueue : public IPackageStockpile {
public:
    virtual PackageQueueType get_queue_type()=0;
    virtual Package pop()=0;
};


class PackageQueue : public IPackageQueue {
private:
    std::list<Package> product_list_;
    PackageQueueType type_;
public:
    PackageQueue(PackageQueueType type){
        type_ = type;
    }
    Package pop();
    PackageQueueType get_queue_type(){return type_;};
    void push(Package&& package){
        product_list_.push_back(std::move(package));
    };
    size_t size(){return product_list_.size();}
    bool empty(){return product_list_.empty();}
    const_iterator cbegin(){return product_list_.cbegin();}
    const_iterator cend(){return product_list_.cend();}
};


#endif
