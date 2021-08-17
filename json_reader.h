#pragma once

#include "json.h"
#include "request_handler.h"

#include <memory>
#include <sstream>

using namespace json;
using namespace handler;

namespace reader {
    class JsonReader final {
    public:
        JsonReader() = default;
        JsonReader(MapRenderer& renderer_)
            : renderer(renderer_) {
        }
        ~JsonReader() = default;

        void parse(istream&);
        void print(const RequestHandler&, ostream&);

    private:
        MapRenderer& renderer;
        unique_ptr<Node> nd;
        void push_requests();
        void push_rendsettings();
    };
}