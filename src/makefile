all: DataBaseManager.o DataBase.o ParamSpliter.o main.o Value.o AttributeValue.o DataTable.o Data.o str_algorithm.o expression.o
	g++ expression.o str_algorithm.o ParamSpliter.o Value.o AttributeValue.o main.o -o main -std=c++14

local: DataBaseManager.o DataBase.o ParamSpliter.o main.o Value.o AttributeValue.o DataTable.o Data.o str_algorithm.o expression.o
	g++ expression.o str_algorithm.o DataBaseManager.o DataBase.o DataTable.o ParamSpliter.o Value.o AttributeValue.o Data.o main.o -o debug/main -std=c++14 -g

main.o: main.cpp
	g++ -c main.cpp -o main.o -std=c++14 -g

DataBaseManager.o: DataBaseManager.h errorstream.h DataBase.h DataTable.h ParamSpliter.h
	g++ -c DataBaseManager.h -o DataBaseManager.o -std=c++14 -g

DataBase.o: DataBase.h DataTable.h errorstream.h
	g++ -c DataBase.h -o DataBase.o -std=c++14 -g

DataTable.o: DataTable.h ParamSpliter.h expression.h ParamSpliter.h errorstream.h Value.h Data.h
	g++ -c DataTable.h -o DataTable.o -std=c++14 -g

Data.o: Data.h AttributeValue.h
	g++ -c Data.h -o Data.o -std=c++14 -g

ParamSpliter.o: ParamSpliter.cpp ParamSpliter.h str_algorithm.h expression.h errorstream.h
	g++ -c ParamSpliter.cpp -o ParamSpliter.o -std=c++14 -g

Value.o: Value.cpp Value.h AttributeValue.h expression.h
	g++ -c Value.cpp -o Value.o -std=c++14 -g

AttributeValue.o: AttributeValue.h AttributeValue.cpp Value.h
	g++ -c AttributeValue.cpp -o AttributeValue.o -std=c++14 -g

str_algorithm.o: str_algorithm.h str_algorithm.cpp
	g++ -c str_algorithm.cpp -o str_algorithm.o -std=c++14 -g

expression.o: expression.h expression.cpp
	g++ -c expression.cpp -o expression.o -std=c++14 -g

gdb:
	gdb debug/main

clean:
	rm *.o