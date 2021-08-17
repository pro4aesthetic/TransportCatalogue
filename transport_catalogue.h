#pragma once
#pragma warning(disable: 4244)
#pragma warning(disable: 4715)
#pragma warning(disable: 4150)

#include "domain.h"

#include <cstdlib>
#include <memory>
#include <deque>
#include <set>

using namespace detail;

namespace transportcatalogue {
    class TransportCatalogue {
    public:        
        void push_bus(const Bus&);
        void push_stop(const Stop&);
        [[nodiscard]] BusStat get_bus(const string_view&) const;
        [[nodiscard]] const set<BusPtr>* get_stop(const string_view&) const noexcept;

    protected:
        TransportCatalogue() = default;
        ~TransportCatalogue() = default;
        unordered_map<string_view, const Bus*> u_bus_catalogue;
        unordered_map<string_view, const Stop*> u_stop_catalogue;
        const Bus* find_buses(const string_view& name) const {
            return u_bus_catalogue.at(name);
        }
        const Stop* find_stops(const string_view& name) const {
            return u_stop_catalogue.at(name);
        }

    private:
        deque<Bus> save_bus;
        deque<Stop> save_stop;
        set<BusPtr> bus = {};
    };
}
