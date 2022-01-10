#include "nodes.hpp"


void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    preferences_[r] = 1;
    size_t size = preferences_.size();
    for (auto & [key, val] : preferences_)
    {
        val = 1/(double)size;
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    preferences_.erase (r);
    size_t size = preferences_.size();
    for (auto & [key, val] : preferences_)
    {
        val = 1/(double)size;
    }
}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    double rand = pg_();
    double distr = 0;
    for (auto & [key, val] : preferences_)
    {
        distr = distr + val;
        if(rand <= distr){
            return key;
        }
    }
    return nullptr;
}


void PackageSender::send_package() {
    if(buffer_sender_){
        auto receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(*buffer_sender_));
        buffer_sender_.reset();
    }

}

void PackageSender::push_package(Package && p) {
    buffer_sender_.emplace(std::move(p));
}

void Worker::do_work(Time t) {

        if(!(*q_).empty() && (!buffer_)){
            buffer_.emplace((*q_).pop());
            start_time_ = t;
        }
        if(buffer_){
            if(t - start_time_ >= pd_-1){
                buffer_sender_.swap(buffer_);
                buffer_.reset();
            }
        }
}
