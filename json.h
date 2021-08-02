#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cctype>
#include <variant>
#include <tuple>

using namespace std;

namespace json
{

    class Node;
    using Dict = map<string, Node>;
    using Array = vector<Node>;
    using Type = variant<nullptr_t, Array, Dict, bool, int, double, string>;

    // Эта ошибка должна выбрасываться при ошибках парсинга JSON
    class ParsingError : public runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class Node final : private Type
    {
    public:
        using variant::variant;

        Node(variant v) : variant(v) {
        }

        const Array& AsArray() const;
        const Dict& AsMap() const;
        int AsInt() const;
        const string& AsString() const;
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

        bool IsNull() const { return holds_alternative<nullptr_t>(*this); }
        bool IsArray() const { return holds_alternative<Array>(*this); }
        bool IsMap() const { return holds_alternative<Dict>(*this); }
        bool IsInt() const { return holds_alternative<int>(*this); }
        bool IsDouble() const;
        bool IsString() const { return holds_alternative<string>(*this); }
        bool IsBool() const { return holds_alternative<bool>(*this); }
        bool IsPureDouble() const;
    };

    class Document
    {
    public:
        explicit Document(Node root) : root_(move(root)) {
        }

        const Node& GetRoot() const { return root_; }
        bool operator==(const Document& other) const { return root_ == other.root_; }
        bool operator!=(const Document& other) const { return root_ != other.root_; }

    private:
        Node root_;
    };

    Document Load(std::istream& input);
    void Print(const Document& doc, ostream& output);

}  // namespace json
