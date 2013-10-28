
// implementation of SmartMemo-0.

namespace sm0 {
	static const int intervals[5] =
		{0,1,7,16,35};

	// interval in days.
	inline int interval(int crct) {
		if (crct < 5) return intervals[crct];
		return (1 << (crct - 4)) * intervals[4];
	}
}
