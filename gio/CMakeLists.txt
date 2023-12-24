add_library(gio STATIC
        gio_utils
        Console.hpp)

set_target_properties(gio PROPERTIES LINKER_LANGUAGE CXX)

add_dependencies(gio gcolors gutils)