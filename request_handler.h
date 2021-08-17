#pragma once

#include "map_renderer.h"

#include <optional>

using namespace renderer;

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)

namespace handler {
    class RequestHandler {
    public:
        // MapRenderer понадобится в следующей части итогового проекта
        RequestHandler(const TransportCatalogue& db, const MapRenderer& renderer)
            : db_(db)
            , renderer_(renderer) {
        }

        // Возвращает информацию о маршруте (запрос Bus)
        optional<BusStat> get_bus_stat(const string_view& bus_name) const;

        // Возвращает маршруты, проходящие через
        const set<BusPtr>* get_buses_by_stop(const string_view& stop_name) const;

        // Этот метод будет нужен в следующей части итогового проекта
        svg::Document render_map() const;

    private:
        // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
        const TransportCatalogue& db_;
        const MapRenderer& renderer_;
    };
}