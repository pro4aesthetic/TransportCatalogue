#include "domain.h"

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
		unordered_set<string_view> st(begin(stops), end(stops));
		return st.size();
	}

	vector<string_view> Buses::get_stops() const
	{
		if (!is_roundtrip && stops.size())
		{
			vector<string_view> result(stops);
			copy(make_move_iterator(rbegin(stops) + 1), make_move_iterator(rend(stops)), back_inserter(result));
			return result;
		}

		return stops;		
	}
}
