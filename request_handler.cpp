#include "request_handler.h"

optional<bus_stat> RequestHandler::get_bus_stat(const string_view& bus_name) const
{
	bus_stat result = db_.get_buses(bus_name);

	if (result.check) return result;

	else return nullopt;
}

optional<bus_ptr> RequestHandler::get_buses_by_stop(const string_view& stop_name) const
{
	bus_ptr result = db_.get_stops(stop_name);

	if (result.check) return result;

	else return nullopt;
}