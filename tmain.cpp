#include "json_reader.h"
#include "log_duration.h"
#include "request_handler.h"

#include <fstream>

void run_system(istream& input, ostream& output)
{
    JsonReader transport;
    transport.parse_requests(input);
    //input.clear();

    RequestHandler requests(transport);
    transport.print(requests, output);
    //output.clear();
}

int main()
{
    /*
     * Примерная структура программы:
     *
     * Считать JSON из stdin
     * Построить на его основе JSON базу данных транспортного справочника
     * Выполнить запросы к справочнику, находящиеся в массива "stat_requests", построив JSON-массив
     * с ответами Вывести в stdout ответы в виде JSON
     */

    LOG_DURATION("program speed..."s);

    ifstream input("big_test_without_map.json", ios_base::in);
    ofstream output("big_test_without_map_result.json", ios_base::out | ios_base::app);

    run_system(input, output);
    input.close();
    output.close();
    
    return 0;
}
