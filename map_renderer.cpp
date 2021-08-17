#include "map_renderer.h"

namespace renderer {
	void MapRenderer::render_road(const set<pair<string_view, const Bus*>>& order_bus
		, const SphereProjector& sprojector
		, svg::Document& doc) const {
		int counter = 0;
		vector<Coordinates> points;

		for (const auto& [_, value] : order_bus) {
			if (counter == settings.color_palette.size()) {
				counter = 0;
			}

			if (value->stops.size() > 0) {
				vector<string_view> renderer = value->get_stops();
				for_each(begin(renderer), end(renderer), [&]
				(const string_view& name) {
						points.emplace_back(u_stop_catalogue.at(name)->coordinates);
					});

				Polyline polyline;
				for (auto it = begin(points); it != end(points); ++it) {
					Point point = sprojector(*it);
					polyline.AddPoint(point);
				}

				doc.Add(Polyline{ polyline }
					.SetFillColor("none")
					.SetStrokeColor(settings.color_palette[counter])
					.SetStrokeWidth(settings.line_width)
					.SetStrokeLineJoin(StrokeLineJoin::ROUND)
					.SetStrokeLineCap(StrokeLineCap::ROUND));
				++counter;
				points.clear();
			}
			else continue;

		}
	}

	void MapRenderer::render_bus_name(const set<pair<string_view, const Bus*>>& order_bus
		, const SphereProjector& sprojector
		, svg::Document& doc) const {
		int counter = 0;
		vector<Coordinates> points;

		auto read_text = [&](const Point& point, const string& name) {
			Text text;
			text.SetPosition(point)
				.SetOffset(settings.bus_label_offset)
				.SetFontSize(settings.bus_label_font_size)
				.SetFontFamily("Verdana")
				.SetFontWeight("bold")
				.SetData(name);

			doc.Add(Text{ text }
				.SetFillColor(settings.underlayer_color)
				.SetStrokeColor(settings.underlayer_color)
				.SetStrokeWidth(settings.underlayer_width)
				.SetStrokeLineJoin(StrokeLineJoin::ROUND)
				.SetStrokeLineCap(StrokeLineCap::ROUND));
			doc.Add(Text{ text }.SetFillColor(settings.color_palette[counter]));
		};

		for (const auto& [_, value] : order_bus) {
			if (counter == settings.color_palette.size()) {
				counter = 0;
			}

			if (value->stops.size() > 0) {
				for_each(begin(value->stops), end(value->stops), [&]
				(const string_view& name) {
					points.emplace_back(u_stop_catalogue.at(name)->coordinates);
				});
			

				for (auto it = begin(points); it != end(points); ++it) {
					Point point = sprojector(*it);
					if (points.front() == points.back()) {
						if (it == begin(points)) {
							read_text(point, string(value->name));
						}
					}
					else {
						if (it == begin(points) || it == end(points) - 1) {
							read_text(point, string(value->name));
						}
					}
				}
				++counter;
				points.clear();
			}
			else continue;
		}
	}

	void MapRenderer::render_stop(const set<pair<string_view, const Bus*>>& order_bus
		, const SphereProjector& sprojector
		, svg::Document& doc) const {
		vector<Coordinates> points;
		set<string_view> render;

		auto read_circle = [&](const Point& point) {
			Circle circle;
			doc.Add(circle
				.SetCenter(point)
				.SetRadius(settings.stop_radius)
				.SetFillColor("white"));
		};
		
		for (const auto& [_, value] : order_bus) {
			for_each(begin(value->stops), end(value->stops), [&]
			(const string_view& name) {
					render.emplace(name);
				});
		}

		for_each(begin(render), end(render), [&]
		(const string_view& name) {
				points.emplace_back(u_stop_catalogue.at(name)->coordinates);
			});

		for (auto it = begin(points); it != end(points); ++it) {
			Point point = sprojector(*it);
			read_circle(point);
		}
	}

	void MapRenderer::render_stop_name(const set<pair<string_view, const Bus*>>& order_bus
		, const SphereProjector& sprojector
		, svg::Document& doc) const {
		vector<pair<string, Coordinates>> points;
		set<string_view> render;

		auto read_text = [&](const Point& point, const string& name) {
			Text text;
			text.SetPosition(point)
				.SetOffset(settings.stop_label_offset)
				.SetFontSize(settings.stop_label_font_size)
				.SetFontFamily("Verdana")
				.SetData(name);

			doc.Add(Text{ text }
				.SetFillColor(settings.underlayer_color)
				.SetStrokeColor(settings.underlayer_color)
				.SetStrokeWidth(settings.underlayer_width)
				.SetStrokeLineJoin(StrokeLineJoin::ROUND)
				.SetStrokeLineCap(StrokeLineCap::ROUND));
			doc.Add(Text{ text }.SetFillColor("black"));
		};

		for (const auto& [_, value] : order_bus) {
			for_each(begin(value->stops), end(value->stops), [&]
			(const string_view& name) {
					render.emplace(name);
				});			
		}

		for_each(begin(render), end(render), [&]
		(const string_view& name) {
				points.emplace_back(name, u_stop_catalogue.at(name)->coordinates);
			});

		for (const auto& [name, coordinates] : points) {
			Point point = sprojector(coordinates);
			read_text(point, name);
		}
	}

	svg::Document MapRenderer::get_settings() const {
		svg::Document doc;
		set<pair<string_view, const Bus*>> order_bus(begin(u_bus_catalogue), end(u_bus_catalogue));
		vector<Coordinates> points;

		for (const auto& [_, value] : order_bus) {
			for_each(begin(value->stops), end(value->stops), [&]
			(const string_view& name) {
					points.emplace_back(u_stop_catalogue.at(name)->coordinates);
				});
		}
		SphereProjector sprojector(begin(points), end(points), settings.width, settings.height, settings.padding);
		
		render_road(order_bus, sprojector, doc);
		render_bus_name(order_bus, sprojector, doc);
		render_stop(order_bus, sprojector, doc);
		render_stop_name(order_bus, sprojector, doc);
		return doc;
	}
}