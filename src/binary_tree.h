#pragma once

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "exceptions.h"
#include "binary_code/binary_code.h"

struct CharToBinaryCode {
    int16_t character;
    BinaryCode code;
};

template <typename Value = char16_t, typename ContainerType = size_t>
class BinaryTree {
public:
    struct Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        Value val = 0;

        explicit Node(std::shared_ptr<Node> left = nullptr, std::shared_ptr<Node> right = nullptr, Value val = 0)
            : left(left), right(right), val(val) {
        }
    };

    explicit BinaryTree(std::vector<ContainerType>& container) : root_(nullptr), container_(container) {
    }

    void Merge(std::shared_ptr<Node> parent, std::shared_ptr<Node> left_child, std::shared_ptr<Node> right_child) {
        parent->left = left_child;
        parent->right = right_child;
    }

    void MakeRoot(std::shared_ptr<Node> root) {
        root_ = root;
    }

    void GetAllLeaves() {
        DfsToLeaf(root_, 0);
    }

    bool DfsToNode(bool path, std::weak_ptr<Node>& node) {
        if (path) {
            node = node.lock().get()->right;
        } else {
            node = node.lock().get()->left;
        }
        if (node.lock().get()->left == nullptr && node.lock().get()->right == nullptr) {
            return true;
        }
        return false;
    }

    std::weak_ptr<Node> GetRoot() {
        return root_;
    }

    void BuildTree() {
        for (ContainerType& element : container_) {
            DfsToPath(root_, element.code, element.character);
        }
    }

private:
    std::shared_ptr<Node> root_;
    std::vector<ContainerType>& container_;

    void DfsToPath(std::weak_ptr<Node> node, BinaryCode& path, int16_t& character, size_t depth = 0) {
        if (path.Size() == depth) {
            node.lock().get()->val = character;
            return;
        }
        if (path.At(depth)) {
            if (node.lock().get()->right == nullptr) {
                node.lock().get()->right = std::make_shared<Node>();
            }
            DfsToPath(node.lock().get()->right, path, character, depth + 1);
        } else {
            if (node.lock().get()->left == nullptr) {
                node.lock().get()->left = std::make_shared<Node>();
            }
            DfsToPath(node.lock().get()->left, path, character, depth + 1);
        }
    }

    void DfsToLeaf(std::shared_ptr<Node> node, size_t code_size) {
        if (node == nullptr) {
            return;
        }
        if (node->left == nullptr && node->right == nullptr) {
            container_[node->val] = code_size;
            return;
        }
        DfsToLeaf(node->left, code_size + 1);
        DfsToLeaf(node->right, code_size + 1);
    }
};
