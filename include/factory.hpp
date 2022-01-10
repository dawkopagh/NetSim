#ifndef NET_SIMULATION_FACTORY_HPP
#define NET_SIMULATION_FACTORY_HPP

#include <list>
#include <istream>
#include <string>
#include <sstream>
#include "nodes.hpp"

enum class ElementType {
    RAMP,
    WORKER,
    STOREHOUSE,
    LINK
};

struct ParsedLineData{
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};


template <class Node>
class NodeCollection{


public:
    using list = typename std::list<Node>;
    using iterator = typename list::iterator;
    using const_iterator = typename list::const_iterator;


    void add(Node&& node) {list_.push_back(std::move(node));}
    void remove_by_id(ElementID id) {
        auto it = find_by_id(id);
        if (it != list_.end()) {
            list_.erase(it);
        }}
    NodeCollection<Node>::iterator find_by_id(ElementID id) {return std::find_if(list_.begin(), list_.end(), [=](Node& node) {return node.get_id() == id;} );}
    [[nodiscard]] const_iterator cbegin() const {return list_.cbegin();}
    [[nodiscard]] const_iterator cend() const {return list_.cend();}
    iterator begin() {return list_.begin();}
    iterator end() {return list_.end();}
private:
    list list_;
};


class Factory{
private:
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;
public:
    void add_ramp(Ramp&& ramp) {ramps_.add(std::move(ramp));}
    void remove_ramp(ElementID id) {ramps_.remove_by_id(id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramps_.cbegin();}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cend() const {return ramps_.cend();}

    void add_worker(Worker&& worker) {workers_.add(std::move(worker));}
    void remove_worker(ElementID id) {
        IPackageReceiver* worker = &(*workers_.find_by_id(id));
        for (auto & ptr : workers_){
            ptr.receiver_preferences_.remove_receiver(worker);
        }
        for (auto & ramp : ramps_){
            ramp.receiver_preferences_.remove_receiver(worker);
        }
        workers_.remove_by_id(id);
    }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers_.cbegin();}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cend() const {return workers_.cend();}

    void add_storehouse(Storehouse&& storehouse) {storehouses_.add(std::move(storehouse));}
    void remove_storehouse(ElementID id) {
        IPackageReceiver* storehouse = &(*storehouses_.find_by_id(id));
        for (auto & worker : workers_){
            worker.receiver_preferences_.remove_receiver(storehouse);
        }
        storehouses_.remove_by_id(id);
    }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return storehouses_.cbegin();}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return storehouses_.cend();}


    bool is_consistent();
    void do_deliveries(Time t) {
        for (auto & ramp : ramps_){
            ramp.deliver_goods(t);
        }}
    void do_package_passing() {
        for (auto & worker : workers_){
            worker.send_package();
        }
        for (auto & ramp : ramps_){
            ramp.send_package();
        }
    }
    void do_work(Time t) {
        for (auto & worker : workers_){
            worker.do_work(t);
        }
    }


};

Factory load_factory_structure(std::istream &is);

void save_factory_structure(Factory& factory, std::ostream& os);



#endif
