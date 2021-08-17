#define _CRT_SECURE_NO_WARNINGS

#include "json_reader.h"

#include <fstream>

using namespace reader;

// паттерн проектирования абстрактная фабрика https://ru.wikipedia.org/wiki/Абстрактная_фабрика_(шаблон_проектирования)
// так же используется паттерн проектирования одиночка https://ru.wikipedia.org/wiki/Одиночка_(шаблон_проектирования)
class TransportFactory;

class TransportFactoryDestroyer {
public:
    ~TransportFactoryDestroyer() {
        delete _instance;
    }
    void initialize(TransportFactory* p) {
        _instance = p;
    }

private:
    TransportFactory* _instance = nullptr;
};

class TransportFactory {
public:
    static TransportFactory* instance();
    MapRenderer* make_catalogue() const {
        return new MapRenderer;
    }

protected:
    TransportFactory() = default;
    ~TransportFactory() = default;
    TransportFactory(const TransportFactory&) = delete;
    TransportFactory& operator=(const TransportFactory&) = delete;
    friend class TransportFactoryDestroyer;

private:
    static TransportFactory* _instance;
    static TransportFactoryDestroyer destroyer;
};

TransportFactory* TransportFactory::_instance = 0;
TransportFactoryDestroyer TransportFactory::destroyer;

TransportFactory* TransportFactory::instance() {
    if (_instance == 0) {
        _instance = new TransportFactory;
        destroyer.initialize(_instance);
    }
    return _instance;
}

void run_system(istream& input, ostream& output) {
    TransportFactory* factory = TransportFactory::instance();
    MapRenderer* renderer = factory->make_catalogue();
    
    JsonReader object(*renderer);
    object.parse(input);

    RequestHandler requests(*renderer, *renderer);
    object.print(requests, output);
}

int main() {
    LOG_DURATION("program speed..."s);
    
    //ifstream input("big_test_with_map.json", ios_base::in);
    //ofstream output("big_test_with_map_result_2.json", ios_base::out);
    ifstream input("123.json", ios_base::in);
    ofstream output("321_2.json", ios_base::out);
    run_system(input, output);

    input.close();
    output.close();
    
    return 0;
}
