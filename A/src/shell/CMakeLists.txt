FIND_LIBRARY(sqlparser REQUIRED)
FIND_LIBRARY(backend REQUIRED)

ADD_EXECUTABLE(main src/shell.cpp)
ADD_EXECUTABLE(main1 src/shell.cpp)
ADD_EXECUTABLE(main2 src/shell.cpp)
#ADD_EXECUTABLE(client src/shellclient.cpp)
ADD_EXECUTABLE(server src/shellserver.cpp)

TARGET_LINK_LIBRARIES(main sqlparser)
TARGET_LINK_LIBRARIES(main backend)

TARGET_LINK_LIBRARIES(main1 sqlparser)
TARGET_LINK_LIBRARIES(main1 backend)

TARGET_LINK_LIBRARIES(main2 sqlparser)
TARGET_LINK_LIBRARIES(main2 backend)

#TARGET_LINK_LIBRARIES(client backend)
#TARGET_LINK_LIBRARIES(client sqlparser)

TARGET_LINK_LIBRARIES(server backend)
TARGET_LINK_LIBRARIES(server sqlparser)

IF(WIN32)
    TARGET_LINK_LIBRARIES(main -lws2_32)
    #TARGET_LINK_LIBRARIES(client -lws2_32)
    TARGET_LINK_LIBRARIES(server -lws2_32)
ENDIF(WIN32)

# add test program
ADD_EXECUTABLE(test src/test.cpp)
TARGET_LINK_LIBRARIES(test backend)
