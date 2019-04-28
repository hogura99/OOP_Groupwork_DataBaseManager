all: DataBaseManager.o DataBase.o ParamSpliter.o main.o Base.o Derived.o DataTable.o Data.o
	g++ DataBaseManager.o DataBase.o DataTable.o ParamSpliter.o Base.o Derived.o Data.o main.o -o main -std=c++14 &> ErrorInfo.txt

main.o: main.cpp
	g++ -c main.cpp -o main.o -std=c++14

DataBaseManager.o: DataBaseManager.cpp DataBaseManager.h
	g++ -c DataBaseManager.cpp -o DataBaseManager.o -std=c++14

DataBase.o: DataBase.cpp DataBase.h DataTable.h DataTable.cpp
	g++ -c DataBase.cpp -o DataBase.o -std=c++14

DataTable.o: DataTable.cpp DataTable.h ParamSpliter.h expression.h
	g++ -c DataTable.cpp -o DataTable.o -std=c++14

ParamSpliter.o: ParamSpliter.cpp ParamSpliter.h
	g++ -c ParamSpliter.cpp -o ParamSpliter.o -std=c++14

Data.o: Data.cpp Data.h
	g++ -c Data.cpp -o Data.o -std=c++14

Base.o: Base.cpp Base.h Derived.h expression.h
	g++ -c Base.cpp -o Base.o -std=c++14

Derived.o: Derived.h Derived.cpp
	g++ -c Derived.cpp -o Derived.o -std=c++14

clean:
	rm *.o