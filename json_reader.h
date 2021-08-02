#pragma once

#include "json.h"
#include "request_handler.h"

#include <memory>

using namespace json;
using namespace requesthandler;

namespace jsonreader
{
    class JsonReader final : public TransportCatalogue
    {
    public:
        explicit JsonReader() = default;
        ~JsonReader() = default;

        void parse_requests(istream&) override;
        void print(const RequestHandler&, ostream&);

    private:
        unique_ptr<Node> nd;
        void push_requests() override;
    };
}
/*
#pragma once

#include "domain.h"
#include "request_handler.h"
#include "json.h"
//#include "map_renderer.h"

namespace transport_catalogue {

    namespace json_reader {

        class JsonReader {
        public:
            void ReadJson(std::istream& is);
            void Print(std::ostream& os) const;
            void InitializeTransportCatalogue(TransportCatalogue& transport_catalogue);
            std::vector<transport_catalogue::request_handler::detail::Request> GetRequests();
            void WriteResponse(std::vector<std::variant<std::nullptr_t, transport_catalogue::request_handler::detail::StopResponse, transport_catalogue::request_handler::detail::BusResponse, transport_catalogue::request_handler::detail::ErrorResponse>> answer);
            //void SetMapSettings(transport_catalogue::map_renderer::MapRenderer& map);

        private:
            void ReadBaseRequests(const json::Node& base_req);
            void ReadStatRequests(const json::Node& stat_req);
            //void ReadRenderSettings(const json::Node& render_settings);

            detail::BusCommand GetBusCommandFromNode(const json::Node& node) const;
            detail::StopCommand GetStopCommandFromNode(const json::Node& node) const;
            request_handler::detail::Request GetRequestFromNode(const json::Node& node) const;

            void InsertBusResponse(request_handler::detail::BusResponse bus, std::map<std::string, json::Node>& target) const;
            void InsertStopResponse(request_handler::detail::StopResponse stop, std::map<std::string, json::Node>& target) const;
            void InsertErrorResponse(request_handler::detail::ErrorResponse error, std::map<std::string, json::Node>& target) const;

            //svg::Color GetColorFromNode(const json::Node& color_node);

            std::pair<std::vector<transport_catalogue::detail::StopCommand>, std::vector<transport_catalogue::detail::BusCommand>> base_requests;
            std::vector<transport_catalogue::request_handler::detail::Request> requests;
            std::vector<std::variant<std::nullptr_t, transport_catalogue::request_handler::detail::StopResponse, transport_catalogue::request_handler::detail::BusResponse, transport_catalogue::request_handler::detail::ErrorResponse>> response;
            //transport_catalogue::map_renderer::MapSettings map_settings;
        };

    }

}*/