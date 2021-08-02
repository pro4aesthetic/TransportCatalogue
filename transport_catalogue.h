#pragma once
#pragma warning(disable: 4244)
#pragma warning(disable: 4715)

#include "domain.h"

#include <utility>
#include <deque>
#include <cstdlib>
#include <memory>
#include <set>

using namespace detail;

namespace transportcatalogue
{
    class TransportCatalogue
    {
    public:
        explicit TransportCatalogue() = default;
        virtual ~TransportCatalogue() = default;
        virtual void parse_requests(istream&) = 0;

        [[nodiscard]] BusStat get_bus(const string_view&) const;
        [[nodiscard]] const set<BusPtr>* get_stop(const string_view&) const noexcept;

    protected:
        deque<Buses> save_bus;
        deque<Stops> save_stop;

        unordered_map<string_view, const Buses*> u_bus_catalogue;
        unordered_map<string_view, const Stops*> u_stop_catalogue;

    private:
        set<BusPtr> bus;

        virtual void push_requests() = 0;
        const Buses* find_buses(const string_view& name) const { return u_bus_catalogue.at(name); }
        const Stops* find_stops(const string_view& name) const { return u_stop_catalogue.at(name); }
    };
}
