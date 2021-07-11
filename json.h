#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cctype>
#include <variant>
#include <tuple>

namespace json
{

    class Node;
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;

    // Эта ошибка должна выбрасываться при ошибках парсинга JSON
    class ParsingError : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class Node final
        : private std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>
    {
    public:
        using variant::variant;

        Node(variant v)
            : variant(v) {
        }

        const Array& AsArray() const;
        const Dict& AsMap() const;
        int AsInt() const;
        const std::string& AsString() const;
        double AsDouble() const;
        bool AsBool() const;

        bool operator==(const Node& other) const
        {           
            return std::tuple(AsArray(), AsMap(), AsInt(), AsString(), AsDouble(), AsBool()) ==
                std::tuple(other.AsArray(), other.AsMap(), other.AsInt(), other.AsString(), other.AsDouble(), other.AsBool());
        }
        bool operator!=(const Node& other) const
        {
            return std::tuple(AsArray(), AsMap(), AsInt(), AsString(), AsDouble(), AsBool()) != 
                std::tuple(other.AsArray(), other.AsMap(), other.AsInt(), other.AsString(), other.AsDouble(), other.AsBool());
        }

        bool IsNull() const;
        bool IsArray() const;
        bool IsMap() const;
        bool IsInt() const;
        bool IsDouble() const;
        bool IsString() const;
        bool IsBool() const;
        bool IsPureDouble() const;
    };

    class Document
    {
    public:
        explicit Document(Node root)
            : root_(move(root)) {
        }

        const Node& GetRoot() const;
        bool operator==(const Document& other) const
        {
            return root_ == other.root_;
        }
        bool operator!=(const Document& other) const
        {
            return root_ != other.root_;
        }

    private:
        Node root_;
    };

    Document Load(std::istream& input);

    void Print(const Document& doc, std::ostream& output);

}  // namespace json
