//
// Created by hama on 20. 4. 15..
//

#ifndef HAMANET_TRY_H
#define HAMANET_TRY_H


#include <exception>
#include <optional>
#include <functional>

namespace hama {

    template<typename T>
    class Try {
        bool isError;
        std::string errorMessage;
        std::optional<T> result;

    public:
        explicit Try(T);

        explicit Try(const std::exception &);

        explicit Try(std::optional<T>);

        bool isFailure();

        bool isSuccess();

        std::optional<T> get();

        T getOrElse(T other);

        std::optional<T> toOption();

        std::string getErrorMessage();
    };


    template<typename T>
    Try<T>::Try(std::optional<T> value) {
        result = value;
        isError = false;
    }

    template<typename T>
    Try<T>::Try(const std::exception &exception) {
        result = std::nullopt;
        errorMessage = exception.what();
        isError = true;
    }

    template<typename T>
    bool Try<T>::isFailure() {
        return isError;
    }

    template<typename T>
    bool Try<T>::isSuccess() {
        return !isFailure();
    }

    template<typename T>
    std::optional<T> Try<T>::get() {
        return result;
    }

    template<typename T>
    T Try<T>::getOrElse(T other) {
        if (isFailure()) {
            return other;
        }
        return result.value_or(other);
    }

    template<typename T>
    std::optional<T> Try<T>::toOption() {
        return result;
    }

    template<typename T>
    std::string Try<T>::getErrorMessage() {
        if (isSuccess()) {
            return "";
        }
        return errorMessage;
    }

    template<typename T>
    Try<T>::Try(T t) {
        result = std::make_optional(t);
        isError = false;
    }

}
#endif //HAMANET_TRY_H
