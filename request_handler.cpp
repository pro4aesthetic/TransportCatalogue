#include "request_handler.h"

void handler(TransportCatalogue& transport, const save_inf& base_requests)
{
	for (const auto& map_ : base_requests)
	{
		if ("Stop"s == get<string>(map_.at("type"s))) //!!!!!!!!!!!!!!!!!!!!!!!
			transport.set_stops_in(map_);

		else if ("Bus"s == get<string>(map_.at("type"s)))
			transport.set_route_in(map_);

		else continue;
	}
}
/*
result_bs answer1(TransportCatalogue& transport, const map<string, tmp_type>& stat_requests)
{
	return transport.get_inf_buses(stat_requests);
}

result_st answer2(TransportCatalogue& transport, const map<string, tmp_type>& stat_requests)
{
	return transport.get_inf_stops(stat_requests);
}*/
/*
result_bs* answer1(TransportCatalogue& transport, const map<string, tmp_type>& stat_requests)
{
	return transport.get_inf_buses(stat_requests);
}

result_st* answer2(TransportCatalogue& transport, const map<string, tmp_type>& stat_requests)
{
	return transport.get_inf_stops(stat_requests);
}*/