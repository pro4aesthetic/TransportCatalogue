#pragma once
#pragma warning(disable: 4244)

#include "domain.h"

#include <utility>
#include <deque>
#include <cstdlib>
#include <unordered_set>
#include <memory>

using namespace detail;

struct BusPtrHash
{
    size_t operator()(const BusPtr& bus_ptr) const
    {
        size_t b_h = b_hasher(bus_ptr.check);
        size_t sv_h = sv_hasher(bus_ptr.name);

        return b_h * 10000000 + sv_h * 10000000000;
    }

private:
    hash<bool> b_hasher;
    hash<string_view> sv_hasher;
};

class TransportCatalogue
{
public:
    explicit TransportCatalogue() = default;

    virtual ~TransportCatalogue() = default;
    virtual void parse_requests(istream&) = 0;

    [[nodiscard]] BusStat get_bus(const string_view&) const;
    [[nodiscard]] const unordered_set<BusPtr, BusPtrHash>* get_stop(const string_view&) const noexcept;

protected:
    deque<Buses> save_bus;
    deque<Stops> save_stop;

    unordered_map<string_view, const Buses*> u_bus_catalogue;
    unordered_map<string_view, const Stops*> u_stop_catalogue;

private:
    unordered_set<BusPtr, BusPtrHash> bus;
    
    virtual void push_requests() = 0;
    const Buses* find_buses(const string_view& name) const { return u_bus_catalogue.at(name); }
    const Stops* find_stops(const string_view& name) const { return u_stop_catalogue.at(name); }
};