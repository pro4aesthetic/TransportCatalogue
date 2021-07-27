#include "json_reader.h"
#include <chrono>

int main()
{    
    const auto start = chrono::steady_clock::now();
    
    const Node& node = parse_requests();
    JsonReader transport(node.AsMap().at("base_requests"s));
    transport.push_base_requests();
    
    RequestHandler rqhandler(transport);
    result_requests(rqhandler, node.AsMap().at("stat_requests"s));

    const auto duration = chrono::steady_clock::now() - start;
    cout << chrono::duration_cast<chrono::milliseconds>(duration).count() << "ms"sv << endl;
    return 0;

    /*
     * Примерная структура программы:
     *
     * Считать JSON из stdin
     * Построить на его основе JSON базу данных транспортного справочника
     * Выполнить запросы к справочнику, находящиеся в массива "stat_requests", построив JSON-массив
     * с ответами Вывести в stdout ответы в виде JSON
     */
}
