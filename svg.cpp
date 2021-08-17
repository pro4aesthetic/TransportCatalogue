#include "svg.h"

namespace svg {
    ostream& operator<<(ostream& os, const StrokeLineCap& line_cap) {
        if (line_cap == StrokeLineCap::BUTT) os << "butt"sv;
        if (line_cap == StrokeLineCap::SQUARE) os << "square"sv;
        if (line_cap == StrokeLineCap::ROUND) os << "round"sv;
        return os;
    }
    ostream& operator<<(ostream& os, const StrokeLineJoin& line_join) {
        if (line_join == StrokeLineJoin::ARCS) os << "arcs"sv;
        if (line_join == StrokeLineJoin::BEVEL) os << "bevel"sv;
        if (line_join == StrokeLineJoin::MITER) os << "miter"sv;
        if (line_join == StrokeLineJoin::MITER_CLIP) os << "miter-clip"sv;
        if (line_join == StrokeLineJoin::ROUND) os << "round"sv;
        return os;
    }

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();
        RenderObject(context);
        context.out << std::endl;
    }

    // ---------- Circle ------------------

    Circle& Circle::SetCenter(Point center) {
        center_ = center;
        return *this;
    }

    Circle& Circle::SetRadius(double radius) {
        radius_ = radius;
        return *this;
    }

    void Circle::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "  <circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
        out << "r=\""sv << radius_ << "\""sv;
        // Выводим атрибуты, унаследованные от PathProps
        RenderAttrs(out);
        out << "/>"sv;
    }

    Polyline& Polyline::AddPoint(Point point) {
        points_.push_back(point);
        return *this;
    }

    void Polyline::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "  <polyline points=\""sv;
        for (size_t i = 0; i < points_.size(); ++i) {
            out << points_[i].x << ","sv << points_[i].y;
            if (i != points_.size() - 1) out << " "sv;
        }
        out << "\""sv;
        // Выводим атрибуты, унаследованные от PathProps
        RenderAttrs(out);
        out << "/>"sv;
    }

    Text& Text::SetPosition(Point pos) {
        position_ = pos;
        return *this;
    }


    Text& Text::SetOffset(Point offset) {
        offset_ = offset;
        return *this;
    }

    // Задаёт размеры шрифта (атрибут font-size)
    Text& Text::SetFontSize(uint32_t size) {
        font_size_ = size;
        return *this;
    }

    // Задаёт название шрифта (атрибут font-family)
    Text& Text::SetFontFamily(string font_family) {
        font_family_ = font_family;
        return *this;
    }

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& Text::SetFontWeight(string font_weight) {
        font_weight_ = font_weight;
        return *this;
    }

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& Text::SetData(string data) {
        data_ = data;
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "  <text";
        RenderAttrs(out);
        out << " x=\"" << position_.x << "\" y=\"" << position_.y << "\" dx=\""
            << offset_.x << "\" dy=\"" << offset_.y << "\" font-size=\"" << font_size_ << "\"";

        if (!font_family_.empty()) { 
            out << " font-family=\"" << font_family_ << "\""; 
        }
        if (!font_weight_.empty()) { 
            out << " font-weight=\"" << font_weight_ << "\""; 
        }
        // Выводим атрибуты, унаследованные от PathProps
        out << ">" << data_ << "</text>";
    }

    void Document::AddPtr(unique_ptr<Object>&& obj) { 
        objects_.push_back(move(obj)); 
    }

    void Document::Render(ostream& out) const {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << endl;
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << endl;

        for (const auto& object : objects_) { 
            object->Render(out);
        }
        out << "</svg>"sv;
    }

}  // namespace svg