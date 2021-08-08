#include "request_handler.h"

namespace handler
{
    optional<BusStat> RequestHandler::get_bus_stat(const string_view& bus_name) const
    {
        if (db_.get_bus(bus_name).check) return db_.get_bus(bus_name);
        else return nullopt;
    }

    const set<BusPtr>* RequestHandler::get_buses_by_stop(const string_view& stop_name) const
    {
        if (!db_.get_stop(stop_name)->empty()) return db_.get_stop(stop_name);
        else return nullptr;
    }

    svg::Document RequestHandler::RenderMap() const
    {
        return svg::Document{};
    }
}
