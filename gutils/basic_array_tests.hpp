//
// Created by Micah on 12/16/2023.
//

#ifndef BESTBUYTRACKER_BASIC_ARRAY_TESTS_HPP
#define BESTBUYTRACKER_BASIC_ARRAY_TESTS_HPP

#define ARRAY_TEST_SIZE 500'000

#include "../gio/Console.hpp"
#include "../gtiming/Stopwatch.hpp"
#include "Array.hpp"
using gio::Console, gtiming::Stopwatch;

namespace gutils {
    void testMyHeapArrayImpl() {
        Console console;

        Stopwatch<double, gtiming::TimeUnit::Milli> stopwatch;

        stopwatch.Tick();

        Array<int> myHeapArrayImpl;
        myHeapArrayImpl.reallocate(ARRAY_TEST_SIZE);

        for (int index = 0; index != ARRAY_TEST_SIZE; index++) {
            const int value = rand();
            myHeapArrayImpl[index] = value;
        }

        stopwatch.Tick();

        console.println("My heap array impl time for TEST_SIZE " + gutils::toString(ARRAY_TEST_SIZE) + " = " +
                        gutils::toString(stopwatch.RunTime().value));
    }

    void testNativeHeapArrayImpl() {
        Console console;

        Stopwatch<double, gtiming::TimeUnit::Milli> stopwatch;

        stopwatch.Tick();

        int* nativeHeapArrayImpl = new int[ARRAY_TEST_SIZE];

        for (int index = 0; index != ARRAY_TEST_SIZE; index++) {
            const int value = rand();
            nativeHeapArrayImpl[index] = value;
        }

        stopwatch.Tick();

        console.println("Native heap array impl time for TEST_SIZE " + gutils::toString(ARRAY_TEST_SIZE) + " = " +
                        gutils::toString(stopwatch.RunTime().value));

        delete[] nativeHeapArrayImpl;
    }

    void testNativeStackArrayImpl() {
        Console console;

        Stopwatch<double, gtiming::TimeUnit::Milli> stopwatch;

        stopwatch.Tick();

        int nativeStackArrayImpl[ARRAY_TEST_SIZE];

        for (int index = 0; index != ARRAY_TEST_SIZE; index++) {
            const int value = rand();
            nativeStackArrayImpl[index] = value;
        }

        stopwatch.Tick();

        console.println("Native stack impl time for TEST_SIZE " + gutils::toString(ARRAY_TEST_SIZE) + " = " +
                        gutils::toString(stopwatch.RunTime().value));
    }

    void test_STD_ArrayImpl() {
        Console console;

        Stopwatch<double, gtiming::TimeUnit::Milli> stopwatch;

        stopwatch.Tick();

        std::array<int, ARRAY_TEST_SIZE> std_arrayImpl{};

        for (int index = 0; index != ARRAY_TEST_SIZE; index++) {
            const int value = rand();
            std_arrayImpl[index] = value;
        }

        stopwatch.Tick();

        console.println("std::array impl time for TEST_SIZE " + gutils::toString(ARRAY_TEST_SIZE) + " = " +
                        gutils::toString(stopwatch.RunTime().value));
    }
}

#endif //BESTBUYTRACKER_BASIC_ARRAY_TESTS_HPP