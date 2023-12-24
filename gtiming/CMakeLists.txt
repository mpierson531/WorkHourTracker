add_library(gtiming STATIC
        timing.hpp
        Time.hpp
        Stopwatch.hpp
        TimeSegment.hpp
        TimeFormat.hpp
        TimeStamp.hpp
        dates.hpp)

set_target_properties(gtiming PROPERTIES LINKER_LANGUAGE CXX)