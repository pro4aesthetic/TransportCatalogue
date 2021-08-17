#include "transport_catalogue.h"

namespace transportcatalogue {
    void TransportCatalogue::push_bus(const Bus& bus) {
        save_bus.push_back(bus);
        u_bus_catalogue[bus.name] = &save_bus.back();
    }

    void TransportCatalogue::push_stop(const Stop& stop) {
        save_stop.push_back(stop);
        u_stop_catalogue[stop.name] = &save_stop.back();
    }

    BusStat TransportCatalogue::get_bus(const string_view& bus_name) const {
        if (u_bus_catalogue.count(bus_name) && u_stop_catalogue.size()) {
            auto bus = find_buses(bus_name);
            double com_distance = 0.0, curvature = 0.0, road_distances = 0.0;

            if (bus->check()) {
                auto vec = bus->get_stops();
                int count = 1;

                for (auto it = begin(vec); it < end(vec) - 1; ++it, ++count) {
                    com_distance += compute_distance(find_stops(*it)->coordinates, find_stops(*(it + 1))->coordinates);
                    if (find_stops(*it)->check_road_distances(*(it + 1))) {
                        road_distances += find_stops(*it)->get_distance(*(it + 1));
                    }
                    else { 
                        road_distances += find_stops(*(it + 1))->get_distance(*it); 
                    }
                }
            }
            curvature = road_distances / com_distance;

            return BusStat {
                curvature,
                road_distances,
                bus->get_stop_count(),
                bus->get_unique_stop_count(),
                true
            };
        }
        else {
            return BusStat{};
        }
    }

    const set<BusPtr>* TransportCatalogue::get_stop(const string_view& stop_name) const noexcept {
        auto bus_ptr = make_unique<set<BusPtr>>(bus);
        if (u_stop_catalogue.count(stop_name) && u_bus_catalogue.size()) {
            for (const auto& [key, value] : u_bus_catalogue) {
                if (value->find_stop(stop_name)) {
                    bus_ptr->emplace(BusPtr{ value->name });
                }
                else { 
                    bus_ptr->emplace(BusPtr{});
                }
            }
        }
        else if (u_stop_catalogue.count(stop_name)) {
            bus_ptr->emplace(BusPtr{});
        }
        return bus_ptr.release();
    }
}
