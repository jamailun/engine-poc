#pragma once

#include <functional>
#include <memory>

template<typename T>
using deleted_unique_ptr = std::unique_ptr<T,std::function<void(T*)>>;
