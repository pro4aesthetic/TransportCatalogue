#include "json_reader.h"

void JsonReader::push_base_requests()
{
	auto node_is_open_stop = [&](const Dict& dict)
	{
		string_view type, name;
		double lat = 0.0, lng = 0.0;
		unordered_map<string_view, int> road_distances;

		for (const auto& [key, value] : dict)
		{
			if (key == "type"sv && value.IsString())
				type = value.AsString();

			else if (key == "name"sv && value.IsString())
				name = value.AsString();

			else if (key == "latitude"sv)
			{
				if (value.IsInt())
					lat = double(abs(value.AsInt()));

				else if (value.IsDouble())
					lat = abs(value.AsDouble());
			}

			else if (key == "longitude"sv)
			{
				if (value.IsInt())
					lng = double(abs(value.AsInt()));

				else if (value.IsDouble())
					lng = abs(value.AsDouble());
			}

			else if (key == "road_distances"sv && value.IsMap())
			{
				for (const auto& [key, val] : value.AsMap())
				{
					if (val.IsInt())
						road_distances.emplace(key, val.AsInt());
					else continue;
				}
			}

			else break;
		}

		save_stop.push_back(stops{ type, name, Coordinates{ lat, lng }, road_distances });
		u_stop_catalogue[name] = &save_stop.back();
	};

	auto node_is_open_bus = [&](const Dict& dict)
	{
		string_view type, name;
		vector<string_view> stops;
		bool is_roundtrip = true;

		for (const auto& [key, value] : dict)
		{
			if (key == "type"sv && value.IsString())
				type = value.AsString();

			else if (key == "name"sv && value.IsString())
				name = value.AsString();

			else if (key == "stops"sv && value.IsArray())
			{
				for (const auto& val : value.AsArray())
				{
					if (val.IsString())
						stops.push_back(val.AsString());

					else continue;
				}
			}

			else if (key == "is_roundtrip"sv && value.IsBool())
				is_roundtrip = value.AsBool();

			else continue;
		}

		save_bus.push_back(buses{ type, name, stops, is_roundtrip });
		u_bus_catalogue[name] = &save_bus.back();
	};

	for (const auto& vnode : nd.AsArray())
	{
		if ("Stop"sv == vnode.AsMap().at("type"s).AsString()) 
			node_is_open_stop(vnode.AsMap());
		
		else node_is_open_bus(vnode.AsMap());
	}
}

