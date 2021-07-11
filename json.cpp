#include "json.h"

using namespace std;

namespace json
{

    namespace
    {

        Node LoadNode(istream& input);

        variant<nullptr_t, Array, Dict, bool, int, double, string> LoadType(istream& input)
        {
            string parsed_num;

            // Считывает в parsed_num очередной символ из input
            auto read_char = [&parsed_num, &input]
            {
                parsed_num += static_cast<char>(input.get());
                if (!input)
                    throw ParsingError("Failed to read number from stream"s);
            };

            // Считывает одну или более цифр в parsed_num из input
            auto read_digits = [&input, read_char]
            {
                if (!isdigit(input.peek()))
                    throw ParsingError("A digit is expected"s);

                while (isdigit(input.peek()))
                    read_char();
            };

            auto read_string_l = [&input, read_char]
            {
                if (!isalnum(input.peek()))
                    throw ParsingError("Failed to read letter from stream"s);

                while (input)
                {
                    if (isalnum(input.peek()))
                        read_char();
                    else break;                    
                }
            };

            auto read_string = [&, read_char]
            {
                if (!isalnum(input.peek()))
                    throw ParsingError("Failed to read letter from stream"s);

                string s1{ "true"s }, s2{ "false"s }, s3{ "null"s };
                int t = 0, f = 0, n = 0, i = -1;
                bool flag = false;

                while (input)
                {
                    if (isalnum(input.peek()))
                    {
                        read_char();
                        ++i;
                        if (s1[t] == parsed_num[i])
                        {
                            if (i == t)
                            {
                                ++t;
                                if (s1.size() - t >= 1)
                                    flag = true;
                                else if (4 == t)
                                    flag = false;
                            }

                            else if (i != t && t > 0)
                                --t;
                        }
                        if (s2[f] == parsed_num[i])
                        {
                            if (i == f)
                            {
                                ++f;
                                if (s2.size() - f >= 1)
                                    flag = true;
                                else if (5 == f)
                                    flag = false;
                            }

                            else if (i != f && f > 0)
                                --f;
                        }
                        if (s3[n] == parsed_num[i])
                        {
                            if (i == n)
                            {
                                ++n;
                                if (s3.size() - n >= 1)
                                    flag = true;
                                else if (4 == n)
                                    flag = false;
                            }

                            else if (i != n && n > 0)
                                --n;
                        }
                    }

                    else
                    {
                        if (flag)
                            throw ParsingError("read string"s);
                        break;
                    }
                }
            };

            auto read_cntrl = [&parsed_num, &input]
            {
                input.ignore();
                switch (input.get())
                {
                case 'n': parsed_num += "\n"s;
                    break;
                case 't': parsed_num += "\t"s;
                    break;
                case 'r': parsed_num += "\r"s;
                    break;
                case 'b': parsed_num += "\b"s;
                    break;
                case 'f': parsed_num += "\f"s;
                    break;
                case 'a': parsed_num += "\a"s;
                    break;
                case 'v': parsed_num += "\v"s;
                    break;
                case '\\': parsed_num += "\\"s;
                    break;
                case '\"': parsed_num += "\""s;
                    break;
                }
            };

            if (input.peek() == ']' || input.peek() == '}')
                throw ParsingError("]}"s);

            else if (input.peek() == '[')
            {
                Array array;

                input.ignore();
                if (input.peek() != EOF)
                {
                    for (char c; input >> c && c != ']';)
                    {
                        if (c == ',' || c == ' ')
                            input.ignore();

                        else
                        {
                            input.putback(c);
                            array.push_back(LoadNode(input));
                        }
                    }
                }

                else
                    throw ParsingError("block array"s);

                return array;
            }
            else if (input.peek() == '{')
            {
                Dict dict;
                bool flag = true;

                input.ignore();
                if (input.peek() != EOF)
                {
                    for (char c; input >> c && c != '}';)
                    {
                        if (c == ',' || c == ' ' || c == ':')
                            input.ignore();
                        else
                        {
                            if (flag)
                            {
                                input.putback(c);
                                parsed_num = LoadNode(input).AsString();
                                flag = false;
                            }
                            else
                            {
                                input.putback(c);
                                dict.insert({ move(parsed_num), LoadNode(input) });
                                flag = true;
                            }
                        }
                    }
                }

                else
                    throw ParsingError("block dict"s);

                return dict;
            }
            else
            {
                if (isdigit(input.peek()) || input.peek() == '-')
                {
                    if (input.peek() == '-')
                        read_char();

                    // Парсим целую часть числа
                    if (input.peek() == '0')
                        read_char();
                    // После 0 в JSON не могут идти другие цифры
                    else
                        read_digits();

                    bool is_int = true;
                    // Парсим дробную часть числа
                    if (input.peek() == '.')
                    {
                        read_char();
                        read_digits();
                        is_int = false;
                    }

                    // Парсим экспоненциальную часть числа
                    if (int ch = input.peek(); ch == 'e' || ch == 'E')
                    {
                        read_char();
                        if (ch = input.peek(); ch == '+' || ch == '-')
                            read_char();

                        read_digits();
                        is_int = false;
                    }

                    try
                    {
                        if (is_int)
                        {
                            // Сначала пробуем преобразовать строку в int
                            try
                            {
                                return stoi(parsed_num);
                            }
                            catch (...)
                            {
                                // В случае неудачи, например, при переполнении
                                // код ниже попробует преобразовать строку в double
                            }
                        }
                        return stod(parsed_num);
                    }
                    catch (...)
                    {
                        throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
                    }
                }
                else
                {
                    int cntrl = 0;
                    if (input.peek() == '"')
                    {
                        while (input)
                        {
                            if (input.peek() == '"')
                            {
                                ++cntrl;
                                input.ignore();
                                if (cntrl % 2 == 0)
                                    break;
                            }
                            else if (input.peek() == '\\')
                                read_cntrl();
                            else if (input.peek() == ' ' || ispunct(input.peek()))
                                read_char();
                            else if (isalnum(input.peek()))
                                read_string_l();
                            else input.ignore();
                        }

                        if (cntrl % 2 != 0)
                            throw ParsingError("block string"s);
                        
                        return parsed_num;
                    }
                    else
                    {
                        if (input.peek() == '\\')
                            read_cntrl();
                        else if (input.peek() == ' ' || ispunct(input.peek()))
                            read_char();
                        else if (isalnum(input.peek()))
                            read_string();
                        else input.ignore();

                        if (parsed_num == "null"s)
                            return nullptr;

                        else if (parsed_num == "true"s)
                            return true;

                        else if (parsed_num == "false"s)
                            return false;

                        else return parsed_num;
                    }
                }
            }
        }

