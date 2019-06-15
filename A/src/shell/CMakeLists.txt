


FIND_LIBRARY(sqlparser REQUIRED)
FIND_LIBRARY(backend REQUIRED)

ADD_EXECUTABLE(main src/shell.cpp)
ADD_EXECUTABLE(client src/shellclient.cpp)

TARGET_LINK_LIBRARIES(main sqlparser)
TARGET_LINK_LIBRARIES(main backend)
TARGET_LINK_LIBRARIES(client backend)

IF(WIN32)
    TARGET_LINK_LIBRARIES(main -lws2_32)
    TARGET_LINK_LIBRARIES(client -lws2_32)
ENDIF(WIN32)