#ifndef NET_SIMULATION_NODES_HPP
#define NET_SIMULATION_NODES_HPP

#include "package.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"
#include "config.hpp"
#include <map>
#include <optional>
#include <memory>
#include <iostream>
#include <utility>

enum class ReceiverType {
    WORKER,
    STOREHOUSE
};

class IPackageReceiver{
public:
    virtual void receive_package(Package&& p) = 0;
    [[nodiscard]] virtual ElementID get_id() const = 0;
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    [[nodiscard]] virtual ReceiverType get_receiver_type() const = 0;
    #endif
};

class ReceiverPreferences{
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;
private:
    preferences_t preferences_;
    ProbabilityGenerator pg_;
public:
    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator) {pg_ = std::move(pg);}
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    [[nodiscard]] const preferences_t & get_preferences() const {return preferences_;}
    [[nodiscard]] const_iterator cbegin() const {return preferences_.cbegin();}
    [[nodiscard]] const_iterator cend() const {return preferences_.cend();}
    iterator begin(){return preferences_.begin();}
    iterator end(){return preferences_.end();}
};

class PackageSender{
public:
    PackageSender(){buffer_sender_ = std::nullopt;}
    ReceiverPreferences receiver_preferences_;
    PackageSender(PackageSender&&)=default;
    void send_package();
    [[nodiscard]] const std::optional<Package>& get_sending_buffer() const {return buffer_sender_;}
protected:
    void push_package(Package&& p);
    std::optional<Package> buffer_sender_;
};

class Ramp: public PackageSender{
private:
    ElementID id_;
    TimeOffset di_;
public:
    Ramp(ElementID id, TimeOffset di) {id_ = id; di_ = di;}
    void deliver_goods(Time t) {
        if((t-1)%di_ == 0){
            buffer_sender_ = Package();
        }
    }
    [[nodiscard]] TimeOffset get_delivery_interval() const {return di_;}
    [[nodiscard]] ElementID get_id() const {return id_;}
};

class Worker: public IPackageReceiver, public PackageSender{
private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    Time start_time_{};
    std::optional<Package> buffer_;
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q){id_ = id; pd_ = pd; q_ = std::move(q); buffer_ = std::nullopt;}
    void do_work(Time t);
    [[nodiscard]] ElementID get_id() const override {return id_;}
    [[nodiscard]] TimeOffset get_processing_duration() const {return pd_;}
    [[nodiscard]] Time get_package_processing_start_time() const {return start_time_;}
    void receive_package(Package&& p) override {(*q_).push(std::move(p));}
    [[nodiscard]] ReceiverType get_receiver_type() const override {return ReceiverType::WORKER;}
    [[nodiscard]] IPackageQueue * get_queue() const {return q_.get();}
    [[nodiscard]] const std::optional<Package>& get_processing_buffer() const {return buffer_;}
    [[nodiscard]] IPackageStockpile::const_iterator cbegin() const {return q_->cbegin();}
    [[nodiscard]] IPackageStockpile::const_iterator cend() const {return q_->cend();}
};

class Storehouse: public IPackageReceiver{
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d){id_ = id; d_ = std::move(d);}
    explicit Storehouse(ElementID id){
        id_ = id;
        std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::LIFO);
        d_ = std::move(d);
    }
    [[nodiscard]] ElementID get_id() const override {return id_;}
    void receive_package(Package&& p) override {(*d_).push(std::move(p));}
    [[nodiscard]] ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;}
    [[nodiscard]] IPackageStockpile* get_stockpile() const {return d_.get();}
    [[nodiscard]] IPackageStockpile::const_iterator cbegin() const {return d_->cbegin();}
    [[nodiscard]] IPackageStockpile::const_iterator cend() const {return d_->cend();}
};

#endif
