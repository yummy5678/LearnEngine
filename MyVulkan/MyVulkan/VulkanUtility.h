#pragma once
#include <vulkan/vulkan.hpp>

// �e���v���[�g�֐��̒�`
template <typename T, typename UniqueT>
std::vector<T> convertToVector(const std::vector<UniqueT>& uniqueVector) {
    std::vector<T> result;
    result.reserve(uniqueVector.size()); // �œK���̂��߂ɃT�C�Y��\�񂷂�

    for (const auto& uniqueElement : uniqueVector) {
        result.push_back(*uniqueElement); // UniqueT����T�����o���Ēǉ�����
    }

    return result;
}