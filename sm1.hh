

static int intervals[] = {0,1,6};


// assumes EF, I, crct as columns in table `items'
// nexpdate

namespace CIndex {
	enum {rowid, EF, I, crct, nexpdate};
}



struct ItemBase {
	void make(const std::unique_ptr<sqlite3> &db) {
		sqlite3::exec(db, "insert into items(EF,I,crct) values(2.5, 0, 0)");
	}


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

	void test(const std::unique_ptr<sqlite3> &db) {
		std::unique_ptr<sqlite3_stmt> stmt =
			sqlite3::prepare
			(db, "select rowid from items where nexpdate <= date('now')");

		std::vector<int> rowids;
		while(sqlite3_step(stmt.get()) == SQLITE_ROW)
			rowids.push_back(sqlite3_column_int(stmt.get(),0));

		std::vector<int> retest_ids;
		for (int rowid : rowids)
			if(test(db, rowid))
				retest_ids.push_back(rowid);

		while(!retest_ids.empty()) {
			std::vector<int> result_ids;
			for (int rowid : result_ids)
				if (retest(rowid))
					result_ids.push_back(rowid);
			retest_ids.swap(result_ids);
		}
	}

	// returns whether re-testing is needed.
	bool test(const std::unique_ptr<sqlite3> &db, int rowid) {
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
