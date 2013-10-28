

static int intervals[] = {0,1,6};


struct ItemBase {
	double EF = 2.5;
	int crct = 0;
	int I = 0;

	int nextI() const {
		if (crct < 3)
			return intervals[crct];
		else
			return I * EF + 0.5;
	}

	/*
	  5 - perfect response
	  4 - correct response after a hesitation
	  3 - correct response recalled with serious difficulty
	  2 - incorrect response; where the correct one seemed easy to recall
	  1 - incorrect response; the correct one remembered
	  0 - complete blackout.
	*/
	virtual int getResponce() const = 0;

	// returns whether re-testing is needed.
	bool test() {
		int q = getResponce();
		EF = EF-0.8+0.28*q-0.02*q*q;
		crct++;
		if (q < 3) crct = 0;
		I = nextI();
		return q < 4;
	}

	bool retest() const {
		return getResponce() < 4;
	}
}
