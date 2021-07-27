#pragma once

#include "domain.h"

#include <utility>
#include <deque>
#include <cstdlib>
#include <unordered_set>

using namespace detail;

struct bus_stat
{
    double curvature = 0.0;
    int route_length = 0;
    int stop_count = 0;
    int unique_stop_count = 0;
    bool check = false;
};

struct bus_ptr
{
    unordered_set<string_view> buses;
    bool check = false;
};

class TransportCatalogue
{
public:
    TransportCatalogue() = default;
    virtual ~TransportCatalogue() = default;

    [[nodiscard]] bus_stat get_buses(const string_view&) const;
    [[nodiscard]] bus_ptr get_stops(const string_view&) const;

protected:
    deque<buses> save_bus;
    deque<stops> save_stop;

    unordered_map<string_view, const buses*> u_bus_catalogue;
    unordered_map<string_view, const stops*> u_stop_catalogue;

private:
    const buses* find_buses(const string_view& str) const
    {
        return u_bus_catalogue.at(str);
    }
    const stops* find_stops(const string_view& str) const
    {
        return u_stop_catalogue.at(str);
    }
};