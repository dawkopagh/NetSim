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

        virtual const_iterator cbegin()   const = 0;
        virtual const_iterator cend()     const = 0;
        virtual const_iterator begin()    const = 0;
        virtual const_iterator end()      const = 0;

        virtual void push(const Package&& package) = 0;
        virtual bool empty() const = 0;
        virtual std::size_t size() const = 0;
        virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile {
    public:
        virtual Package pop() = 0;
        virtual PackageQueueType get_queue_type() const = 0;
        virtual ~IPackageQueue() = default;

};

class PackageQueue : public IPackageQueue{
    public:
        PackageQueue(PackageQueueType packageQueueType) : queueType_(packageQueueType) {}

        const_iterator cbegin()   const override {return queue_.cbegin();} ;
        const_iterator cend()     const override {return queue_.cend();} ;
        const_iterator begin()    const override {return queue_.begin();} ;
        const_iterator end()      const override {return queue_.end();} ;

        void push(const Package&& package) override {queue_.emplace_back(std::move(package)); };
        bool empty() const override {return queue_.size(); };
        std::size_t size() const override {return queue_.size(); };

        PackageQueueType get_queue_type() const override {return queueType_; };
        Package pop() override;

        ~PackageQueue() = default;
    private:
        std::list<Package> queue_;
        PackageQueueType queueType_;
};
#endif //NETSIM_STORAGE_TYPES_HPP
