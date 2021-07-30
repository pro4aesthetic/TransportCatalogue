#include "domain.h"
#include "log_duration.h"

namespace detail
{
	bool Buses::find_stop(const string_view& name) const
	{		
		if (count(begin(stops), end(stops), name))
			return true;
		
		return false;
	}

	int Buses::get_unique_stop_count() const
	{
		int counter = 0;

		for (const auto& v : stops)
		{
			int r = count(begin(stops), end(stops), v);
			if (r == 1)  ++counter;
		}
		int cnt = stops.size() - counter;
		int result = cnt / 2;

		return counter + result;
	}

	vector<string_view> Buses::get_stops() const
	{
		if (!is_roundtrip)
		{
			vector<string_view> result(stops);
			copy(make_move_iterator(rbegin(stops) + 1), make_move_iterator(rend(stops)), back_inserter(result));
			return result;
		}

		return stops;		
	}
}