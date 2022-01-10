#include "factory.hpp"
#include <map>
#include <ostream>

enum class NodeColor { UNVISITED, VISITED, VERIFIED };

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors){
    if(node_colors[sender] == NodeColor::VERIFIED){
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;
    if((*sender).receiver_preferences_.cend() == (*sender).receiver_preferences_.cbegin()){
        throw std::logic_error("Error");
    }

    bool is_storehouse_reachable = true;
    bool sender_has_receiver = false;
    for(auto ptr = (*sender).receiver_preferences_.cbegin(); ptr != (*sender).receiver_preferences_.cend(); ptr++){
        if((*ptr).first->get_receiver_type() == ReceiverType::STOREHOUSE){
            sender_has_receiver = true;
        } else if((*ptr).first->get_receiver_type() == ReceiverType::WORKER){
            IPackageReceiver* receiver_ptr = (*ptr).first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if(sendrecv_ptr == sender){
                continue;
            }
            sender_has_receiver = true;
            if(node_colors[sendrecv_ptr] == NodeColor::UNVISITED){
                is_storehouse_reachable = has_reachable_storehouse(sendrecv_ptr, node_colors);
            }

        }
    }
    node_colors[sender] = NodeColor::VERIFIED;
    if(sender_has_receiver && is_storehouse_reachable){
        return true;
    } else{

        throw std::logic_error("Error");
    }
}

bool Factory::is_consistent() {
    std::map<const PackageSender *, NodeColor> color;
    for (auto ptr = workers_.begin(); ptr != workers_.end(); ptr++){
        Worker* worker = &(*ptr);
        auto sender = dynamic_cast<PackageSender*>(worker);
        color[sender] = NodeColor::UNVISITED;
    }
    for (auto ptr = ramps_.begin(); ptr != ramps_.end(); ptr++){
        Ramp* ramp = &(*ptr);
        auto sender = dynamic_cast<PackageSender*>(ramp);
        color[sender] = NodeColor::UNVISITED;
    }

    try {
        for (auto ptr = ramps_.begin(); ptr != ramps_.end(); ptr++){
            Ramp* ramp = &(*ptr);
            auto sender = dynamic_cast<PackageSender*>(ramp);
            has_reachable_storehouse(sender, color);
        }
    }
    catch (const std::logic_error &) {
        return false;
    }


    return true;
}

ParsedLineData parse_line(std::string line){
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    ParsedLineData parsed;
    if(tokens[0] == "LOADING_RAMP"){
        parsed.element_type = ElementType::RAMP;
    }else if(tokens[0] == "WORKER"){
        parsed.element_type = ElementType::WORKER;
    }else if(tokens[0] == "STOREHOUSE"){
        parsed.element_type = ElementType::STOREHOUSE;
    }else if(tokens[0] == "LINK"){
        parsed.element_type = ElementType::LINK;
    }else {
        throw std::logic_error("Error");
    }
    tokens.erase(tokens.begin());
    for(auto x: tokens){
        std::stringstream stringstream(x);
        std::string key_val[2];
        int i = 0;
        while (std::getline(stringstream, token, '=')) {
            key_val[i] = token;
            i++;
        }
        parsed.parameters[key_val[0]] = key_val[1];
    }

    return parsed;
}

Factory load_factory_structure(std::istream &is) {
    Factory factory;
    std::string line;

    while (std::getline(is, line)) {
        if(!line.empty() && line[0] != ';'){
            ParsedLineData data = parse_line(line);
            if(data.element_type == ElementType::RAMP){
                factory.add_ramp(Ramp(std::stoi(data.parameters["id"]), std::stoi(data.parameters["delivery-interval"])));
            }else if(data.element_type == ElementType::WORKER){
                if(data.parameters["queue-type"] == "FIFO"){
                    factory.add_worker(Worker(std::stoi(data.parameters["id"]), std::stoi(data.parameters["processing-time"]), std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
                } else if(data.parameters["queue-type"] == "LIFO"){
                    factory.add_worker(Worker(std::stoi(data.parameters["id"]), std::stoi(data.parameters["processing-time"]), std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
                }
            }else if(data.element_type == ElementType::STOREHOUSE){
                factory.add_storehouse(Storehouse(std::stoi(data.parameters["id"])));
            }else if(data.element_type == ElementType::LINK){
                std::stringstream stringstream_src(data.parameters["src"]);
                std::stringstream stringstream_dest(data.parameters["dest"]);
                std::string key_val_src[2];
                std::string key_val_dest[2];
                std::string token;
                int i = 0;
                while (std::getline(stringstream_src, token, '-')) {
                    key_val_src[i] = token;
                    i++;
                }

                i = 0;
                while (std::getline(stringstream_dest, token, '-')) {
                    key_val_dest[i] = token;
                    i++;
                }

                if(key_val_src[0]=="ramp" && key_val_dest[0]=="worker"){
                    Ramp& r = *(factory.find_ramp_by_id(std::stoi(key_val_src[1])));
                    r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(std::stoi(key_val_dest[1]))));
                }
                else if(key_val_src[0]=="worker" && key_val_dest[0]=="store"){
                    Worker& w = *(factory.find_worker_by_id(std::stoi(key_val_src[1])));
                    w.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(std::stoi(key_val_dest[1]))));
                }
                else if(key_val_src[0]=="worker" && key_val_dest[0]=="worker"){
                    Worker& w = *(factory.find_worker_by_id(std::stoi(key_val_src[1])));
                    w.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(std::stoi(key_val_dest[1]))));
                }
                else if(key_val_src[0]=="ramp" && key_val_dest[0]=="store"){
                    Ramp& r = *(factory.find_ramp_by_id(std::stoi(key_val_src[1])));
                    r.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(std::stoi(key_val_dest[1]))));
                }


            }

        }
    }
    return factory;
}

