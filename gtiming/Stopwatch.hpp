//
// Created by Micah on 10/21/2023.
//

#ifndef CPP_TESTING_LEARNING_STOPWATCH_HPP
#define CPP_TESTING_LEARNING_STOPWATCH_HPP

#include "Time.hpp"
#include "../gutils/gutils.hpp"
#include <chrono>
using std::chrono::duration, std::chrono::high_resolution_clock;

namespace gtiming {
    template<gutils::floating_point FloatType, TimeUnit Unit>
    class Stopwatch {
    private:
        Time<FloatType, Unit> startTime;
        Time<FloatType, Unit> endTime;
        Time<FloatType, Unit> runTime;
        Time<FloatType, Unit> average;
        Time<FloatType, Unit> totalTime;
        bool isTicking;
        size_t ticks;

        void Start() {
            ticks++;
            startTime = Time<FloatType, Unit>::Now();
        }

        void End() {
            switch(Unit) {
                case TimeUnit::Nano:
                    endTime = Time<FloatType, Unit>::Now();
                    break;
                case TimeUnit::Milli:
                    endTime = Time<FloatType, Unit>::Now();
                    break;
                case TimeUnit::Second:
                    endTime = Time<FloatType, Unit>::Now();
                    break;
            }

            runTime = endTime - startTime;
            totalTime += runTime;
            average = totalTime / ticks;
        }
    public:
        Stopwatch() :
        startTime(0.0),
        endTime(0.0),
        runTime(0.0),
        average(0.0),
        totalTime(0.0),
        ticks(0),
        isTicking(false) { }

        Time<FloatType, Unit> Tick() {
            if (!isTicking) {
                Start();
                isTicking = true;
                return 0.0;
            }

            End();
            Start();
            return runTime;
        }

        void Reset() {
            startTime = 0.0;
            endTime = 0.0;
            runTime = 0.0;
            average = 0.0;
            totalTime = 0.0;
            isTicking = false;
            ticks = 0;
        }

        [[nodiscard]] bool IsTicking() const {
            return isTicking;
        }

        [[nodiscard]] Time<FloatType, Unit> StartTime() const {
            return startTime;
        }

        [[nodiscard]] Time<FloatType, Unit> EndTime() const {
            return endTime;
        }

        [[nodiscard]] Time<FloatType, Unit> RunTime() const {
            return runTime;
        }

        [[nodiscard]] Time<FloatType, Unit> Average() const {
            return average;
        }

        [[nodiscard]] Time<FloatType, Unit> TotalTime() const {
            return totalTime;
        }

        [[nodiscard]] size_t Ticks() const {
            return ticks;
        }
    };
}

#endif //CPP_TESTING_LEARNING_STOPWATCH_HPP