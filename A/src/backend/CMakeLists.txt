
PROJECT(SQLBackend)

AUX_SOURCE_DIRECTORY(src SRC_DIR)
INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(backend ${SRC_DIR} src/databaseext.cpp src/databaseext.h src/server.h src/client.h)