void save_factory_structure(Factory& factory, std::ostream& os){
    for (auto ptr = factory.ramp_cbegin(); ptr != factory.ramp_cend(); ptr++){
        os<<"LOADING_RAMP id="+std::to_string((*ptr).get_id())+" delivery-interval="+std::to_string((*ptr).get_delivery_interval())+"\n";
    }
    for (auto ptr = factory.worker_cbegin(); ptr != factory.worker_cend(); ptr++){
        if((*ptr).get_queue()->get_queue_type() == PackageQueueType::LIFO){
            os<<"WORKER id="+std::to_string((*ptr).get_id())+" processing-time="+std::to_string((*ptr).get_processing_duration())+" queue-type=LIFO"+"\n";
        }
        if((*ptr).get_queue()->get_queue_type() == PackageQueueType::FIFO){
            os<<"WORKER id="+std::to_string((*ptr).get_id())+" processing-time="+std::to_string((*ptr).get_processing_duration())+" queue-type=FIFO"+"\n";
        }
    }
    for (auto ptr = factory.storehouse_cbegin(); ptr != factory.storehouse_cend(); ptr++){
        os<<"STOREHOUSE id="+std::to_string((*ptr).get_id())+"\n";
    }

    for (auto ptr = factory.ramp_cbegin(); ptr != factory.ramp_cend(); ptr++){
        for (auto ptr_2 = (*ptr).receiver_preferences_.cbegin(); ptr_2 != (*ptr).receiver_preferences_.cend(); ptr_2++){
            if((*ptr_2).first->get_receiver_type() == ReceiverType::WORKER){
                os<<"LINK src=ramp-"+std::to_string((*ptr).get_id())+" dest=worker-"+std::to_string((*ptr_2).first->get_id())+"\n";
            }

        }

    }
    for (auto ptr = factory.worker_cbegin(); ptr != factory.worker_cend(); ptr++){
        for (auto ptr_2 = (*ptr).receiver_preferences_.cbegin(); ptr_2 != (*ptr).receiver_preferences_.cend(); ptr_2++){
            if((*ptr_2).first->get_receiver_type() == ReceiverType::STOREHOUSE){
                os<<"LINK src=worker-"+std::to_string((*ptr).get_id())+" dest=store-"+std::to_string((*ptr_2).first->get_id())+"\n";
            }
            if((*ptr_2).first->get_receiver_type() == ReceiverType::WORKER){
                os<<"LINK src=worker-"+std::to_string((*ptr).get_id())+" dest=worker-"+std::to_string((*ptr_2).first->get_id())+"\n";
            }

        }
    }


    os.flush();
}