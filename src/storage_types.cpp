#include <list>
#include <package.hpp>
#include "storage_types.hpp"

Package PackageQueue::pop() {
    switch (type_) {
        case PackageQueueType::FIFO: {
            Package first_elem(std::move(product_list_.front()));
            product_list_.pop_front();
            return first_elem;
        }
        case PackageQueueType::LIFO: {
            Package last_elem(std::move(product_list_.back()));
            product_list_.pop_back();
            return last_elem;
        }
    }
    Package p;
    return p;
}

