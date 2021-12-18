//
// Created by dkope on 18.12.2021.
//

#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <vector>
#include <ostream>
#include <list>

#include "package.hpp"

enum PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:
        using const_iterator = std::list<Package>::const_iterator;

//        virtual std::vector<???>::const_iterator cbegin() const = 0;
//        virtual std::vector<???>::const_iterator cend() const = 0;
//        virtual std::vector<???>::const_iterator begin() const = 0;
//        virtual std::vector<???>::const_iterator end() const = 0;

        virtual void push() const = 0;
        virtual bool empty() const = 0;
        virtual std::size_t size() const = 0;
        virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile {
    public:
        virtual Package pop() = 0;
        virtual PackageQueueType get_queue_type() = 0;
        virtual ~IPackageQueue() = default;

};

class PackageQueue : public IPackageQueue{
    public:
        PackageQueue(PackageQueueType packageQueueType) : queueType_(packageQueueType) {}

        void push(const Package& package) {queue_.push_back(package); };
        bool empty() {return queue_.size(); };
        std::size_t size() {return queue_.size(); };

        Package pop() override;
        PackageQueueType get_queue_type() override {return queueType_; };

        ~PackageQueue() = default;
    private:
        std::list<Package> queue_;
        PackageQueueType queueType_;
};
#endif //NETSIM_STORAGE_TYPES_HPP
