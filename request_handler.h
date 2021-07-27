﻿#pragma once

#include "transport_catalogue.h"
#include <optional>

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)

class RequestHandler 
{
public:
    // MapRenderer понадобится в следующей части итогового проекта
    RequestHandler(const TransportCatalogue& db/*, const renderer::MapRenderer& renderer*/)
        : db_(db) {
    }

    // Возвращает информацию о маршруте (запрос Bus)
    optional<bus_stat> get_bus_stat(const string_view& bus_name) const;

    // Возвращает маршруты, проходящие через
    optional<bus_ptr> get_buses_by_stop(const string_view& stop_name) const;

    // Этот метод будет нужен в следующей части итогового проекта
    //svg::Document RenderMap() const;

private:
    // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
    const TransportCatalogue& db_;
    //const renderer::MapRenderer& renderer_;
};
