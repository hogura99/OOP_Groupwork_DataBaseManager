INCLUDE_DIRECTORIES(src)
AUX_SOURCE_DIRECTORY(src SIMPLE_PARSER_SRC)

ADD_LIBRARY(sqlparser ${SIMPLE_PARSER_SRC} src/parserext.cpp src/parserext.h)

# add test parser
#ADD_EXECUTABLE(test_parser test/testparser.cpp)
#TARGET_LINK_LIBRARIES(test_parser sqlparser)
#TARGET_LINK_LIBRARIES(test_parser backend)