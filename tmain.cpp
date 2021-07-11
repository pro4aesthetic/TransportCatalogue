#include "json_reader.h"

int main()
{    
    auto [base_requests, stat_requests] = info();
    
    TransportCatalogue transport;
    handler(transport, base_requests);

    answer_handler(transport, stat_requests);

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
