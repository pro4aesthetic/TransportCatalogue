#include "json_reader.h"
#include "request_handler.h"

#include <fstream>

using namespace jsonreader;
using namespace requesthandler;

void run_system(istream& input, ostream& output)
{
    JsonReader transport;
    transport.parse_requests(input);

    RequestHandler requests(transport);
    transport.print(requests, output);
}

int main()
{
    LOG_DURATION("program speed..."s);
    
    ifstream input("big_test_without_map.json", ios_base::in);
    ofstream output("big_test_without_map_result_2.json", ios_base::out);

    run_system(input, output);
    //ifstream input("test.json", ios_base::in);

    //run_system(input, cout);
    input.close();
    output.close();
    
    return 0;
}
