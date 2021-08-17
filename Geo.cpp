#define _USE_MATH_DEFINES
#include "geo.h"

namespace geo {
    bool IsZero(double value) {
        return abs(value) < EPSILON;
    }

    double compute_distance(Coordinates from, Coordinates to) {
        using namespace std;
        const double dr = M_PI / 180.0;
        return acos(sin(from.lat * dr) * sin(to.lat * dr)
            + cos(from.lat * dr) * cos(to.lat * dr) * cos(abs(from.lng - to.lng) * dr))
            * 6371000;
    }
}  // namespace geo
