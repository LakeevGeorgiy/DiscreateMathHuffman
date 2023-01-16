add_library(Decode Decode.h Decode.cpp)
target_link_libraries(Decode PRIVATE Encode)