/*#include ...

// JS
// db.etv...

// ��������
var time_from_on = 0;

// 1/sec.
function onRequestParams() {
	if (time_from_on === 7) {
	  db.etv = 42;
	}
	time_from_on++;
}

function onRequestCfg() {
	// Nothing
}


//@NoThreadSafe
TEST(DatabaseMan, ETV) {
	// ����������
	Database* db = new Database();

	// To V8
	v8_wrapper = V8Wrapper(db, model_file_name);

	/// Work
	// Change state
	for (int i = 0; i < 8; i++) {
		v8_wrapper.requestCfg(); //������ JS
		v8_wrapper.requestParams();

		// Make pkgs
		vector<Slot> config_pkg = db->emitCfg();  // vector<Slog> emitCfg const {}
		vector<Slot> params_pkg = db->emitAll();
	}
	delete db;
}*/