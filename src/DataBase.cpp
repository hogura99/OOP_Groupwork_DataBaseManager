void DataBase::createTable(const std::string &command) {
	using namespace std;
	vector<string> param, attrType;
	vector<bool> not_null;
	string pri_key;
	auto res = ParamSpliter::split_createTable(command, param, not_null, pri_key);
	assert(res == TABLE_CREATE); // !!!

	for (int i = 1; i < param.size(); i += 2)
		attrType.push_back(make_pair(param[i], param[i + 1]));
	mTable[param[0]] = new DataTable(param[0], attrType, not_null);
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
			// xq should complete this showCol()
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

void DataBase::insertData(const std::vector<std::string> &param)
{
	if (mTable.count(param[0]) || param.size() % 2 == 1)
	{
		auto _table = mTable[ param[0] ];
		std::vector<ATTRIBUTE> _attributes;
		for (int i = 1; i < param.size(); i += 2)
		{
			auto _attr_type = _table->getTypename(param[i]) ; // pay attention to the legality
			Base *pt = NULL;
			switch (_attr_type)
			{
				case DataTable::INT :
					int val = 0;
					if (myStr2Int(param[i + 1], val))
						pt = new dataInt(val);
					break;
				case DataTable::DOUBLE :
					double val = 0;
					if (myStr2Double(param[i + 1], val))
						pt = new dataDouble(val);
					break;
				case DataTable::STRING :
					pt = new dataString(param[i + 1]);
					break;
			}
			if (pt != NULL)
			{
				_attributes.push_back( ATTRIBUTE(param[i], pt) );
			}
		}
		_table.insert(_attributes);
	}
	else
	{
		// show insert error
	}
}