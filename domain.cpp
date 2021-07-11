#include "domain.h"
#include <iostream>

namespace detail
{
	bool buses::find_stop(const string& str) const
	{
		auto it = find(begin(stops), end(stops), str);

		if (it != end(stops))
			return true;

		return false;
	}

	int buses::get_stop_count() const
	{
		if (!stops.size()) return 0;

		return get_stops().size();
	}

	vector<string> buses::get_stops() const
	{
		if (!stops.size()) return stops;

		else
		{
			vector<string> result(stops);

			if (!is_roundtrip)
				copy(rbegin(stops) + 1, rend(stops), back_inserter(result));

			return result;
		}
	}
}