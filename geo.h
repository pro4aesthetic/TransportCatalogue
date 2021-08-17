#pragma once

#include <cmath>
#include <utility>

using namespace std;

namespace geo {
    inline const double EPSILON = 1e-6;
    struct Coordinates {
        double lat; // Широта
        double lng; // Долгота

        bool operator==(const Coordinates& lhs) const noexcept {
            return pair(fabs(this->lat - lhs.lat), fabs(this->lng - lhs.lng)) < pair(EPSILON, EPSILON);
        }
    };

    bool IsZero(double value);
    double compute_distance(Coordinates from, Coordinates to);

}  // namespace geo