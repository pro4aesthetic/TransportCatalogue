#pragma once

#include "transport_catalogue.h"

using save_inf = vector<map<string, tmp_type>>;

void handler(TransportCatalogue&, const save_inf&);
inline result_bs answer1(TransportCatalogue& transport, const map<string, tmp_type>& stat_requests)
{
	return transport.get_inf_buses(stat_requests);
}

inline result_st answer2(TransportCatalogue& transport, const map<string, tmp_type>& stat_requests)
{
	return transport.get_inf_stops(stat_requests);
}

//result_bs* answer1(TransportCatalogue&, const map<string, tmp_type>&);
//result_st* answer2(TransportCatalogue&, const map<string, tmp_type>&);