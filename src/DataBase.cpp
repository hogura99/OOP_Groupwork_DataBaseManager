void DataBase::createTable(const std::string &command) {
	std::vector<std::string> param;
	std::vector<bool> not_null;
	std::string pri_key;
	ParamSpliter::SplitTC(command, param, not_null, pri_key);
	// !!! ----> issue 1 !!!
}

void DataBase::dropTable(const std::string &Name) {
	try {
		auto it = mTable.find(Name);
		if (it != mTable.end()) {
			delete it.second;
			mTable.erase(it);
		} else
			throw false;
	}
	catch (bool) {
		// deletion error
	}
}

void DataBase::showTable(const std::string &Name) {
	try {
		auto it = mTable.find(Name);
		if (it != mTable.end()) {
			it.second->showCol(); // ? list each columns of table ? what is this ?
		} else
			throw(false);
	}
	catch (bool) {
		// show table error
	}
}

void DataBase::showTableAll() { // temporarily function
	for (auto it: mTable) {
		cout << it.first << endl;
	}
}

void DataBase::insertData(const std::vector<std::string> &param) {
	if (mTable.count(param[0])) {
		auto pt = mTable[ param[0] ];
		// ----> !!! issue 1 !!!
	} else {
		// show insert error
	}
}