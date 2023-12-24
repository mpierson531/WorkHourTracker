#pragma once

#include <optional>
using std::optional, std::nullopt;

namespace gutils {
    template<class T, class E>
    class Result {
    private:
        optional<T> ok;
        optional<E> err;
    public:
        Result() : Result(nullopt, nullopt) {}

        static Result<T, E> okOf(const T& ok) {
            return Result(optional(ok), nullopt);
        }

        static Result<T, E> errOf(const E& err) {
            return Result(nullopt, optional(err));
        }

        Result(const T& ok, const E& err) {
            this->ok = optional(std::move(ok));
            this->err = optional(std::move(err));
        }

        Result(const optional<T> ok, const optional<E> err) : ok(std::move(ok)), err(std::move(err)) { }

        operator bool() const {
            return isOk();
        }

        T& unwrap() {
            if (!isOk()) {
                throw std::runtime_error { "Result::ok was unwrapped but did not have an ok value." };
            }

            return ok.value();
        }

        E& unwrap_err() {
            if (isOk()) {
                throw std::runtime_error { "Result::err was unwrapped but did not contain anything." };
            }

            return err.value();
        }

        [[nodiscard]] bool isOk() const {
            return !isErr();
        }

        [[nodiscard]] bool isErr() const {
            return err.has_value();
        }
    };
}

//template <typename FloatType, typename E>
//Result<FloatType, E> Result<FloatType, E>::okOf(FloatType ok) {
//	return Result<FloatType, E>(&ok, nullptr);
//}

//template <typename FloatType, typename E>
//Result<FloatType, E> Result<FloatType, E>::okOf(FloatType* ok) {
//	return Result<FloatType, E>(ok, nullptr);
//}

//template <typename FloatType, typename E>
//Result<FloatType, E> Result<FloatType, E>::errOf(E err) {
//	return Result<FloatType, E>(nullptr, err);
//}

//template <typename FloatType, typename E>
//Result<FloatType, E> Result<FloatType, E>::errOf(E* err) {
//	return Result<FloatType, E>(nullptr, err);
//}