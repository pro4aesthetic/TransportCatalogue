#define _CRT_SECURE_NO_WARNINGS

#include "json_reader.h"
#include "request_handler.h"
#include "map_renderer.h"

#include <fstream>
#include <cstdlib> // для ф-ии getenv

using namespace reader;
using namespace handler;

// паттерн проектирования абстрактная фабрика https://ru.wikipedia.org/wiki/Абстрактная_фабрика_(шаблон_проектирования)
// так же используется паттерн проектирования одиночка https://ru.wikipedia.org/wiki/Одиночка_(шаблон_проектирования)
class TransportCatalogueFactory;

class TransportCatalogueFactoryDestroyer {
public:
    ~TransportCatalogueFactoryDestroyer() {
        delete _instance;
    }
    void initialize(TransportCatalogueFactory* p) {
        _instance = p;
    }

private:
    TransportCatalogueFactory* _instance;
};

class TransportCatalogueFactory {
public:
    static TransportCatalogueFactory& instance();
    virtual TransportCatalogue* make_renderer_catalogue() {
        return new TransportCatalogue;
    }

protected:
    TransportCatalogueFactory() = default;
    TransportCatalogueFactory(const TransportCatalogueFactory&) = delete;
    TransportCatalogueFactory& operator=(const TransportCatalogueFactory&) = delete;
    ~TransportCatalogueFactory() = default;
    friend class TransportCatalogueFactoryDestroyer;

private:
    static TransportCatalogueFactory* _instance;
    static TransportCatalogueFactoryDestroyer destroyer;
};

class MapTransportCatalogueFactory : public TransportCatalogueFactory {
public:
    MapTransportCatalogueFactory() = default;
    TransportCatalogue* make_renderer_catalogue() {
        return new MapRenderer;
    }
};

class Create {
public:
    TransportCatalogue* create(TransportCatalogueFactory& factory);
};

TransportCatalogueFactory* TransportCatalogueFactory::_instance = 0;
TransportCatalogueFactoryDestroyer TransportCatalogueFactory::destroyer;

TransportCatalogueFactory& TransportCatalogueFactory::instance() {
    if (_instance == 0) {
        const char* transport_catalogue_style = getenv("TRANSPORTCATALOGUESTYLE");

        if (strcmp(transport_catalogue_style, "map") == 0) {
            _instance = new MapTransportCatalogueFactory;
            destroyer.initialize(_instance);
        }
        else {
            _instance = new TransportCatalogueFactory;
            destroyer.initialize(_instance);
        }
    }
    return *_instance;
}

TransportCatalogue* Create::create(TransportCatalogueFactory& factory) {
    TransportCatalogue* catalogue = factory.make_renderer_catalogue();
    return catalogue;
}

void run_system(istream& input, ostream& output) {
    Create run;
    TransportCatalogueFactory& factory = TransportCatalogueFactory::instance();
    TransportCatalogue* db = run.create(factory);
    
    JsonReader object;
    object.parse(input);

    //RequestHandler requests(*db, dynamic_cast<MapRenderer&>(*db));
    //object.print(requests, output);
}

int main() {
    LOG_DURATION("program speed..."s);
    
    //ifstream input("big_test_with_map.json", ios_base::in);
    //ofstream output("big_test_with_map_result_2.json", ios_base::out);
    ifstream input("render_test.json", ios_base::in);
    ofstream output("map_test.svg", ios_base::out);

    run_system(input, output);

    input.close();
    output.close();
    
    return 0;
}