Node parse_requests()
{
	//string js;
	stringstream strm;
	/*
	while (getline(cin, js))
	{
		static int counter = 0;
		if (js.find('{') != string::npos)
			++counter;

		if (js.find('}') != string::npos)
			++counter;

		strm << js;

		if (counter % 2 == 0 && !js.find('}')) break;
		strm << endl;
	}
	*/

	Dict dict;
	dict.insert({
			{"base_requests"s,
				Array
				{
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Tolstopaltsevo"s},
						{"latitude"s, 55.611087},
						{"longitude"s, 37.20829},
						{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Marushkino"s},
						{"latitude"s, 55.595884},
						{"longitude"s, 37.209755},
						{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
					},
					Dict{
						{"type"s, "Bus"s},
						{"name"s, "256"s},
						{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
						{"is_roundtrip"s, true}
					},
					Dict{
						{"type"s, "Bus"s},
						{"name"s, "750"s},
						{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
						{"is_roundtrip"s, false}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Rasskazovka"s},
						{"latitude"s, 55.632761},
						{"longitude"s, 37.333324},
						{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Biryulyovo Zapadnoye"s},
						{"latitude"s, 55.574371},
						{"longitude"s , 37.6517},
						{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Biryusinka"s},
						{"latitude"s, 55.581065},
						{"longitude"s, 37.64839},
						{"road_distances"s, Dict{{"Universam"s, 750}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Universam"s},
						{"latitude"s, 55.587655},
						{"longitude"s , 37.645687},
						{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Biryulyovo Tovarnaya"s},
						{"latitude"s, 55.592028},
						{"longitude"s, 37.653656},
						{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Biryulyovo Passazhirskaya"s},
						{"latitude"s, 55.580999},
						{"longitude"s, 37.659164},
						{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
					},
					Dict{
						{"type"s, "Bus"s},
						{"name"s, "828"s},
						{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
						{"is_roundtrip"s, true}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Rossoshanskaya ulitsa"s},
						{"latitude"s, 55.595579},
						{"longitude"s, 37.605757},
						{"road_distances"s, Dict{}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s,"Prazhskaya"s},
						{"latitude"s, 55.611678},
						{"longitude"s, 37.603831},
						{"road_distances"s, Dict{}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Tolstopaltsevo"s},
						{"latitude"s, 55.611087},
						{"longitude"s, 37.20829},
						{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Marushkino"s},
						{"latitude"s, 55.595884},
						{"longitude"s, 37.209755},
						{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
					},
					Dict{
						{"type"s, "Bus"s},
						{"name"s, "256"s},
						{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
						{"is_roundtrip"s, true}
					},
					Dict{
						{"type"s, "Bus"s},
						{"name"s, "750"s},
						{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
						{"is_roundtrip"s, false}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Rasskazovka"s},
						{"latitude"s, 55.632761},
						{"longitude"s, 37.333324},
						{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Biryulyovo Zapadnoye"s},
						{"latitude"s, 55.574371},
						{"longitude"s , 37.6517},
						{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Biryusinka"s},
						{"latitude"s, 55.581065},
						{"longitude"s, 37.64839},
						{"road_distances"s, Dict{{"Universam"s, 750}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Universam"s},
						{"latitude"s, 55.587655},
						{"longitude"s , 37.645687},
						{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Biryulyovo Tovarnaya"s},
						{"latitude"s, 55.592028},
						{"longitude"s, 37.653656},
						{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Biryulyovo Passazhirskaya"s},
						{"latitude"s, 55.580999},
						{"longitude"s, 37.659164},
						{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
					},
					Dict{
						{"type"s, "Bus"s},
						{"name"s, "828"s},
						{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
						{"is_roundtrip"s, true}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s, "Rossoshanskaya ulitsa"s},
						{"latitude"s, 55.595579},
						{"longitude"s, 37.605757},
						{"road_distances"s, Dict{}}
					},
					Dict{
						{"type"s, "Stop"s},
						{"name"s,"Prazhskaya"s},
						{"latitude"s, 55.611678},
						{"longitude"s, 37.603831},
						{"road_distances"s, Dict{}}
					},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Tolstopaltsevo"s},
								{"latitude"s, 55.611087},
								{"longitude"s, 37.20829},
								{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Marushkino"s},
								{"latitude"s, 55.595884},
								{"longitude"s, 37.209755},
								{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
						},
							Dict{
								{"type"s, "Bus"s},
								{"name"s, "256"s},
								{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
								{"is_roundtrip"s, true}
						},
							Dict{
								{"type"s, "Bus"s},
								{"name"s, "750"s},
								{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
								{"is_roundtrip"s, false}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Rasskazovka"s},
								{"latitude"s, 55.632761},
								{"longitude"s, 37.333324},
								{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Biryulyovo Zapadnoye"s},
								{"latitude"s, 55.574371},
								{"longitude"s , 37.6517},
								{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Biryusinka"s},
								{"latitude"s, 55.581065},
								{"longitude"s, 37.64839},
								{"road_distances"s, Dict{{"Universam"s, 750}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Universam"s},
								{"latitude"s, 55.587655},
								{"longitude"s , 37.645687},
								{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Biryulyovo Tovarnaya"s},
								{"latitude"s, 55.592028},
								{"longitude"s, 37.653656},
								{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Biryulyovo Passazhirskaya"s},
								{"latitude"s, 55.580999},
								{"longitude"s, 37.659164},
								{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
						},
							Dict{
								{"type"s, "Bus"s},
								{"name"s, "828"s},
								{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
								{"is_roundtrip"s, true}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Rossoshanskaya ulitsa"s},
								{"latitude"s, 55.595579},
								{"longitude"s, 37.605757},
								{"road_distances"s, Dict{}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s,"Prazhskaya"s},
								{"latitude"s, 55.611678},
								{"longitude"s, 37.603831},
								{"road_distances"s, Dict{}}
						},
							Dict{
							{"type"s, "Stop"s},
							{"name"s, "Tolstopaltsevo"s},
							{"latitude"s, 55.611087},
							{"longitude"s, 37.20829},
							{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Marushkino"s},
								{"latitude"s, 55.595884},
								{"longitude"s, 37.209755},
								{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
						},
							Dict{
								{"type"s, "Bus"s},
								{"name"s, "256"s},
								{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
								{"is_roundtrip"s, true}
						},
							Dict{
								{"type"s, "Bus"s},
								{"name"s, "750"s},
								{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
								{"is_roundtrip"s, false}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Rasskazovka"s},
								{"latitude"s, 55.632761},
								{"longitude"s, 37.333324},
								{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Biryulyovo Zapadnoye"s},
								{"latitude"s, 55.574371},
								{"longitude"s , 37.6517},
								{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Biryusinka"s},
								{"latitude"s, 55.581065},
								{"longitude"s, 37.64839},
								{"road_distances"s, Dict{{"Universam"s, 750}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Universam"s},
								{"latitude"s, 55.587655},
								{"longitude"s , 37.645687},
								{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Biryulyovo Tovarnaya"s},
								{"latitude"s, 55.592028},
								{"longitude"s, 37.653656},
								{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Biryulyovo Passazhirskaya"s},
								{"latitude"s, 55.580999},
								{"longitude"s, 37.659164},
								{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
						},
							Dict{
								{"type"s, "Bus"s},
								{"name"s, "828"s},
								{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
								{"is_roundtrip"s, true}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s, "Rossoshanskaya ulitsa"s},
								{"latitude"s, 55.595579},
								{"longitude"s, 37.605757},
								{"road_distances"s, Dict{}}
						},
							Dict{
								{"type"s, "Stop"s},
								{"name"s,"Prazhskaya"s},
								{"latitude"s, 55.611678},
								{"longitude"s, 37.603831},
								{"road_distances"s, Dict{}}
						},
									Dict{
									{"type"s, "Stop"s},
									{"name"s, "Tolstopaltsevo"s},
									{"latitude"s, 55.611087},
									{"longitude"s, 37.20829},
									{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Marushkino"s},
										{"latitude"s, 55.595884},
										{"longitude"s, 37.209755},
										{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
								},
									Dict{
										{"type"s, "Bus"s},
										{"name"s, "256"s},
										{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
										{"is_roundtrip"s, true}
								},
									Dict{
										{"type"s, "Bus"s},
										{"name"s, "750"s},
										{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
										{"is_roundtrip"s, false}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Rasskazovka"s},
										{"latitude"s, 55.632761},
										{"longitude"s, 37.333324},
										{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Biryulyovo Zapadnoye"s},
										{"latitude"s, 55.574371},
										{"longitude"s , 37.6517},
										{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Biryusinka"s},
										{"latitude"s, 55.581065},
										{"longitude"s, 37.64839},
										{"road_distances"s, Dict{{"Universam"s, 750}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Universam"s},
										{"latitude"s, 55.587655},
										{"longitude"s , 37.645687},
										{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Biryulyovo Tovarnaya"s},
										{"latitude"s, 55.592028},
										{"longitude"s, 37.653656},
										{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Biryulyovo Passazhirskaya"s},
										{"latitude"s, 55.580999},
										{"longitude"s, 37.659164},
										{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
								},
									Dict{
										{"type"s, "Bus"s},
										{"name"s, "828"s},
										{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
										{"is_roundtrip"s, true}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Rossoshanskaya ulitsa"s},
										{"latitude"s, 55.595579},
										{"longitude"s, 37.605757},
										{"road_distances"s, Dict{}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s,"Prazhskaya"s},
										{"latitude"s, 55.611678},
										{"longitude"s, 37.603831},
										{"road_distances"s, Dict{}}
								},
									Dict{
							{"type"s, "Stop"s},
							{"name"s, "Tolstopaltsevo"s},
							{"latitude"s, 55.611087},
							{"longitude"s, 37.20829},
							{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Marushkino"s},
										{"latitude"s, 55.595884},
										{"longitude"s, 37.209755},
										{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
								},
									Dict{
										{"type"s, "Bus"s},
										{"name"s, "256"s},
										{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
										{"is_roundtrip"s, true}
								},
									Dict{
										{"type"s, "Bus"s},
										{"name"s, "750"s},
										{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
										{"is_roundtrip"s, false}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Rasskazovka"s},
										{"latitude"s, 55.632761},
										{"longitude"s, 37.333324},
										{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Biryulyovo Zapadnoye"s},
										{"latitude"s, 55.574371},
										{"longitude"s , 37.6517},
										{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Biryusinka"s},
										{"latitude"s, 55.581065},
										{"longitude"s, 37.64839},
										{"road_distances"s, Dict{{"Universam"s, 750}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Universam"s},
										{"latitude"s, 55.587655},
										{"longitude"s , 37.645687},
										{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Biryulyovo Tovarnaya"s},
										{"latitude"s, 55.592028},
										{"longitude"s, 37.653656},
										{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Biryulyovo Passazhirskaya"s},
										{"latitude"s, 55.580999},
										{"longitude"s, 37.659164},
										{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
								},
									Dict{
										{"type"s, "Bus"s},
										{"name"s, "828"s},
										{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
										{"is_roundtrip"s, true}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s, "Rossoshanskaya ulitsa"s},
										{"latitude"s, 55.595579},
										{"longitude"s, 37.605757},
										{"road_distances"s, Dict{}}
								},
									Dict{
										{"type"s, "Stop"s},
										{"name"s,"Prazhskaya"s},
										{"latitude"s, 55.611678},
										{"longitude"s, 37.603831},
										{"road_distances"s, Dict{}}
								},
											Dict{
									{"type"s, "Stop"s},
									{"name"s, "Tolstopaltsevo"s},
									{"latitude"s, 55.611087},
									{"longitude"s, 37.20829},
									{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Marushkino"s},
												{"latitude"s, 55.595884},
												{"longitude"s, 37.209755},
												{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
										},
											Dict{
												{"type"s, "Bus"s},
												{"name"s, "26"s},
												{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Univeam"s, "Biryuovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
												{"is_roundtrip"s, true}
										},
											Dict{
												{"type"s, "Bus"s},
												{"name"s, "75"s},
												{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
												{"is_roundtrip"s, false}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Rasskka"s},
												{"latitude"s, 55.632761},
												{"longitude"s, 37.333324},
												{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Biryulyovo Zapadnoye"s},
												{"latitude"s, 55.574371},
												{"longitude"s , 37.6517},
												{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Biryuka"s},
												{"latitude"s, 55.581065},
												{"longitude"s, 37.64839},
												{"road_distances"s, Dict{{"Universam"s, 750}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Univeam"s},
												{"latitude"s, 55.587655},
												{"longitude"s , 37.645687},
												{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Biryulo Tovarnaya"s},
												{"latitude"s, 55.592028},
												{"longitude"s, 37.653656},
												{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Biryulyovo Passazhirskaya"s},
												{"latitude"s, 55.580999},
												{"longitude"s, 37.659164},
												{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
										},
											Dict{
												{"type"s, "Bus"s},
												{"name"s, "828"s},
												{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
												{"is_roundtrip"s, true}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Rossoshanskaya ulitsa"s},
												{"latitude"s, 55.595579},
												{"longitude"s, 37.605757},
												{"road_distances"s, Dict{}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s,"Prazhskaya"s},
												{"latitude"s, 55.611678},
												{"longitude"s, 37.603831},
												{"road_distances"s, Dict{}}
										},
											Dict{
							{"type"s, "Stop"s},
							{"name"s, "Tolstopaltsevo"s},
							{"latitude"s, 55.611087},
							{"longitude"s, 37.20829},
							{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Marushkino"s},
												{"latitude"s, 55.595884},
												{"longitude"s, 37.209755},
												{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
										},
											Dict{
												{"type"s, "Bus"s},
												{"name"s, "256"s},
												{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
												{"is_roundtrip"s, true}
										},
											Dict{
												{"type"s, "Bus"s},
												{"name"s, "750"s},
												{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
												{"is_roundtrip"s, false}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Rasskazovka"s},
												{"latitude"s, 55.632761},
												{"longitude"s, 37.333324},
												{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Biryulyovo Zapadnoye"s},
												{"latitude"s, 55.574371},
												{"longitude"s , 37.6517},
												{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Biryusinka"s},
												{"latitude"s, 55.581065},
												{"longitude"s, 37.64839},
												{"road_distances"s, Dict{{"Universam"s, 750}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Universam"s},
												{"latitude"s, 55.587655},
												{"longitude"s , 37.645687},
												{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Biryulyovo Tovarnaya"s},
												{"latitude"s, 55.592028},
												{"longitude"s, 37.653656},
												{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Biryulyovo Passazhirskaya"s},
												{"latitude"s, 55.580999},
												{"longitude"s, 37.659164},
												{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
										},
											Dict{
												{"type"s, "Bus"s},
												{"name"s, "828"s},
												{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
												{"is_roundtrip"s, true}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s, "Rossoshanskaya ulitsa"s},
												{"latitude"s, 55.595579},
												{"longitude"s, 37.605757},
												{"road_distances"s, Dict{}}
										},
											Dict{
												{"type"s, "Stop"s},
												{"name"s,"Prazhskaya"s},
												{"latitude"s, 55.611678},
												{"longitude"s, 37.603831},
												{"road_distances"s, Dict{}}
										},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Tolstopaltsevo"s},
														{"latitude"s, 55.611087},
														{"longitude"s, 37.20829},
														{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Marushkino"s},
														{"latitude"s, 55.595884},
														{"longitude"s, 37.209755},
														{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "256"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "750"s},
														{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
														{"is_roundtrip"s, false}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rasskazovka"s},
														{"latitude"s, 55.632761},
														{"longitude"s, 37.333324},
														{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Zapadnoye"s},
														{"latitude"s, 55.574371},
														{"longitude"s , 37.6517},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryusinka"s},
														{"latitude"s, 55.581065},
														{"longitude"s, 37.64839},
														{"road_distances"s, Dict{{"Universam"s, 750}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Universam"s},
														{"latitude"s, 55.587655},
														{"longitude"s , 37.645687},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Tovarnaya"s},
														{"latitude"s, 55.592028},
														{"longitude"s, 37.653656},
														{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Passazhirskaya"s},
														{"latitude"s, 55.580999},
														{"longitude"s, 37.659164},
														{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "828"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rossoshanskaya ulitsa"s},
														{"latitude"s, 55.595579},
														{"longitude"s, 37.605757},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s,"Prazhskaya"s},
														{"latitude"s, 55.611678},
														{"longitude"s, 37.603831},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Tolstopaltsevo"s},
														{"latitude"s, 55.611087},
														{"longitude"s, 37.20829},
														{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Marushkino"s},
														{"latitude"s, 55.595884},
														{"longitude"s, 37.209755},
														{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "256"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "750"s},
														{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
														{"is_roundtrip"s, false}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rasskazovka"s},
														{"latitude"s, 55.632761},
														{"longitude"s, 37.333324},
														{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Zapadnoye"s},
														{"latitude"s, 55.574371},
														{"longitude"s , 37.6517},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryusinka"s},
														{"latitude"s, 55.581065},
														{"longitude"s, 37.64839},
														{"road_distances"s, Dict{{"Universam"s, 750}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Universam"s},
														{"latitude"s, 55.587655},
														{"longitude"s , 37.645687},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Tovarnaya"s},
														{"latitude"s, 55.592028},
														{"longitude"s, 37.653656},
														{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Passazhirskaya"s},
														{"latitude"s, 55.580999},
														{"longitude"s, 37.659164},
														{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "828"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rossoshanskaya ulitsa"s},
														{"latitude"s, 55.595579},
														{"longitude"s, 37.605757},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s,"Prazhskaya"s},
														{"latitude"s, 55.611678},
														{"longitude"s, 37.603831},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Tolstopaltsevo"s},
														{"latitude"s, 55.611087},
														{"longitude"s, 37.20829},
														{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Marushkino"s},
														{"latitude"s, 55.595884},
														{"longitude"s, 37.209755},
														{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "256"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "750"s},
														{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
														{"is_roundtrip"s, false}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rasskazovka"s},
														{"latitude"s, 55.632761},
														{"longitude"s, 37.333324},
														{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Zapadnoye"s},
														{"latitude"s, 55.574371},
														{"longitude"s , 37.6517},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryusinka"s},
														{"latitude"s, 55.581065},
														{"longitude"s, 37.64839},
														{"road_distances"s, Dict{{"Universam"s, 750}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Universam"s},
														{"latitude"s, 55.587655},
														{"longitude"s , 37.645687},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Tovarnaya"s},
														{"latitude"s, 55.592028},
														{"longitude"s, 37.653656},
														{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Passazhirskaya"s},
														{"latitude"s, 55.580999},
														{"longitude"s, 37.659164},
														{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "828"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rossoshanskaya ulitsa"s},
														{"latitude"s, 55.595579},
														{"longitude"s, 37.605757},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s,"Prazhskaya"s},
														{"latitude"s, 55.611678},
														{"longitude"s, 37.603831},
														{"road_distances"s, Dict{}}
												},
													Dict{
													{"type"s, "Stop"s},
													{"name"s, "Tolstltsevo"s},
													{"latitude"s, 55.611087},
													{"longitude"s, 37.20829},
													{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Marushkino"s},
														{"latitude"s, 55.595884},
														{"longitude"s, 37.209755},
														{"road_distances"s, Dict{{"Rasszovka"s, 9900}, {"Marushkino"s , 100}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "26"s},
														{"stops"s, Array{"Biryulyovo Zanoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "750"s},
														{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Mhkino"s, "Rasskazovka"s }},
														{"is_roundtrip"s, false}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rasskaka"s},
														{"latitude"s, 55.632761},
														{"longitude"s, 37.333324},
														{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Zapadnoye"s},
														{"latitude"s, 55.574371},
														{"longitude"s , 37.6517},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryusinka"s},
														{"latitude"s, 55.581065},
														{"longitude"s, 37.64839},
														{"road_distances"s, Dict{{"Universam"s, 750}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Universam"s},
														{"latitude"s, 55.587655},
														{"longitude"s , 37.645687},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Tovarnaya"s},
														{"latitude"s, 55.592028},
														{"longitude"s, 37.653656},
														{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Passazhirskaya"s},
														{"latitude"s, 55.580999},
														{"longitude"s, 37.659164},
														{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "828"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rossoshanskaya ulitsa"s},
														{"latitude"s, 55.595579},
														{"longitude"s, 37.605757},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s,"Prazhskaya"s},
														{"latitude"s, 55.611678},
														{"longitude"s, 37.603831},
														{"road_distances"s, Dict{}}
												},
													Dict{
													{"type"s, "Stop"s},
													{"name"s, "Tolstopaltsevo"s},
													{"latitude"s, 55.611087},
													{"longitude"s, 37.20829},
													{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Marushkino"s},
														{"latitude"s, 55.595884},
														{"longitude"s, 37.209755},
														{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "256"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "750"s},
														{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
														{"is_roundtrip"s, false}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rasskazovka"s},
														{"latitude"s, 55.632761},
														{"longitude"s, 37.333324},
														{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Zapadnoye"s},
														{"latitude"s, 55.574371},
														{"longitude"s , 37.6517},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryusinka"s},
														{"latitude"s, 55.581065},
														{"longitude"s, 37.64839},
														{"road_distances"s, Dict{{"Universam"s, 750}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Universam"s},
														{"latitude"s, 55.587655},
														{"longitude"s , 37.645687},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Tovarnaya"s},
														{"latitude"s, 55.592028},
														{"longitude"s, 37.653656},
														{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Passazhirskaya"s},
														{"latitude"s, 55.580999},
														{"longitude"s, 37.659164},
														{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "828"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya ulitsa"s, "Biryulyovo Zapadnoye"s} },
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rossoshanskaya ulitsa"s},
														{"latitude"s, 55.595579},
														{"longitude"s, 37.605757},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s,"Prazhskaya"s},
														{"latitude"s, 55.611678},
														{"longitude"s, 37.603831},
														{"road_distances"s, Dict{}}
												},
													Dict{
											{"type"s, "Stop"s},
											{"name"s, "Tolstltsevo"s},
											{"latitude"s, 55.611087},
											{"longitude"s, 37.20829},
											{"road_distances"s, Dict{{"Marhkino"s, 3900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Marushkino"s},
														{"latitude"s, 55.595884},
														{"longitude"s, 37.209755},
														{"road_distances"s, Dict{{"Rasskaka"s, 9900}, {"Marushkino"s , 100}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "256"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryinka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "750"s},
														{"stops"s, Array{"Tolstopaltsevo"s, "Marukino"s, "Marushkino"s, "Rasskazovka"s }},
														{"is_roundtrip"s, false}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rasskvka"s},
														{"latitude"s, 55.632761},
														{"longitude"s, 37.333324},
														{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Zapadnoye"s},
														{"latitude"s, 55.574371},
														{"longitude"s , 37.6517},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryuinka"s, 1800}, {"Unirsam"s, 2400}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryuinka"s},
														{"latitude"s, 55.581065},
														{"longitude"s, 37.64839},
														{"road_distances"s, Dict{{"Universam"s, 750}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Uniersam"s},
														{"latitude"s, 55.587655},
														{"longitude"s , 37.645687},
														{"road_distances"s, Dict{{"Rossoshanska ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Tovarnaya"s},
														{"latitude"s, 55.592028},
														{"longitude"s, 37.653656},
														{"road_distances"s, Dict{{"Biryuvo Pasazhirskaya"s, 1300}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Passazhirskaya"s},
														{"latitude"s, 55.580999},
														{"longitude"s, 37.659164},
														{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "828"s},
														{"stops"s, Array{"Biryadnoye"s, "Universam"s, "Rosotsa"s, "Biryulyovo Zapadnoye"s} },
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rosulitsa"s},
														{"latitude"s, 55.595579},
														{"longitude"s, 37.605757},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s,"Prazhskaya"s},
														{"latitude"s, 55.611678},
														{"longitude"s, 37.603831},
														{"road_distances"s, Dict{}}
												},
													Dict{
											{"type"s, "Stop"s},
											{"name"s, "Tolstopaltsevo"s},
											{"latitude"s, 55.611087},
											{"longitude"s, 37.20829},
											{"road_distances"s, Dict{{"Marusino"s, 3900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Makino"s},
														{"latitude"s, 55.595884},
														{"longitude"s, 37.209755},
														{"road_distances"s, Dict{{"Rasska"s, 9900}, {"Marushkino"s , 100}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "2564"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Birynka"s, "Universam"s, "Biryulyovo Tovarnaya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "750"s},
														{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Marushkino"s, "Rasskazovka"s }},
														{"is_roundtrip"s, false}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rasskazovka"s},
														{"latitude"s, 55.632761},
														{"longitude"s, 37.333324},
														{"road_distances"s, Dict{{"Marushkino"s, 9500}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Zapadnoye"s},
														{"latitude"s, 55.574371},
														{"longitude"s , 37.6517},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryusinka"s},
														{"latitude"s, 55.581065},
														{"longitude"s, 37.64839},
														{"road_distances"s, Dict{{"Universam"s, 750}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Universam"s},
														{"latitude"s, 55.587655},
														{"longitude"s , 37.645687},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Tovarnaya"s},
														{"latitude"s, 55.592028},
														{"longitude"s, 37.653656},
														{"road_distances"s, Dict{{"Biryulyovzhirskaya"s, 1300}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyohirskaya"s},
														{"latitude"s, 55.580999},
														{"longitude"s, 37.659164},
														{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "828"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "rsam"s, "Rossoshanskaya ulitsa"s, "Biryulyye"s} },
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rossoshanskatsa"s},
														{"latitude"s, 55.595579},
														{"longitude"s, 37.605757},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s,"Prazya"s},
														{"latitude"s, 55.611678},
														{"longitude"s, 37.603831},
														{"road_distances"s, Dict{}}
												},
													Dict{
									{"type"s, "Stop"s},
									{"name"s, "Tolstopaltsevo"s},
									{"latitude"s, 55.611087},
									{"longitude"s, 37.20829},
									{"road_distances"s, Dict{{"Marushkino"s, 3900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Maino"s},
														{"latitude"s, 55.595884},
														{"longitude"s, 37.209755},
														{"road_distances"s, Dict{{"Rasskazovka"s, 9900}, {"Marushkino"s , 100}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "2546"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Biryusinka"s, "Universam"s, "Biryulya"s, "Biryulyovo Passazhirskaya"s, "Biryulyovo Zapadnoye"s}},
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "7504"s},
														{"stops"s, Array{"Tolstopaltsevo"s, "Marushkino"s, "Maruno"s, "Rasskazovka"s }},
														{"is_roundtrip"s, false}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Raka"s},
														{"latitude"s, 55.632761},
														{"longitude"s, 37.333324},
														{"road_distances"s, Dict{{"Maruino"s, 9500}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Zapadnoye"s},
														{"latitude"s, 55.574371},
														{"longitude"s , 37.6517},
														{"road_distances"s, Dict{{"Rossotsa"s, 7500}, {"Biryusinka"s, 1800}, {"Universam"s, 2400}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Bisinka"s},
														{"latitude"s, 55.581065},
														{"longitude"s, 37.64839},
														{"road_distances"s, Dict{{"Uniersam"s, 750}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Universam"s},
														{"latitude"s, 55.587655},
														{"longitude"s , 37.645687},
														{"road_distances"s, Dict{{"Rossoshanskaya ulitsa"s, 5600}, {"Biryulyovo Tovarnaya"s, 900}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Tovarnaya"s},
														{"latitude"s, 55.592028},
														{"longitude"s, 37.653656},
														{"road_distances"s, Dict{{"Biryulyovo Passazhirskaya"s, 1300}}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Biryulyovo Passazhirskaya"s},
														{"latitude"s, 55.580999},
														{"longitude"s, 37.659164},
														{"road_distances"s, Dict{{"Biryulyovo Zapadnoye"s, 1200}}}
												},
													Dict{
														{"type"s, "Bus"s},
														{"name"s, "8248"s},
														{"stops"s, Array{"Biryulyovo Zapadnoye"s, "Universam"s, "Rossoshanskaya utsa"s, "Biryulyovopadnoye"s} },
														{"is_roundtrip"s, true}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s, "Rossosa ulitsa"s},
														{"latitude"s, 55.595579},
														{"longitude"s, 37.605757},
														{"road_distances"s, Dict{}}
												},
													Dict{
														{"type"s, "Stop"s},
														{"name"s,"Prazhskaya"s},
														{"latitude"s, 55.611678},
														{"longitude"s, 37.603831},
														{"road_distances"s, Dict{}}
												}
				}
			},
			{"stat_requests"s,
				Array
				{
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "2564"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "7450"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "7515"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "BiryulyZapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "2546"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biradnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "2546"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "2556"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "7505"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryupadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "7540"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "7515"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "26"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "50"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyoapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Saara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "25"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "50"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Saara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryuoye"s} },
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "2565"s} },
					Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
					Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
					Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
					Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazaya"s} },
					Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} }
				}
			}
		}
	);
	Print(Document{ dict }, strm);
	Document doc = Load(strm);

	return doc.GetRoot();
}

void result_requests(const RequestHandler& rqhandler, const Node& node)
{
	Array arr;
	for (const auto& vnode : node.AsArray())
	{
		Dict dict;
		if ("Stop"sv == vnode.AsMap().at("type"s).AsString())
		{
			Array arr1;
			if (auto result = rqhandler.get_buses_by_stop(vnode.AsMap().at("name"s).AsString()))
			{
				for (const auto& vec : result->buses)
					arr1.push_back(static_cast<string>(vec));

				dict.insert(
					{
						{"buses"s, arr1},
						{"request_id"s, vnode.AsMap().at("id"s).AsInt()}
					});
			}

			else
				dict.insert(
					{
						{ "request_id"s, vnode.AsMap().at("id"s).AsInt() },
						{ "error_message"s, "not found"s }
					});

			arr.emplace_back(dict);
		}

		else
		{
			if (auto result = rqhandler.get_bus_stat(vnode.AsMap().at("name"s).AsString()))
			{
				dict.insert(
					{
						{"curvature"s, result->curvature},
						{"request_id"s, vnode.AsMap().at("id"s).AsInt()},
						{"route_length"s, result->route_length},
						{"stop_count"s, result->stop_count},
						{"unique_stop_count"s, result->unique_stop_count}
					});
			}

			else
				dict.insert(
					{
						{ "request_id"s, vnode.AsMap().at("id"s).AsInt() },
						{ "error_message"s, "not found"s }
					});

			arr.emplace_back(dict);
		}
	}

	Print(Document{ arr }, cout);
}