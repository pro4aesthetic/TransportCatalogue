#include "domain.h"

namespace detail
{
	bool buses::find_stop(string_view str) const
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

	vector<string_view> buses::get_stops() const
	{		
		if (!stops.size()) return stops;
		
		else
		{
			vector<string_view> result(stops);

			if (!is_roundtrip)
				copy(make_move_iterator(rbegin(stops) + 1), make_move_iterator(rend(stops)), back_inserter(result));

			return result;
		}
	}
}