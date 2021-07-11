#pragma once

#include <sstream>
#include <algorithm>
#include "json.h"
#include "request_handler.h"

using namespace json;

using result_parse = tuple<save_inf, save_inf>;

result_parse info();
result_parse parse(istream&);
void answer_handler(TransportCatalogue&, const save_inf&);