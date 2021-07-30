#pragma once

#include "geo.h"

#include <string_view>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <tuple>

using namespace std;
using namespace geo;

namespace detail
{
    struct Stops
    {
        Stops() = default;
        ~Stops() = default;
        
        string_view name = "none"sv;
        Coordinates coordinates {0.0, 0.0};
        unordered_map<string_view, int> road_distances;

    public:
        int get_distance(const string_view& name) const { return road_distances.at(name); }
        bool check_road_distances(const string_view& name) const { return road_distances.count(name); }
        bool check() const { return road_distances.empty(); }
    };

    struct Buses
    {
        Buses() = default;
        ~Buses() = default;
        
        string_view name = "none"sv;
        vector<string_view> stops;
        bool is_roundtrip = true;

    public:
        bool find_stop(const string_view&) const;
        int get_unique_stop_count() const;
        int get_stop_count() const { return get_stops().size(); }
        vector<string_view> get_stops() const;
    };

    struct BusStat
    {
        double curvature = 0.0;
        int route_length = 0;
        int stop_count = 0;
        int unique_stop_count = 0;
        bool check = false;
    };

    struct BusPtr
    {
        string_view name = "none"sv;
        bool check = false;

        bool operator==(const BusPtr& lhs) const noexcept
        {
            return tuple(lhs.check, lhs.name) == tuple(this->check, this->name);
        }
    };
}

