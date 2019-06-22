
PROJECT(SQLBackend)

AUX_SOURCE_DIRECTORY(src SRC_DIR)
INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(backend ${SRC_DIR} src/databaseext.cpp src/databaseext.h src/server.cpp src/server.h src/client.h src/client.cpp src/clientexcept.h src/regexpr.cpp src/regexpr.h)

