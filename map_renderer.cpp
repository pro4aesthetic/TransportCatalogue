#include "map_renderer.h"

namespace renderer {
	svg::Document MapRenderer::get_settings() const {
		for (const auto& [key, value] : u_bus_catalogue) {
			auto stop = find_stops(value->get_stops());
		}
		return svg::Document{};
	}
}