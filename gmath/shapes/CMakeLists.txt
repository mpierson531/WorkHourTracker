add_subdirectory(collisions)

add_library(shapes STATIC
            Shape.hpp
            shapes.hpp
)

target_link_libraries(shapes PUBLIC collisions)

set_target_properties(shapes PROPERTIES LINKER_LANGUAGE CXX)