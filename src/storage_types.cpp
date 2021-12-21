//
// Created by dkope on 18.12.2021.
//

#include "storage_types.hpp"
#include "package.hpp"

// @TODO: Zwalnianie indeksu
Package PackageQueue::pop() {
    switch (queueType_) {
        case PackageQueueType::FIFO: {
            Package returned_package = queue_.front();
            queue_.pop_front();
            return returned_package;
        }
        case PackageQueueType::LIFO: {
            Package returned_package = queue_.back();
            queue_.pop_back();
            return returned_package;
        }
    }
}