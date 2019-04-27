#include "DataTable.h"
#define NAME first
#define BASE second

/*
    未完成任务：
        checkLegality的封装问题
        notNull的检查
        update的合法性检查
        若干处红标未解决
*/

DataTable::DataTable(const std::string &_tableName, std::vector< std::pair<std::string, int> >& _attrTable, const std::string& _primaryKey, const std::vector<std::string>& _notNullKey)
: tableName(_tableName), primaryKey(_primaryKey)
{
    for (auto iter = _attrTable.begin(); iter != _attrTable.end(); iter++)
    {
        notNullKey[iter->first] = false;
        attrTable[iter->first] = iter->second;

    }
    for (auto iter = _notNullKey.begin(); iter != _notNullKey.end(); iter++)
    {
        notNullKey[*iter] = true;
    }
}


bool checkLegality(DataTable _DataTable, const std::vector<ATTRIBUTE>& attributes)
{
    bool flag = true;   //函数返回值

    //检查主键
    const Base* primaryKeyValue;
    std::map<std::string, bool> checkNotNull;
    for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
    {
        if (iter->NAME == _DataTable.primaryKey)
            primaryKeyValue = &iter->BASE;
    }
    bool _exist = false;
    for (auto iter = _DataTable.mData.begin(); iter != _DataTable.mData.end(); iter++)   //iter不能大小比较
    {
        if ((*iter)->getData(_DataTable.primaryKey) == primaryKeyValue)
            _exist = true;
    }
    if (_exist)
    {
        std::cerr << "This Primary Key already exists." << std::endl;   //输出用cerr
        flag = false;
    }

    //检查类型&非空(这里应该还要检查类型名输入是否正确的。。。好烦啊这个version不写了，用户难道自己心里没点B数🐎)
    for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
    {
        /*
        switch (_DataTable.attrTable[iter->NAME])
        {
            case _DataTable.INT:
                if (typeid(iter->BASE) != typeid(dataInt))  //基类不等于派生类
                {
                    std::cerr << "The type of \"" << iter->NAME << "\" should be \"int\"." << std::endl;
                    flag = false;
                }
                break;
            case _DataTable.DOUBLE:
                if (typeid(iter->BASE) != typeid(dataDouble))
                {
                    std::cerr << "The type of \"" << iter->NAME << "\" should be \"double\"." << std::endl;
                    flag = false;
                }
                break;
            case _DataTable.STRING:
                if (typeid(iter->BASE) != typeid(dataString))
                {
                    std::cerr << "The type of \"" << iter->NAME << "\" should be \"string\"." << std::endl;
                    flag = false;
                }
                break;
        }
        */
       checkNotNull[iter->NAME] = true;
    }
    for (auto iter = _DataTable.notNullKey.begin(); iter != _DataTable.notNullKey.end(); iter++)
    {
        if ( iter->second && !checkNotNull[iter->first])
        {
            flag = false;
            std::cerr << iter->first << "can not be Null." << std::endl;
        }
    }
    return flag;
}

void DataTable::insert(const std::vector< ATTRIBUTE > &attributes)
{
    Data* _data = new Data;
    if (!checkLegality(*this, attributes))
    {
        std::cerr << "Failed to insert. Please check your input. "<< std::endl;
    }
    else
    {
        for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
        {
            _data->setData(iter->NAME, &iter->BASE);
        }
        mData.insert(mData.end(), _data);
        std::cerr << "Successfully inserted." << std::endl; //这里应该再输出更详细的信息的，这版就不写了
    }
}

void DataTable::remove(std::list<Data*> &dataList)
{
    for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
    {
        delete *iter;
        mData.remove(*iter);
    }
}

void DataTable::update(const ATTRIBUTE &attributes, std::list<Data*> &dataList)
{
    for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
    {
        (*iter)->setData(attributes.NAME, attributes.BASE);
    }
}

void DataTable::select(const std::string &attrName, std::vector<Base*> &attrList, const std::list<Data*> &dataList)
{
    for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
    {
        Base* temp = (*iter)->getData(attrName);
        attrList.insert(attrList.end(), temp);
    }
}