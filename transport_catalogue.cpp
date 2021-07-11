#include "transport_catalogue.h"
#include <iostream>
/*
inline void TransportCatalogue::set_route_in(const map<string, tmp_type>& map_)
{
    string_view type, name;
    vector<string> stops;
    bool is_roundtrip1 = true;

    for (const auto& [key, value] : map_)
    {
        if (key == "type"s && holds_alternative<string>(value))
            type = get<string>(value);

        else if (key == "name"s && holds_alternative<string>(value))
            name = get<string>(value);

        else if (key == "stops"s && holds_alternative<vector<string>>(value))
            stops = get<vector<string>>(value);

        else if (key == "is_roundtrip"s && holds_alternative<bool>(value))
            is_roundtrip1 = get<bool>(value);

        else continue;
    }

    save_bus.push_back(buses{type, name, stops, is_roundtrip1});
    u_bus_catalogue[name] = &save_bus.back();
}

inline void TransportCatalogue::set_stops_in(const map<string, tmp_type>& map_)
{
    string_view type, name;
    double lat1 = 0.0, lng1 = 0.0;
    map<string, int> road_distances;

    for (const auto& [key, value] : map_)
    {
        if (key == "type"s && holds_alternative<string>(value))
            type = get<string>(value);

        else if (key == "name"s && holds_alternative<string>(value))
            name = get<string>(value);

        else if (key == "latitude"s)
        {
            if (holds_alternative<int>(value))
                lat1 = double(abs(get<int>(value)));

            else if (holds_alternative<double>(value))
                lat1 = abs(get<double>(value));
        }

        else if (key == "longitude"s)
        {
            if (holds_alternative<int>(value))
                lng1 = double(abs(get<int>(value)));

            else if (holds_alternative<double>(value))
                lng1 = abs(get<double>(value));
        }

        else if (key == "road_distances"s && holds_alternative<map<string, int>>(value))
            road_distances = get<map<string, int>>(value);

        else break;
    }

    save_stop.push_back(stops{ type, name, Coordinates{ lat1, lng1 }, road_distances });
    u_stop_catalogue[name] = &save_stop.back();
}*/

result_bs TransportCatalogue::get_inf_buses(const map<string, tmp_type>& map_)
{
    if (u_bus_catalogue.count(get<string>(map_.at("name"s))))
    {
        auto bus = find_buses(get<string>(map_.at("name"s)));

        int length = 0;
        double com_distance = 0.0, curvature = 0.0;

        if (bus->get_unique_stop_count())
        {
            auto vec = bus->get_stops();
            int count = 1;

            for (auto it = begin(vec); it < end(vec) - 1; ++it, ++count)
            {
                if (find_stops(*it)->check_road_distances() || (count == vec.size() - 1 && !bus->check_is_roundtrip()))
                {
                    com_distance += compute_distance(find_stops(*(it + 1))->get_coordinates(), find_stops(*it)->get_coordinates());
                    length += find_stops(*(it + 1))->get_distance(*it);
                }

                else
                {
                    com_distance += compute_distance(find_stops(*it)->get_coordinates(), find_stops(*(it + 1))->get_coordinates());
                    length += find_stops(*it)->get_distance(*(it + 1));
                }
            }

            curvature = length / com_distance;
        }

        return result_bs
        {
            curvature,
            get<int>(map_.at("id"s)),
            length,
            bus->get_stop_count(),
            bus->get_unique_stop_count(),
            true
        };
    }

    else
    {
        return result_bs
        {
            0.0,
            get<int>(map_.at("id"s)),
            0,
            0,
            0,
            false
        };
    }
}

