#pragma once

#include "geo.h"
#include "log_duration.h"

#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace geo;

namespace detail {
    struct Stop {
        Stop() = default;
        ~Stop() = default;
        
        string_view name;
        Coordinates coordinates {0, 0};
        unordered_map<string_view, int> road_distances;

        int get_distance(const string_view& name) const { 
            return road_distances.at(name); 
        }
        bool check_road_distances(const string_view& name) const { 
            return road_distances.count(name); 
        }
    };

    struct Bus {
        Bus() = default;
        ~Bus() = default;
        
        string_view name;
        vector<string_view> stops;
        bool is_roundtrip = false;
    
        bool find_stop(const string_view&) const;
        int get_unique_stop_count() const;
        vector<string_view> get_stops() const;

        int get_stop_count() const { 
            return get_stops().size(); 
        }
        bool check() const { 
            return !get_stops().empty(); 
        }
    };

    struct BusStat {
        double curvature = 0;
        double route_distances = 0;
        int stop_count = 0;
        int unique_stop_count = 0;
        bool check = false;
    };

    struct BusPtr {
        string_view name;

        bool operator==(const BusPtr& lhs) const noexcept { 
            return this->name == lhs.name; 
        }
        bool operator<(const BusPtr& lhs) const noexcept { 
            return this->name < lhs.name; 
        }
    };
}
