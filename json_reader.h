#pragma once

#include "json.h"
#include "request_handler.h"

#include <memory>

using namespace json;
using namespace handler;

namespace reader {
    class JsonReader final {
    public:
        JsonReader() = default;
        ~JsonReader() = default;
        void parse(istream&);
        void print(const RequestHandler&, ostream&);

    private:
        unique_ptr<Node> nd;
        void push_requests();
        void push_settings();
    };
}