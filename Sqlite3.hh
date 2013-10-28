
#include <sqlite3>
#include <memory>


namespace sqlite3 {

	std::unique_ptr<sqlite3> open(const char *filename) {
		sqlite3 *p;
		// todo: how the return code should be handled.
		sqlite3_open(filename, &p);

		return
			std::unique_ptr<sqlite3>
			(p, [](sqlite3 *p_del)
			 { sqlite3_close_v2(p_del); });
	}

	std::unique_ptr<sqlite3_stmt> prepare(std::unique_ptr<sqlite3> db, const char *sql) {
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2
			(db.get(),
			 sql,
			 -1,
			 &stmt,
			 nullptr);

		return
		ptr.reset(stmt, [](sqlite3_stmt *stmt_del){
				sqlite3_finalize(stmt_del);
			});
	}
}
