add_subdirectory(shapes)

add_library(gmath STATIC
        math.hpp
        Vector2.hpp
        Vector3.hpp
)

target_link_libraries(gmath PUBLIC shapes)

set_target_properties(gmath PROPERTIES LINKER_LANGUAGE CXX)