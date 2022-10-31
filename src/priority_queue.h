#pragma once

#include <cstdlib>
#include <vector>

template <typename T = int64_t, typename Container = std::vector<T>, typename Comparator = std::less<T>>
class PriorityQueue {
public:
    PriorityQueue() {
    }

    size_t Size() {
        return container_.size();
    }

    bool Empty() {
        return container_.empty();
    }

    void Push(T val) {
        container_.push_back(val);
        SiftUp(container_.size() - 1);
    }

    T Pop() {
        T head = container_[0];
        container_[0] = container_[container_.size() - 1];
        container_.pop_back();
        SiftDown(0);
        return head;
    }

private:
    Container container_;
    Comparator comparator_;

    void SiftUp(size_t node) {
        while (node > 0 && comparator_(container_[node], container_[(node - 1) / 2])) {
            std::swap(container_[node], container_[(node - 1) / 2]);
            node = (node - 1) / 2;
        }
    }

    void SiftDown(size_t node) {
        while (2 * node + 1 < container_.size()) {
            size_t left_child = 2 * node + 1;
            size_t right_child = 2 * node + 2;
            size_t parent_node = left_child;
            if (right_child < container_.size() && comparator_(container_[right_child], container_[left_child])) {
                parent_node = right_child;
            }
            if (comparator_(container_[node], container_[parent_node])) {
                break;
            }
            std::swap(container_[node], container_[parent_node]);
            node = parent_node;
        }
    }
};
