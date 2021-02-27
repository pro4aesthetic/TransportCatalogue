#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <hash_set>
#include <cstdlib>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <string_view>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <algorithm>

using namespace std;

#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)
#define LOG_DURATION_STREAM(x, y) LogDuration UNIQUE_VAR_NAME_PROFILE(x, y)

class LogDuration 
{
public:
    // заменим имя типа std::chrono::steady_clock
    // с помощью using для удобства
    using Clock = std::chrono::steady_clock;

    LogDuration(const std::string& id, std::ostream& dst_stream = std::cerr)
        : id_(id)
        , dst_stream_(dst_stream) {
    }

    ~LogDuration() 
    {
        using namespace std::chrono;
        using namespace std::literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        dst_stream_ << id_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }

private:
    const std::string id_;
    const Clock::time_point start_time_ = Clock::now();
    std::ostream& dst_stream_;
};


vector<string> QueryOnCreateTC()
{
    int query_count;
    cin >> query_count;

    vector<string> temp_catalog;
    temp_catalog.reserve(query_count);
    cin.get();
    for (int query = 0; query < query_count; ++query)
    {
        string query_str;
        getline(cin, query_str);
        temp_catalog.push_back(move(query_str));
    }

    return temp_catalog;
}

tuple<vector<string_view>, int> SplitInToWords(string_view str)
{
    vector<string_view> up;
    unordered_set<string_view> tmp;
    int unique_stops = 0;

    if (str.find('>') != str.npos)
    {
        while (true)
        {
            const auto space = str.find('>');
            up.push_back(str.substr(0, space - 1));
            tmp.insert(str.substr(0, space - 1));

            if (space == str.npos)
            {
                unique_stops = tmp.size() - 1;
                break;
            }
            else str.remove_prefix(space + 2);
        }
    }
    else
    {
        while (true)
        {
            const auto space = str.find('-');
            up.push_back(str.substr(0, space - 1));
            tmp.insert(str.substr(0, space - 1));

            if (space == str.npos)
            {
                unique_stops = tmp.size();
                up.insert(up.end(), up.rbegin() + 1, up.rend());
                break;
            }
            else str.remove_prefix(space + 2);
        }
    }

    return { up, unique_stops };
}


struct Coordinates
{
    double lat = 0.0;
    double lng = 0.0;
};

inline double ComputeDistance(Coordinates from, Coordinates to) 
{
    using namespace std;
    static const double dr = 3.1415926535 / 180.;
    return acos(sin(from.lat * dr) * sin(to.lat * dr)
        + cos(from.lat * dr) * cos(to.lat * dr) * cos(abs(from.lng - to.lng) * dr))
        * 6371000;
}


struct In
{
    In() = default;
    constexpr In(string_view name_ = "none"sv, int c_s = 0, int u_s = 0, uint64_t l = 0, double c = 0.0)
        : name(name_),
        count_stops(c_s),
        unique_stops(u_s),
        length(l),
        curvate(c) {
    }

    string_view name;
    int count_stops;
    int unique_stops;
    uint64_t length;
    double curvate;
};

struct Out
{
    Out() = default;
    Out(string_view stop_, const deque<string_view>&inf_)
        : stop(stop_),
        inf(move(inf_)) {
    }

    string_view stop;
    deque<string_view> inf;
};

class TransportCatalogue
{
    struct Bus
    {
        string_view name = "none"sv;
        vector<string_view> up;
        int unique_stops = 0;
    };

    struct Stop
    {
        string_view name = "none"sv;
        Coordinates coo;
    };

public:
    void SetRouteInTC(string_view str)
    {
        string_view name, stops;
        const auto t1 = str.find(':');
        name = str.substr(4, t1 - 4);
        stops = str.substr(t1 + 2, str.npos);
        const auto& [up, unique_stops] = SplitInToWords(stops);

        save_bus.push_back(move(Bus{ name, up, unique_stops }));
        u_bus_catalogue[name] = &save_bus.back();
    }

