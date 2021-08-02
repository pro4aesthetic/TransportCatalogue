#pragma once

namespace geo 
{
    struct Coordinates 
    {
        double lat; // Широта
        double lng; // Долгота
    };

    double compute_distance(Coordinates from, Coordinates to);

}  // namespace geo
/*
#pragma once

#include <cmath>

namespace transport_catalogue {

    namespace detail {

        struct Coordinates {
            double lat;
            double lng;
        };

        inline double ComputeDistance(Coordinates from, Coordinates to) {
            using namespace std;
            static const double dr = 3.1415926535 / 180.;
            return acos(sin(from.lat * dr) * sin(to.lat * dr)
                + cos(from.lat * dr) * cos(to.lat * dr) * cos(abs(from.lng - to.lng) * dr))
                * 6371000;
        }

    }

}*/