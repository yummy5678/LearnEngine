#pragma once
#include <vulkan/vulkan.hpp>

// テンプレート関数の定義
template <typename T, typename UniqueT>
std::vector<T> convertToVector(const std::vector<UniqueT>& uniqueVector) {
    std::vector<T> result;
    result.reserve(uniqueVector.size()); // 最適化のためにサイズを予約する

    for (const auto& uniqueElement : uniqueVector) {
        result.push_back(*uniqueElement); // UniqueTからTを取り出して追加する
    }

    return result;
}