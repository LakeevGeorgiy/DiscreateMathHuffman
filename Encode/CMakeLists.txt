add_library(Encode Encode.h Encode.cpp)
target_link_libraries(Encode PRIVATE Decode)