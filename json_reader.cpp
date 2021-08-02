#include "json_reader.h"

namespace jsonreader
{
    void JsonReader::parse_requests(istream& input)
    {
        Document doc = Load(input);
        nd = make_unique<Node>(doc.GetRoot());

        push_requests();
    }

    void JsonReader::push_requests()
    {
        auto stop_open = [&](const Dict& dict)
        {
            Stops stop;

            stop.name = dict.at("name").AsString();
            stop.coordinates.lat = dict.at("latitude").AsDouble();
            stop.coordinates.lng = dict.at("longitude").AsDouble();

            if (dict.count("road_distances"s))
                for (const auto& [key, val] : dict.at("road_distances"s).AsMap())
                    stop.road_distances.emplace(key, val.AsInt());
                
            save_stop.push_back(move(stop));
            u_stop_catalogue[stop.name] = &save_stop.back();
        };

        auto bus_open = [&](const Dict& dict)
        {
            Buses bus;

            bus.name = dict.at("name"s).AsString();
            for (const auto& val : dict.at("stops"s).AsArray())
                bus.stops.push_back(val.AsString());
            bus.is_roundtrip = dict.at("is_roundtrip"s).AsBool();


            save_bus.push_back(move(bus));
            u_bus_catalogue[bus.name] = &save_bus.back();
        };

        for (const auto& vnode : nd->AsMap().at("base_requests"s).AsArray())
        {
            if ("Stop"sv == vnode.AsMap().at("type"s).AsString())
                stop_open(vnode.AsMap());

            else bus_open(vnode.AsMap());
        }
    }

    void JsonReader::print(const RequestHandler& requests, ostream& output)
    {
        auto error_response = [](Dict& dict, int id)
        {
            dict.insert
            ({
                { "request_id"s, id },
                { "error_message"s, "not found"s }
            });
        };

        auto bus_response = [&](Dict& dict, const string_view& name, int id)
        {
            if (auto result = requests.get_bus_stat(name))
                dict.insert
                ({
                    { "curvature"s, result->curvature },
                    { "request_id"s, id },
                    { "route_length"s, result->route_distances },
                    { "stop_count"s, result->stop_count },
                    { "unique_stop_count"s, result->unique_stop_count }
                });

            else error_response(dict, id);
        };

        auto stop_response = [&](Dict& dict, const string_view& name, int id)
        {
            Array arr;
            if (const auto& result = requests.get_buses_by_stop(name))
            {
                for (const auto& bus_ptr : *result)
                    if (bus_ptr.name.size())
                        arr.push_back(move(static_cast<string>(bus_ptr.name)));

                dict.insert
                ({
                    { "buses"s, move(arr) },
                    { "request_id"s, id }
                });
            }

            else error_response(dict, id);
        };

        Array arr;
        for (const auto& vnode : nd->AsMap().at("stat_requests"s).AsArray())
        {
            Dict dict;
            if ("Stop"sv == vnode.AsMap().at("type"s).AsString())
                stop_response(dict, vnode.AsMap().at("name"s).AsString(), vnode.AsMap().at("id"s).AsInt());

            else bus_response(dict, vnode.AsMap().at("name"s).AsString(), vnode.AsMap().at("id"s).AsInt());
            arr.emplace_back(move(dict));
        }

        Print(Document{ arr }, output);
    }
}
