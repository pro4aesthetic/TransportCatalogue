#include "json_reader.h"

result_parse info()
{
	string js;
	stringstream strm;
	/*
	while (getline(cin, js))
	{
		static int counter = 0;
		if (js.find('{') != string::npos || js.find('}') != string::npos)
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
	}
				}
			},
			{"stat_requests"s,
				Array
				{
					Dict{ {"id"s, 1}, {"type"s, "Bus"s}, {"name"s, "256"s} },
	Dict{ {"id"s, 2}, {"type"s , "Bus"s}, {"name"s, "750"s} },
	Dict{ {"id"s, 3}, {"type"s, "Bus"s}, {"name"s, "751"s} },
	Dict{ {"id"s, 4}, {"type"s, "Stop"s}, {"name"s, "Samara"s} },
	Dict{ {"id"s, 5}, {"type"s, "Stop"s}, {"name"s, "Prazhskaya"s} },
	Dict{ {"id"s, 6}, {"type"s, "Stop"s}, {"name"s, "Biryulyovo Zapadnoye"s} }
				}
			}
		}
	);

	Print(Document{ dict }, strm);
	
	return parse(strm);
}

tmp_type isopen(const Node& node)
{
	if (node.IsArray())
	{
		vector<string> tmp_vec;
		for (const auto& val : node.AsArray())
		{
			if (val.IsString())
				tmp_vec.push_back(val.AsString());

			else continue;
		}
		return tmp_vec;
	}

	else if (node.IsBool()) return node.AsBool();

	else if (node.IsInt()) return node.AsInt();

	else if (node.IsDouble()) return node.AsDouble();

	else if (node.IsMap())
	{
		map<string, int> tmp_map;
		for (const auto& [key, value] : node.AsMap())
		{
			if (value.IsInt())
				tmp_map.emplace(key, value.AsInt());
			else continue;
		}
		return tmp_map;
	}

	else if (node.IsString()) return node.AsString();

	else return nullptr;
}

result_parse parse(istream& input)
{
	Document doc = Load(input);
	save_inf base_requests, stat_requests;
	Node node = doc.GetRoot();
	Node arr1 = node.AsMap().at("base_requests"s);

	for (const auto& val : arr1.AsArray())
	{
		map<string, tmp_type> datl;
		for (const auto& [key, value] : val.AsMap())
			datl.insert({key, isopen(value)});
		
		base_requests.push_back(datl);
	}

	Node arr2 = node.AsMap().at("stat_requests"s);

	for (const auto& val : arr2.AsArray())
	{
		map<string, tmp_type> datl;
		for (const auto& [key, value] : val.AsMap())
			datl.insert({ key, isopen(value) });

		stat_requests.push_back(datl);
	}

	return {base_requests, stat_requests};
}

void answer_handler(TransportCatalogue& transport, const save_inf& stat_requests)
{
	Array arr;

	for (const auto& map_ : stat_requests)
	{
		if ("Bus"s == get<string>(map_.at("type"s)))
		{
			Dict dict;
			result_bs r1 = answer1(transport, map_);

			if (r1.check)
			{
				dict.insert(
					{
						{"curvature"s, r1.curvature},
						{"request_id"s, r1.request_id},
						{"route_length"s, r1.route_length},
						{"stop_count"s, r1.stop_count},
						{"unique_stop_count"s, r1.unique_stop_count}
					});
			}

			else
				dict.insert(
					{ 
						{ "request_id"s, r1.request_id }, 
						{ "error_message"s, "not found"s } 
					});

			arr.emplace_back(dict);
		}

		else
		{
			Dict dict;
			result_st r2 = answer2(transport, map_);

			Array arr1;
			if (r2.check)
			{
				for (const auto& vec_ : r2.buses)
					arr1.push_back(vec_);

				dict.insert(
					{
						{"buses"s, arr1},
						{"request_id"s, r2.request_id}
					});
			}

			else
				dict.insert(
					{ 
						{ "request_id"s, r2.request_id }, 
						{ "error_message"s, "not found"s } 
					});

			arr.emplace_back(dict);
		}		
	}

	Print(Document{ arr }, cout);
}
/*

void answer_handler(TransportCatalogue& transport, const save_inf& stat_requests)
{
	Array arr;

	for (const auto& map_ : stat_requests)
	{
		if ("Bus"s == get<string>(map_.at("type"s)))
		{
			Dict dict;
			auto r1 = answer1(transport, map_);

			if (r1->check)
			{
				dict.insert(
					{
						{"curvature"s, r1->curvature},
						{"request_id"s, r1->request_id},
						{"route_length"s, r1->route_length},
						{"stop_count"s, r1->stop_count},
						{"unique_stop_count"s, r1->unique_stop_count}
					});
			}

			else
				dict.insert(
					{
						{ "request_id"s, r1->request_id },
						{ "error_message"s, "not found"s }
					});

			arr.emplace_back(dict);
		}

		else
		{
			Dict dict;
			auto r2 = answer2(transport, map_);

			Array arr1;
			if (r2->check)
			{
				for (const auto& vec_ : r2->buses)
					arr1.push_back(vec_);

				dict.insert(
					{
						{"buses"s, arr1},
						{"request_id"s, r2->request_id}
					});
			}

			else
				dict.insert(
					{
						{ "request_id"s, r2->request_id },
						{ "error_message"s, "not found"s }
					});

			arr.emplace_back(dict);
		}
	}

	Print(Document{ arr }, cout);
}*/