    void SetStopsInTC(string_view str)
    {
        string_view name, coordinates;
        const auto t1 = str.find(':');
        name = str.substr(5, t1 - 5);
        coordinates = str.substr(t1 + 2, str.npos);

        const auto t2 = coordinates.find(',');
        string_view str1 = coordinates.substr(0, t2), str2 = coordinates.substr(t2 + 2, coordinates.npos);
        const auto t3 = str2.find(',');
        string_view str3 = str2.substr(0, t3);
        double lat = stod(string(str1));
        double lng = stod(string(str3));
        
        if (t3 != str2.npos)
            SetDistanceTC(name, str2.substr(t3 + 2, str2.npos));

        save_stop.push_back(move(Stop{ name, Coordinates{lat, lng} }));
        u_stop_catalogue[name] = &save_stop.back();
    }

    [[nodiscard]] In GetInformationBusTC(string_view str)
    {
        auto bus = FindBuses(str.substr(4, str.npos));

        if (!bus)
            return In{ str.substr(4, str.npos) };

        string_view name = bus->name;
        int count_s = bus->up.size();
        int unique_s = bus->unique_stops;

        uint64_t length = 0;
        double result = 0.0;

        for (auto itr = bus->up.begin(); itr < bus->up.end() - 1; ++itr)
        {
            const Stop* from = FindStops(*itr);
            const Stop* to = FindStops(*(itr + 1));

            length += GetDistanceTC(from->name, to->name);
            result += ComputeDistance(from->coo, to->coo);
        }
        double curvate = length / result;

        return In{ name, count_s, unique_s, length, curvate };
    }

    [[nodiscard]] Out GetInformationStopTC(string_view str)
    {
        deque<string_view> inf;
        string_view name = str.substr(5, str.npos);
        
        if (u_stop_catalogue.count(name))
        {
            for (const auto& bus : save_bus)
            {
                auto it = find(bus.up.begin(), bus.up.end(), name);
                if (it != bus.up.end())
                    inf.push_back(bus.name);
            }
            sort(inf.begin(), inf.end());
            inf.push_front("buses"sv);
        }
        else  inf.push_back("not found"sv);

        
        return Out{ name, inf };
    }

    auto GetStops()
    {
        return u_stop_catalogue;
    }

    auto GetBus()
    {
        return u_bus_catalogue;
    }

private: 
    struct PairHasher
    {
        size_t operator()(pair<string_view, string_view> str) const
        {
            string_view str1, str2;
            tie(str1, str2) = str;
            return static_cast<size_t>(hasher(str1) * 1'000 + hasher(str2) * 1'000'000);
        }

    private:
        hash<string_view> hasher;
    };

    deque<Bus> save_bus;
    deque<Stop> save_stop;
    unordered_map<string_view, const Bus*> u_bus_catalogue;
    unordered_map<string_view, const Stop*> u_stop_catalogue;  
    unordered_map<pair<string_view, string_view>, uint64_t, PairHasher> distance;

    void SetDistanceTC(string_view name, string_view distance_)
    {
        while (true)
        {
            const auto t1 = distance_.find(',');
            string_view str = distance_.substr(0, t1);
            const auto t2 = str.find('m');

            uint64_t result = stoi(string(str.substr(0, t2)));
            string_view stop = str.substr(t2 + 5, str.npos);

            distance[{name, stop}] = move(result);

            if (t1 == distance_.npos)
                break;

            else distance_.remove_prefix(t1 + 2);
        }
    }

    uint64_t GetDistanceTC(string_view from, string_view to)
    {
        if (!distance.count({ from, to }))
            return distance[{ to, from }];
        return distance[{ from, to }];
    }

    const Bus* FindBuses(string_view str)
    {
        return u_bus_catalogue[str];
    }

