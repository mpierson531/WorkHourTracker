#include <iostream>
#include <string>
using std::string;

#include "gio/Console.hpp"
#include "gio/gio_utils"
#include "gutils/Array.hpp"
#include "gutils/Result.hpp"
#include "gtiming/TimeSegment.hpp"
#include "gtiming/TimeFormat.hpp"
#include "gtiming/TimeStamp.hpp"
using gio::Console, gutils::Array, gutils::Result;
using gtiming::TimeSegment, gtiming::TimeFormat, gtiming::TimeStamp;

#include "tracker/tracker.hpp"
#include "gtiming/TimeStamp.hpp"
#include "gtiming/TimeFormat.hpp"

int main() {
    Console console;

    const string testFilePath = "C:\\Users\\Micah\\CLionProjects\\WorkHourTracker\\test_file.txt";

    auto testStringResult = gio::readFile(testFilePath);

    if (!testStringResult) {
        console.println("Error reading test file.");
        exit(-1);
    }

    const string testString = testStringResult.unwrap();

    auto tokens = tracker::tokenize(testString);

    if (!tokens) {
        const Array<string> errors = tokens.unwrap_err();

        for (const string& error : errors) {
            console.println(error);
        }

        return -1;
    }

    auto days = tracker::parse<true>(tokens.unwrap());

    if (!days) {
        const Array<string> errors = days.unwrap_err();

        for (const string& error : errors) {
            console.println(error);
        }

        return -1;
    }

    const double totalAmount = tracker::evaluate(days.unwrap());

    console.println(totalAmount);

    return 0;
}