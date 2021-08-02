#pragma once

#include "transport_catalogue.h"

#include <optional>

using namespace transportcatalogue;

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)

namespace requesthandler
{
    class RequestHandler
    {
    public:
        // MapRenderer понадобится в следующей части итогового проекта
        RequestHandler(const TransportCatalogue& db/*, const renderer::MapRenderer& renderer*/)
            : db_(db) {
        }

        // Возвращает информацию о маршруте (запрос Bus)
        optional<BusStat> get_bus_stat(const string_view& bus_name) const;

        // Возвращает маршруты, проходящие через
        const set<BusPtr>* get_buses_by_stop(const string_view& stop_name) const;

        // Этот метод будет нужен в следующей части итогового проекта
        //svg::Document RenderMap() const;

    private:
        // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
        const TransportCatalogue& db_;
        //const renderer::MapRenderer& renderer_;
    };
}
/*
#pragma once

#include "transport_catalogue.h"

#include <vector>
#include <variant>
#include <string>

namespace transport_catalogue {

    namespace request_handler {

        namespace detail {

            enum class QueryType {
                StopQuery,
                BusQuery
            };

            struct BusResponse {
                size_t request_id;
                double curvature;
                double route_length;
                size_t stop_count;
                size_t unique_stop_count;
                bool exists;
            };

            struct StopResponse {
                size_t request_id;
                std::set<std::string> buses;
                bool exists;
            };

            struct ErrorResponse {
                size_t request_id;
                std::string error_message;
            };

            struct Request {
                size_t id;
                QueryType type;
                std::string name;
            };

        }



        class RequestHandler {
        public:
            void InitializeRequestHandler(const std::vector<detail::Request>& requests_);
            void HandleRequests(const transport_catalogue::TransportCatalogue& transport_catalogue_);
            std::vector<std::variant<std::nullptr_t, detail::StopResponse, detail::BusResponse, detail::ErrorResponse>> GetResponses();
            size_t GetRequestCount() const;

        private:

            std::vector<detail::Request> requests;
            std::vector<std::variant<std::nullptr_t, detail::StopResponse, detail::BusResponse, detail::ErrorResponse>> responses;

        };


    }

}
*/