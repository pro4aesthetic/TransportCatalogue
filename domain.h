#pragma once

#include "geo.h"

#include <string_view>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace geo;

namespace detail
{
    struct stops
    {
        stops() = default;
        stops(string_view type_,
            string_view name_,
            Coordinates coo_,
            unordered_map<string_view, int> road_distances_)
            : type(type_),
            name(name_),
            coo(coo_),
            road_distances(move(road_distances_)) {
        }

    private:
        string_view type = "none"sv;
        string_view name = "none"sv;
        Coordinates coo;
        unordered_map<string_view, int> road_distances;

    public:
        string_view get_name() const
        {
            return name;
        }

        int get_distance(string_view str) const
        {
            return road_distances.at(str);
        }

        Coordinates get_coordinates() const
        {
            return coo;
        }

        bool check_road_distances(string_view str) const
        {
            return road_distances.count(str);
        }

        bool check() const 
        {
            return road_distances.empty();
        }
    };

    struct buses
    {
        buses() = default;
        buses(string_view type_,
            string_view name_,
            vector<string_view> stops_,
            bool is_roundtrip_)
            : type(type_),
            name(name_),
            stops(move(stops_)),
            is_roundtrip(is_roundtrip_) {
        }

    private:
        string_view type = "none"sv;
        string_view name = "none"sv;
        vector<string_view> stops;
        bool is_roundtrip = true;

    public:
        bool find_stop(string_view) const;

        bool check_is_roundtrip() const
        {
            return is_roundtrip;
        }

        string_view get_name() const
        {
            return name;
        }

        int get_unique_stop_count() const
        {
            int counter = 0;

            for (const auto& v : stops)
            {
                int r = count(begin(stops), end(stops), v);
                if (r == 1)  ++counter;
            }
            int cnt = stops.size() - counter;
            int result = cnt / 2;

            return counter + result;
        }

        int get_stop_count() const;

        vector<string_view> get_stops() const;
    };
}

