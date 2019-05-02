all: DataBaseManager.o DataBase.o ParamSpliter.o main.o Value.o Derived.o DataTable.o Data.o
	g++ DataBaseManager.o DataBase.o DataTable.o ParamSpliter.o Value.o Derived.o Data.o main.o -o main -std=c++14 -g

main.o: main.cpp
	g++ -c main.cpp -o main.o -std=c++14 -g

DataBaseManager.o: DataBaseManager.cpp DataBaseManager.h
	g++ -c DataBaseManager.cpp -o DataBaseManager.o -std=c++14 -g

DataBase.o: DataBase.cpp DataBase.h DataTable.h DataTable.cpp
	g++ -c DataBase.cpp -o DataBase.o -std=c++14 -g

DataTable.o: DataTable.cpp DataTable.h ParamSpliter.h expression.h
	g++ -c DataTable.cpp -o DataTable.o -std=c++14 -g

ParamSpliter.o: ParamSpliter.cpp ParamSpliter.h
	g++ -c ParamSpliter.cpp -o ParamSpliter.o -std=c++14 -g

Data.o: Data.cpp Data.h
	g++ -c Data.cpp -o Data.o -std=c++14 -g

Value.o: Value.cpp Value.h Derived.h expression.h
	g++ -c Value.cpp -o Value.o -std=c++14 -g

Derived.o: Derived.h Derived.cpp
	g++ -c Derived.cpp -o Derived.o -std=c++14 -g

gdb:
	gdb debug/main

clean:
	rm *.o