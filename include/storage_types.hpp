//
// Created by Wiktor on 19.12.2021.
//

#ifndef NET_SIMULATION_STORAGE_TYPES_HPP
#define NET_SIMULATION_STORAGE_TYPES_HPP



#include <list>
#include <iostream>



enum class PackageQueueType{
    FIFO, LIFO
};

class IPackageStockpile
{
public:
    using const_iterator = std::list<Package>::const_iterator; //alias
    virtual void push(Package package) = 0; //methods of virtual
    virtual bool empty() const = 0;
    virtual std::size_type size() const = 0;
    virtual ~IPackageStockpile() = default; //virtual desctructor
    virtual const_iterator cbegin() const = 0; //ready-only
    virtual const_iterator cend() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
};

class IPackageQueue: public IPackageStockpile
{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
    virtual ~IPackageQueue() = default;
};

class PackageQueue: public IPackageQueue
{
public:
    PackageQueue(PackageQueueType type) : type_(type) {}
    void push(Package package) override;
    bool empty() const override { return PackageList_.empty(); }
    std::size_t size() const override { return PackageList_.size(); }
    Package pop() override;
    PackageQueueType get_queue_type() override {return type_;}
    const_iterator cbegin() const override { return PackageList_.cbegin(); }
    const_iterator cend() const override { return PackageList_.cend(); }
    const_iterator begin() const override { return PackageList_.cbegin(); }
    const_iterator end() const override{ return PackageList_.cend(); }

private:
    std::list <Package> PackageList_;
    PackageQueueType type_;
};

#endif