        Node LoadNode(istream& input)
        {
            try
            {
                return Node(move(LoadType(input)));
            }
            catch (ParsingError&)
            {
                throw ParsingError("LoadNode error"s);
            }
        }

    }  // namespace

    const Array& Node::AsArray() const
    {
        if (!IsArray())
            throw logic_error("Not an array"s);
        return get<Array>(*this);
    }

    const Dict& Node::AsMap() const
    {
        if (!IsMap())
            throw logic_error("Not an map"s);
        return get<Dict>(*this);
    }

    int Node::AsInt() const
    {
        if (!IsInt())
            throw logic_error("Not an int"s);
        return get<int>(*this);
    }

    const string& Node::AsString() const
    {
        if (!IsString())
            throw logic_error("Not an string"s);
        return get<string>(*this);
    }

    double Node::AsDouble() const
    {
        try
        {
            if (IsInt())
                return static_cast<double>(get<int>(*this));
            return get<double>(*this);
        }
        catch (...)
        {
            throw logic_error("Not an double"s);
        }
    }

    bool Node::AsBool() const
    {
        if (!IsBool())
            throw logic_error("Not an bool"s);
        return get<bool>(*this);
    }

    bool Node::IsNull() const
    {
        return holds_alternative<nullptr_t>(*this);
    }
    bool Node::IsArray() const
    {
        return holds_alternative<Array>(*this);
    }
    bool Node::IsMap() const
    {
        return holds_alternative<Dict>(*this);
    }
    bool Node::IsInt() const
    {
        return holds_alternative<int>(*this);
    }
    bool Node::IsDouble() const
    {
        if (IsInt())
            return true;
        return holds_alternative<double>(*this);
    }
    bool Node::IsString() const
    {
        return holds_alternative<string>(*this);
    }
    bool Node::IsBool() const
    {
        return holds_alternative<bool>(*this);
    }
    bool Node::IsPureDouble() const
    {
        if (IsInt())
            return false;
        return holds_alternative<double>(*this);
    }

    const Node& Document::GetRoot() const
    {
        return root_;
    }

    Document Load(istream& input)
    {
        return Document{ LoadNode(input) };
    }

    ostream& operator<<(ostream& output, const Node& node)
    {
        if (node.IsNull())
            output << "null"s;
        else if (node.IsArray())
        {
            bool flag = false;
            output << "["s;
            for (const auto& noded : node.AsArray())
            {
                if (flag)
                    output << ", "s;
                output << noded;
                flag = true;
            }
            output << "]"s;
        }
        else if (node.IsMap())
        {
            bool flag = false;
            output << "{"s << endl;
            for (const auto& [key, noded] : node.AsMap())
            {
                if (flag)
                    output << ","s << endl;
                output << "\""s << key << "\""s << ": "s << noded;
                flag = true;
            }
            output << endl << "}"s;
        }
        else if (node.IsBool())
            output << boolalpha << node.AsBool();
        else if (node.IsInt())
            output << node.AsInt();
        else if (node.IsDouble())
            output << node.AsDouble();
        else if (node.IsString())
        {
            output << "\""s;
            for (const char c : node.AsString())
            {
                switch (c)
                {
                case '\"': output << "\\\""s;
                    break;
                case '\\': output << "\\\\"s;
                    break;
                case '\n': output << "\\n"s;
                    break;
                case '\r': output << "\\r"s;
                    break;
                case '\t': output << "\\t"s;
                    break;
                case '\b': output << "\\b"s;
                    break;
                case '\f': output << "\\f"s;
                    break;
                case '\a': output << "\\a"s;
                    break;
                case '\v': output << "\\v"s;
                    break;
                case '\'': output << "\\\'"s;
                    break;
                case '\?': output << "\\\?"s;
                    break;
                default: output << c;
                }
            }
            output << "\""s;
        }
        return output;
    }

    void Print(const Document& doc, ostream& output)
    {
        output << doc.GetRoot();
    }

}  // namespace json
