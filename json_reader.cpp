#include "json_reader.h"

namespace reader {
    void JsonReader::parse(istream& input) {
        json::Document doc = Load(input);
        nd = move(make_unique<Node>(doc.GetRoot()));

        push_requests();
        push_rendsettings();
    }

    void JsonReader::push_requests() {
        auto stop_open = [&](const Dict& dict) {
            Stop stop;
            stop.name = dict.at("name").AsString();
            stop.coordinates.lat = dict.at("latitude").AsDouble();
            stop.coordinates.lng = dict.at("longitude").AsDouble();

            if (dict.count("road_distances"s)) {
                for (const auto& [key, val] : dict.at("road_distances"s).AsMap()) {
                    stop.road_distances.emplace(key, val.AsInt());
                }
            }                
            renderer.push_stop(stop);
        };

        auto bus_open = [&](const Dict& dict) {
            Bus bus;
            bus.name = dict.at("name"s).AsString();
            for (const auto& val : dict.at("stops"s).AsArray()) {
                bus.stops.push_back(val.AsString());
            }
            bus.is_roundtrip = dict.at("is_roundtrip"s).AsBool();

            renderer.push_bus(bus);
        };

        for (const auto& vnode : nd->AsMap().at("base_requests"s).AsArray()) {
            if ("Stop"sv == vnode.AsMap().at("type"s).AsString()) {
                stop_open(vnode.AsMap());
            }
            else bus_open(vnode.AsMap());
        }
    }

    Color color(const Node& color) {
        if (color.IsString()) {
            return color.AsString();
        }
        else if (color.IsArray()) {
            if (color.AsArray().size() == 3) {
                return Rgb(
                    color.AsArray()[0].AsInt(),
                    color.AsArray()[1].AsInt(),
                    color.AsArray()[2].AsInt()
                );
            }
            else {
                return Rgba(
                    color.AsArray()[0].AsInt(),
                    color.AsArray()[1].AsInt(),
                    color.AsArray()[2].AsInt(),
                    color.AsArray()[3].AsDouble()
                );
            }
        }
        else { 
            return monostate{}; 
        }
    }

    void JsonReader::push_rendsettings() {        
        Settings settings;
        const auto& dict = nd->AsMap().at("render_settings"s).AsMap();
        settings.width = dict.at("width"s).AsDouble();
        settings.height = dict.at("height"s).AsDouble();
        settings.padding = dict.at("padding"s).AsDouble();
        settings.stop_radius = dict.at("stop_radius"s).AsDouble();
        settings.line_width = dict.at("line_width"s).AsDouble();
        settings.bus_label_font_size = dict.at("bus_label_font_size"s).AsInt();
        settings.bus_label_offset = { 
            dict.at("bus_label_offset"s).AsArray()[0].AsDouble(), 
            dict.at("bus_label_offset"s).AsArray()[1].AsDouble() 
        };
        settings.stop_label_font_size = dict.at("stop_label_font_size"s).AsInt();
        settings.stop_label_offset = {
            dict.at("stop_label_offset"s).AsArray()[0].AsDouble(),
            dict.at("stop_label_offset"s).AsArray()[1].AsDouble()
        };
        settings.underlayer_color = color(dict.at("underlayer_color"s));
        settings.underlayer_width = dict.at("underlayer_width"s).AsDouble();
        for (const auto& color_ : dict.at("color_palette"s).AsArray()) {
            settings.color_palette.push_back(color(color_));
        }

        renderer.push_settings(settings);
    }

    void JsonReader::print(const RequestHandler& requests, ostream& output) {
        auto error_response = [](Dict& dict, int id) {
            dict.insert({
                { "request_id"s, id },
                { "error_message"s, "not found"s }
            });
        };

        auto bus_response = [&](Dict& dict, const string_view& name, int id) {
            if (auto result = requests.get_bus_stat(name)) {
                dict.insert({
                    { "curvature"s, result->curvature },
                    { "request_id"s, id },
                    { "route_length"s, result->route_distances },
                    { "stop_count"s, result->stop_count },
                    { "unique_stop_count"s, result->unique_stop_count }
                });
            }
            else error_response(dict, id);
        };

        auto stop_response = [&](Dict& dict, const string_view& name, int id) {
            Array arr;
            if (const auto& result = requests.get_buses_by_stop(name)) {
                for (const auto& bus_ptr : *result) {
                    if (bus_ptr.name.size()) {
                        arr.push_back(move(static_cast<string>(bus_ptr.name)));
                    }
                }

                dict.insert({
                    { "buses"s, move(arr) },
                    { "request_id"s, id }
                });
            }
            else error_response(dict, id);
        };

        Array arr;
        for (const auto& vnode : nd->AsMap().at("stat_requests"s).AsArray()) {
            Dict dict;
            if ("Stop"sv == vnode.AsMap().at("type"s).AsString()) {
                stop_response(dict, vnode.AsMap().at("name"s).AsString(), vnode.AsMap().at("id"s).AsInt());
            }
            else if ("Bus"sv == vnode.AsMap().at("type"s).AsString()) {
                bus_response(dict, vnode.AsMap().at("name"s).AsString(), vnode.AsMap().at("id"s).AsInt());
            }
            else {
                stringstream strm;
                svg::Document doc = requests.render_map();
                doc.Render(strm);
                //doc.Render(output);
                dict.insert({
                    { "map"s, move(strm.str()) },
                    { "request_id"s, vnode.AsMap().at("id"s).AsInt() }
                });
            }
            arr.emplace_back(move(dict));
        }
        Print(json::Document{arr}, output);
    }
}
