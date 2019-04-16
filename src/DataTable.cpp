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

bool checkLegality(DataTable _DataTable, const std::vector<ATTRIBUTE>& attributes)
{
    /*
        实现逻辑：
            检查主键：确定传入参数中主键的值（一个for循环），遍历现有列表中主键的值，看是否有重复（又一个for循环）
            检查类型：遍历传入参数，用typeid判断
            检查非空：非空这里我不太会写，感觉写出来会很丑）
    */
    bool flag = true;   //函数返回值

    //检查主键
    const Base* primaryKeyValue;
    for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
    {
        if (iter->NAME == _DataTable.primaryKey)
            primaryKeyValue = &iter->BASE;
    }
    bool _exist = false;
    for (auto iter = _DataTable.mData.begin(); iter < _DataTable.mData.end(); iter++)
    {
        if ((*iter)->getData(_DataTable.primaryKey) == primaryKeyValue)
            _exist = true;
    }
    if (_exist)
    {
        std::cout << "This Primary Key already exists." << std::endl;
        flag = false;
    }

    //检查类型&非空(这里应该还要检查类型名输入是否正确的。。。好烦啊这个version不写了，用户难道自己心里没点B数🐎)
    for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
    {
        switch (_DataTable.attrTable[iter->NAME])
        {
            case _DataTable.INT:
                if (typeid(iter->BASE) != typeid(dataInt))
                {
                    std::cout << "The type of \"" << iter->NAME << "\" should be \"int\"." << std::endl;
                    flag = false;
                }
                break;
            case _DataTable.DOUBLE:
                if (typeid(iter->BASE) != typeid(dataDouble))
                {
                    std::cout << "The type of \"" << iter->NAME << "\" should be \"double\"." << std::endl;
                    flag = false;
                }
                break;
            case _DataTable.STRING:
                if (typeid(iter->BASE) != typeid(dataString))
                {
                    std::cout << "The type of \"" << iter->NAME << "\" should be \"string\"." << std::endl;
                    flag = false;
                }
                break;
        }
    }
    return flag;
}

void DataTable::insert(const std::vector< ATTRIBUTE > &attributes)
{
    Data _data;
    if (!checkLegality(*this, attributes))
    {
        std::cout << "Failed to insert. Please check your input. "<< std::endl;
    }
    else
    {
        for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
        {
            _data.setData(iter->NAME, &iter->BASE);
        }
        mData.insert(mData.end(), &_data);  //这里的问题怎么改）
        std::cout << "Successfully inserted." << std::endl; //这里应该再输出更详细的信息的，这版就不写了
    }
}

void DataTable::remove(std::list<Data*> &dataList)
{
    for (auto iter = dataList.begin(); iter < dataList.end(); iter++)
    {
        delete *iter;
        mData.remove(*iter);
    }
}

void DataTable::update(const ATTRIBUTE &attributes, std::list<Data*> &dataList)
{
    for (auto iter = dataList.begin(); iter < dataList.end(); iter++)
    {
        (*iter)->setData(attributes.NAME, attributes.BASE);
    }
}

void DataTable::select(const std::string &attrName, std::vector<Base*> &attrList, const std::list<Data*> &dataList)
{
    for (auto iter = dataList.begin(); iter < dataList.end(); iter++)
    {
        Base* temp = (*iter)->getData(attrName);
        attrList.insert(attrList.end(), temp);
    }
}