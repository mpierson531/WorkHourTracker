add_library(tracker
        Token.hpp
        Tokenizer.hpp
        Day.hpp
        Parser.hpp
        tracker.hpp)

set_target_properties(tracker PROPERTIES LINKER_LANGUAGE CXX)