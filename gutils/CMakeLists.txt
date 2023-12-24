add_library(gutils STATIC
        Result.hpp
        gutils.hpp
        Array.hpp
        basic_array_tests.hpp
        BitBoolArray.hpp
        AutoBitBoolArray.hpp
        bit_utils.hpp
        ENUM_CLASS.hpp)

set_target_properties(gutils PROPERTIES LINKER_LANGUAGE CXX)