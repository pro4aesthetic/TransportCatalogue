#pragma once

#include "json.h"
#include "request_handler.h"

#include <memory>

using namespace json;

class JsonReader final : public TransportCatalogue
{
public:
	explicit JsonReader() = default;
	~JsonReader() = default;

	void parse_requests(istream&) override;
	void print(const RequestHandler&, ostream&);

private:	
	unique_ptr<Node> nd;
	void push_requests() override;
};