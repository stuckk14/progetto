ADD_EXECUTABLE(parte2 main.cpp)

ADD_LIBRARY(Graphics2 STATIC graphics2.cpp)
ADD_LIBRARY(Pandemy2 STATIC pandemy2.cpp)

TARGET_LINK_LIBRARIES(parte2 Graphics2 Pandemy2 sfml-system sfml-graphics pthread)


SET_TARGET_PROPERTIES(parte2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Parte2/bin")