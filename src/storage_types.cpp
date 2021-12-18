//
// Created by dkope on 18.12.2021.
//

#include "storage_types.hpp"
#include "package.hpp"

// @TODO: Zwalnianie indeksu
Package PackageQueue::pop() {
    Package returned_package;
    switch (queueType_) {
        case PackageQueueType::FIFO:
            returned_package = queue_.front();
            queue_.pop_front();
            return returned_package;
        case PackageQueueType::LIFO:
            Package returned_package = queue_.back();
            queue_.pop_back();
            break;
    }
}