    const Stop* FindStops(string_view str)
    {
        return u_stop_catalogue[str];
    }
};


vector<string> RequestToTC()
{
    int query_count;
    cin >> query_count;

    vector<string> temp_catalog;
    temp_catalog.reserve(query_count);

    cin.get();
    for (int query = 0; query < query_count; ++query)
    {
        string query_str;
        getline(cin, query_str);
        temp_catalog.push_back(move(query_str));
    }

    return temp_catalog;
}

ostream& operator<<(ostream& os, const In& in)
{
        if (!in.count_stops)
            os << "Bus "s << in.name << ": not found"s << endl;
        else
        {
            os << "Bus "s << in.name << ": "s
                << in.count_stops << " stops on route, "s
                << in.unique_stops << " unique stops, "s
                << in.length << " route length, "s
                << setprecision(6) << in.curvate << " curvature"s
                << endl;
        }

    return os;
}

ostream& operator<<(ostream& os, const Out& out)
{
    os << "Stop "s << out.stop << ": "s;
    bool flag = false;
    for (const auto& bus : out.inf)
    {
        if (flag)
            os << " "s;
        else if (out.inf.size() == 1 && bus[0] != 'n') os << "no "s;
        os << bus;
        flag = true;
    }

    os << endl;

    return os;
}

template<class Struct>
inline void PrintTC(const Struct& in)
{
    cout << in;
}


int main()
{
    LOG_DURATION("speed main"); 
    TransportCatalogue TransCat;
    
    /*const auto& temp_catalog = QueryOnCreateTC();
    
    for (string_view query : temp_catalog)
    {
        if (query[0] == 'S')
            TransCat.SetStopsInTC(query);
        else TransCat.SetRouteInTC(query);
    }

    const auto& ref = RequestToTC();


    for (string_view query : ref)
    {
        if (query[0] == 'S')
            PrintTC(TransCat.GetInformationStopTC(query));
        else PrintTC(TransCat.GetInformationBusTC(query));
    }*/
    
    //
    //
    //                                                     ТАКОЙ СПОСОБ РАБОТАЕТ
    //
    //
    //
    //
    vector<string> vstr =
    {
        {"Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino"},
        {"Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka, 100m to Marushkino"},
        {"Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye"},
        {"Bus 750: Tolstopaltsevo - Marushkino - Marushkino - Rasskazovka"},
        {"Stop Rasskazovka: 55.632761, 37.333324, 9500m to Marushkino"},
        {"Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam"},
        {"Stop Biryusinka: 55.581065, 37.64839, 750m to Universam"},
        {"Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya"},
        {"Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya"},
        {"Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye"},
        {"Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye"},
        {"Stop Rossoshanskaya ulitsa: 55.595579, 37.605757"},
        {"Stop Prazhskaya: 55.611678, 37.603831"}
    };
    for (string_view str : vstr)
    {
        if (str[0] == 'S')
            TransCat.SetStopsInTC(str);
        else TransCat.SetRouteInTC(str);
    }

    vector<string> query_route =
    {
        {"Bus 256"},
        {"Bus 750"},
        {"Bus 751"},
        {"Stop Samara"},
        {"Stop Prazhskaya"},
        {"Stop Biryulyovo Zapadnoye"}
    };

    for (string_view str : query_route)
    {
        if (str[0] == 'S')
            PrintTC(TransCat.GetInformationStopTC(str));
        else PrintTC(TransCat.GetInformationBusTC(str));
    }

    return 0;
}


/*for (auto& [key, value] : TransCat.GetBus())
   {
       cout << key << ": " << endl;

       cout << value->name << ": " << endl;
       for (string_view str : value->up)
       {
           cout << str << " " << str.size() << endl;
       }
       cout << value->unique_stops << endl;
   }

   for (auto& [key, value] : TransCat.GetStops())
   {
       cout << key << ": " << endl;
       cout << value->name << ": " << value->coo.lat << ", " << value->coo.lng << endl;
   }*/