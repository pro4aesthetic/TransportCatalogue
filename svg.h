#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <optional>
#include <variant>
#include <sstream>

using namespace std;

namespace svg 
{
    struct Point 
    {
        Point() = default;
        Point(double x, double y)
            : x(x)
            , y(y) {
        }
        double x = 0;
        double y = 0;
    };

    struct RenderContext 
    {
        RenderContext(ostream& out)
            : out(out) {
        }

        RenderContext(ostream& out, int indent_step, int indent = 0)
            : out(out)
            , indent_step(indent_step)
            , indent(indent) {
        }

        RenderContext Indented() const { 
            return { out, indent_step, indent + indent_step }; 
        }

        void RenderIndent() const { 
            for (int i = 0; i < indent; ++i) 
                out.put(' '); 
        }

        ostream& out;
        int indent_step = 0;
        int indent = 0;
    };

    class Object 
    {
    public:
        void Render(const RenderContext& context) const;
        virtual ~Object() = default;

    private:
        virtual void RenderObject(const RenderContext& context) const = 0;
    };

    struct Rgb
    {
        Rgb() = default;
        Rgb(uint8_t red_, uint8_t green_, uint8_t blue_)
            : red(red_)
            , green(green_)
            , blue(blue_) {
        }

        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
    };

    struct Rgba
    {
        Rgba() = default;
        Rgba(uint8_t red_, uint8_t green_, uint8_t blue_, double opacity_)
            : red(red_)
            , green(green_)
            , blue(blue_)
            , opacity(opacity_) {
        }

        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        double opacity = 1.0;
    };

    using Color = variant<monostate, string, Rgb, Rgba>;

    // Объявив в заголовочном файле константу со спецификатором inline,
    // мы сделаем так, что она будет одной на все единицы трансляции,
    // которые подключают этот заголовок.
    // В противном случае каждая единица трансляции будет использовать свою копию этой константы
    inline const Color NoneColor{ "none" };

    enum class StrokeLineCap 
    {
        BUTT,
        ROUND,
        SQUARE
    };

    enum class StrokeLineJoin 
    {
        ARCS,
        BEVEL,
        MITER,
        MITER_CLIP,
        ROUND
    };

    struct SolutionPath
    {
        ostream& out;
        void operator()(monostate) const { out << "none"; }
        void operator()(const string& color) const { out << color; }
        void operator()(const Rgb& color) const
        {
            out << "rgb(" << static_cast<int>(color.red) << "," << static_cast<int>(color.green) << "," << static_cast<int>(color.blue) << ")";
        }
        void operator()(const Rgba& color) const
        {
            out << "rgba(" << static_cast<int>(color.red) << "," << static_cast<int>(color.green) << "," << static_cast<int>(color.blue) << "," << color.opacity << ")";
        }
    };

    ostream& operator<<(ostream& os, const StrokeLineCap& line_cap);
    ostream& operator<<(ostream& os, const StrokeLineJoin& line_join);

    template <typename Owner>
    class PathProps
    {
    public:
        Owner& SetFillColor(Color color)
        {
            fill_color_ = move(color);
            return AsOwner();
        }
        Owner& SetStrokeColor(Color color)
        {
            stroke_color_ = move(color);
            return AsOwner();
        }
        Owner& SetStrokeWidth(double width)
        {
            width_ = move(width);
            return AsOwner();
        }
        Owner& SetStrokeLineCap(StrokeLineCap line_cap)
        {
            line_cap_ = move(line_cap);
            return AsOwner();
        }
        Owner& SetStrokeLineJoin(StrokeLineJoin line_join)
        {
            line_join_ = move(line_join);
            return AsOwner();
        }

    protected:
        ~PathProps() = default;
        void RenderAttrs(ostream& out) const
        {
            if (fill_color_)
            {
                out << " fill=\"";
                std::visit(SolutionPath{ out }, *fill_color_);
                out << "\"";
            }

            if (stroke_color_)
            {
                out << " stroke=\"";
                std::visit(SolutionPath{ out }, *stroke_color_);
                out << "\"";
            }

            if (width_) out << " stroke-width=\"" << *width_ << "\"";
            if (line_cap_) out << " stroke-linecap=\"" << *line_cap_ << "\"";
            if (line_join_) out << " stroke-linejoin=\"" << *line_join_ << "\"";
        }

    private:
        Owner& AsOwner()
        {
            // static_cast безопасно преобразует *this к Owner&,
            // если класс Owner — наследник PathProps
            return static_cast<Owner&>(*this);
        }

        optional<Color> fill_color_;
        optional<Color> stroke_color_;
        optional<double> width_;
        optional<StrokeLineCap> line_cap_;
        optional<StrokeLineJoin> line_join_;
    };

    class Circle final : public Object, public PathProps<Circle>
    {
    public:
        Circle& SetCenter(Point center);
        Circle& SetRadius(double radius);

    private:
        void RenderObject(const RenderContext& context) const override;
        Point center_;
        double radius_ = 1.0;
    };

    class Polyline final : public Object, public PathProps<Polyline>
    {
    public:
        // Добавляет очередную вершину к ломаной линии
        Polyline& AddPoint(Point point);

    private:
        vector<Point> points_;
        void RenderObject(const RenderContext& context) const override;
    };

    class Text final : public Object, public PathProps<Text>
    {
    public:
        Text& SetPosition(Point pos);

        // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
        Text& SetOffset(Point offset);

        // Задаёт размеры шрифта (атрибут font-size)
        Text& SetFontSize(uint32_t size);

        // Задаёт название шрифта (атрибут font-family)
        Text& SetFontFamily(string font_family);

        // Задаёт толщину шрифта (атрибут font-weight)
        Text& SetFontWeight(string font_weight);

        // Задаёт текстовое содержимое объекта (отображается внутри тега text)
        Text& SetData(string data);

    private:
        Point position_;
        Point offset_;
        string font_family_;
        string data_;
        uint32_t font_size_ = 1;
        string font_weight_;
        void RenderObject(const RenderContext& context) const override;
    };

    class ObjectContainer
    {
    public:
        ObjectContainer() = default;
        template <typename T>
        void Add(T obj) { AddPtr(move(make_unique<T>(obj))); }
        virtual void AddPtr(unique_ptr<Object>&& obj) = 0;

    protected:
        ~ObjectContainer() = default;
    };

    // Интерфейс Drawable задаёт объекты, которые можно нарисовать с помощью Graphics
    class Drawable
    {
    public:
        Drawable() = default;
        virtual ~Drawable() = default;
        virtual void Draw(ObjectContainer& container) const = 0;
    };

    class Document final : public ObjectContainer
    {
    public:
        void AddPtr(unique_ptr<Object>&& obj) override;
        void Render(ostream& out) const;

    private:
        vector<unique_ptr<Object>> objects_;
    };
}