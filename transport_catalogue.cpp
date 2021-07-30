#include "transport_catalogue.h"
#include <iostream>

BusStat TransportCatalogue::get_bus(const string_view& bus_name) const
{
    if (u_bus_catalogue.count(bus_name))
    {
        auto bus = find_buses(bus_name);

        int length = 0;
        double com_distance = 0.0, curvature = 0.0;

        if (bus->get_stop_count())
        {
            auto vec = bus->get_stops();
            int count = 1;

            for (auto it = begin(vec); it < end(vec) - 1; ++it, ++count)
            {
                if (end(u_stop_catalogue) != u_stop_catalogue.find(*it) && end(u_stop_catalogue) != u_stop_catalogue.find(*(it + 1)))
                {
                    com_distance += compute_distance(find_stops(*it)->coordinates, find_stops(*(it + 1))->coordinates);

                    if (!find_stops(*it)->check_road_distances(*(it + 1)) && !find_stops(*(it + 1))->check_road_distances(*it))
                        continue;

                    else if (!find_stops(*it)->check_road_distances(*(it + 1)) || find_stops(*it)->check())
                        length += find_stops(*(it + 1))->get_distance(*it);

                    else length += find_stops(*it)->get_distance(*(it + 1));
                }
            }

            curvature = length / com_distance;
        }

        return BusStat
        {
            curvature,
            length,
            bus->get_stop_count(),
            bus->get_unique_stop_count(),
            true
        };
    }

    else return BusStat{};
}

const unordered_set<BusPtr, BusPtrHash>* TransportCatalogue::get_stop(const string_view& stop_name) const noexcept
{
    auto bus_ptr = make_unique<unordered_set<BusPtr, BusPtrHash>>(bus);
    if (u_stop_catalogue.count(stop_name))
    {
        for (const auto& [key, value] : u_bus_catalogue)
            if (value->find_stop(stop_name))
                bus_ptr->emplace(BusPtr{ value->name, true });
    }    

    return bus_ptr.get();
}