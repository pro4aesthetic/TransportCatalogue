#pragma once

#include "domain.h"

#include <utility>
#include <deque>
#include <cstdlib>
#include <variant>

using namespace detail;

using tmp_type = variant<nullptr_t, string, int, double, bool, vector<string>, map<string, int>>;

struct result_bs
{
    double curvature;
    int request_id;
    int route_length;
    int stop_count;
    int unique_stop_count;
    bool check;
};

struct result_st
{
    vector<string> buses;
    int request_id;
    bool check;
};

class TransportCatalogue final
{
public:
    inline void set_route_in(const map<string, tmp_type>& map_)
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

        save_bus.push_back(buses{ type, name, stops, is_roundtrip1 });
        u_bus_catalogue[name] = &save_bus.back();
    }

    inline void set_stops_in(const map<string, tmp_type>& map_)
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

            else continue;
        }

        save_stop.push_back(stops{ type, name, Coordinates{ lat1, lng1 }, road_distances });
        u_stop_catalogue[name] = &save_stop.back();
    }

    [[nodiscard]] result_bs get_inf_buses(const map<string, tmp_type>&);
    [[nodiscard]] result_st get_inf_stops(const map<string, tmp_type>&);

private:
    deque<buses> save_bus;
    deque<stops> save_stop;

    map<string_view, const buses*> u_bus_catalogue;
    map<string_view, const stops*> u_stop_catalogue;

    const buses* find_buses(string_view str)
    {
        return u_bus_catalogue.at(str);
    }
    const stops* find_stops(string_view str)
    {
        return u_stop_catalogue.at(str);
    }
};
/*
#pragma once

#include "domain.h"

#include <utility>
#include <deque>
#include <cstdlib>
#include <variant>

using namespace detail;

using tmp_type = variant<nullptr_t, string, int, double, bool, vector<string>, map<string, int>>;

struct result_bs
{
    double curvature;
    int request_id;
    int route_length;
    int stop_count;
    int unique_stop_count;
    bool check;
};

struct result_st
{
    vector<string> buses;
    int request_id;
    bool check;
};

class TransportCatalogue final
{
public:
    void set_route_in(const map<string, tmp_type>&);
    void set_stops_in(const map<string, tmp_type>&);

    [[nodiscard]] result_bs* get_inf_buses(const map<string, tmp_type>&);
    [[nodiscard]] result_st* get_inf_stops(const map<string, tmp_type>&);

private:
    deque<buses> save_bus;
    deque<stops> save_stop;

    result_st r;
    result_bs t;

    map<string_view, const buses*> u_bus_catalogue;
    map<string_view, const stops*> u_stop_catalogue;

    const buses* find_buses(string_view str)
    {
        return u_bus_catalogue.at(str);
    }
    const stops* find_stops(string_view str)
    {
        return u_stop_catalogue.at(str);
    }
};
*/