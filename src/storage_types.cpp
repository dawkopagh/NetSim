//
// Created by Wiktor on 19.12.2021.
//

#include "package.hpp"
#include "storage_types.hpp"
#include <utility>
void PackageQueue::push(Package package)
{
    PackageList_.emplace_back(std::move(package));
}

Package PackageQueue::pop()
{
    switch (type_)
    {
        case PackageQueueType::FIFO:
        {
            Package package(PackageList_.begin()->get_id());
            PackageList_.pop_front();
            return package;
        }
        case PackageQueueType::LIFO:
        {
            Package package((PackageList_.end()--)->get_id());
            PackageList_.pop_back();
            return package;
        }
        default:
        {
            throw std::invalid_argument( "received negative value" );
        }
    }
}
