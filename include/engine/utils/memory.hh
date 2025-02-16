#pragma once

#include <functional>
#include <memory>

template<typename T>
using deleted_unique_ptr = std::unique_ptr<T,std::function<void(T*)>>;

using callback = std::function<void(void)>;

template<typename T>
using supplier = std::function<T(void)>;

template<typename T>
using consumer = std::function<void(T)>;

template<typename T, typename U>
using mapper = std::function<U(T)>;
