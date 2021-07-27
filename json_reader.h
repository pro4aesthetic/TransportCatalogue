#pragma once

#include <sstream>
#include <algorithm>
#include "json.h"
#include "request_handler.h"

using namespace json;

class JsonReader : public TransportCatalogue
{
public:
	JsonReader() = default;
	JsonReader(const Node& nd_)
		: nd(nd_) {
	}
	~JsonReader() = default;

	void push_base_requests();

private:	
	const Node& nd;
};

Node parse_requests();
void result_requests(const RequestHandler&, const Node&);