result_st TransportCatalogue::get_inf_stops(const map<string, tmp_type>& map_)
{
    if (u_stop_catalogue.count(get<string>(map_.at("name"s))))
    {
        vector<string> bus;
        for (auto& [key, value] : u_bus_catalogue)
        {
            if (value->find_stop(get<string>(map_.at("name"s))))
                bus.push_back(value->get_name());
        }

        return result_st
        {
            bus,
            get<int>(map_.at("id"s)),
            true
        };
    }

    else
    {
        return result_st
        {
            {},
            get<int>(map_.at("id"s)),
            false
        };
    }
}
/*

#include "transport_catalogue.h"

void TransportCatalogue::set_route_in(const map<string, tmp_type>& map_)
{
    string_view type, name;
    vector<string> stops;
    bool is_roundtrip1 = true;

    for (const auto& [key, value] : map_)
    {
        if (key == "type"s && holds_alternative<string>(value))
            type = get<string>(value);

        else if (key == "name"s && holds_alternative<string>(value))
            name = get<string>(value);

        else if (key == "stops"s && holds_alternative<vector<string>>(value))
            stops = get<vector<string>>(value);

        else if (key == "is_roundtrip"s && holds_alternative<bool>(value))
            is_roundtrip1 = get<bool>(value);
    }

    save_bus.push_back(buses{ type, name, stops, is_roundtrip1 });
    u_bus_catalogue[name] = &save_bus.back();
}

void TransportCatalogue::set_stops_in(const map<string, tmp_type>& map_)
{
    string_view type, name;
    double lat1 = 0.0, lng1 = 0.0;
    map<string, int> road_distances;

    for (const auto& [key, value] : map_)
    {
        if (key == "type"s && holds_alternative<string>(value))
            type = get<string>(value);

        else if (key == "name"s && holds_alternative<string>(value))
            name = get<string>(value);

        else if (key == "latitude"s)
        {
            if (holds_alternative<int>(value))
                lat1 = double(abs(get<int>(value)));

            else if (holds_alternative<double>(value))
                lat1 = abs(get<double>(value));
        }

        else if (key == "longitude"s)
        {
            if (holds_alternative<int>(value))
                lng1 = double(abs(get<int>(value)));

            else if (holds_alternative<double>(value))
                lng1 = abs(get<double>(value));
        }

        else if (key == "road_distances"s && holds_alternative<map<string, int>>(value))
            road_distances = get<map<string, int>>(value);
    }

    save_stop.push_back(stops{ type, name, Coordinates{ lat1, lng1 }, road_distances });
    u_stop_catalogue[name] = &save_stop.back();
}

result_bs* TransportCatalogue::get_inf_buses(const map<string, tmp_type>& map_)
{
    if (u_bus_catalogue.count(get<string>(map_.at("name"s))))
    {
        auto bus = find_buses(get<string>(map_.at("name"s)));

        int length = 0;
        double com_distance = 0.0, curvature = 0.0;

        if (bus->get_unique_stop_count())
        {
            auto vec = bus->get_stops();

            for (auto it = vec.begin(); it < vec.end() - 1; ++it)
            {
                com_distance += compute_distance(find_stops(*it)->get_coordinates(), find_stops(*(it + 1))->get_coordinates());
                if (find_stops(*it)->check_road_distances() && find_stops(*(it + 1))->check_road_distances())
                    continue;

                else if (find_stops(*it)->check_road_distances() && !find_stops(*(it + 1))->check_road_distances())
                    length += find_stops(*(it + 1))->get_distance(*it);

                else length += find_stops(*it)->get_distance(*(it + 1));
            }

            //if (com_distance != 0)
                curvature = length / com_distance;
        }

        t = 
        {
            curvature,
            get<int>(map_.at("id"s)),
            length,
            bus->get_stop_count(),
            bus->get_unique_stop_count(),
            true
        };

        return &t;
    }

    else
    {
        t =
        {
            0.0,
            get<int>(map_.at("id"s)),
            0,
            0,
            0,
            false
        };

        return &t;
    }
}

result_st* TransportCatalogue::get_inf_stops(const map<string, tmp_type>& map_)
{
    if (u_stop_catalogue.count(get<string>(map_.at("name"s))))
    {
        vector<string> bus;
        for (auto& [key, value] : u_bus_catalogue)
        {
            if (value->find_stop(get<string>(map_.at("name"s))))
                bus.push_back(value->get_name());
        }

        r =
        {
            bus,
            get<int>(map_.at("id"s)),
            true
        };

        return &r;
    }

    else
    {
        r =
        {
            {},
            get<int>(map_.at("id"s)),
            false
        };

        return &r;
    }
}
*/