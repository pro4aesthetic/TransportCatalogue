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