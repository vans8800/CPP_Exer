#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <cstdlib>

// 定义一个简单的链表节点
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// 不使用 prefetch 的链表求和
long long sumListWithoutPrefetch(Node* head) {
    long long sum = 0;
    Node* current = head;
    while (current != nullptr) {
        // 耗时操作：访问当前节点的数据（触发 cache miss）
        sum += current->data;
        current = current->next;  // 下一次迭代依赖此指针，但 CPU 此时才去内存取
    }
    return sum;
}

// 使用 __builtin_prefetch 优化的链表求和
long long sumListWithPrefetch(Node* head) {
    if (!head) return 0;

    long long sum = 0;
    Node* current = head;

    // 预取头节点的下一个节点（启动流水线）
    if (current->next) {
        __builtin_prefetch(current->next, 0, 3); // 读取，高局部性
    }

    while (current != nullptr) {
        sum += current->data;

        Node* next = current->next;

        // 关键优化：提前预取下下个节点（避免 pointer chasing 的 stall）
        if (next && next->next) {
            __builtin_prefetch(next->next, 0, 3);
        }

        current = next;
    }
    return sum;
}

int main() {
    const size_t N = 10'000'000; // 百万级节点，模拟大规模业务数据

    // 构建一个长链表（故意不连续分配，模拟真实内存布局）
    std::vector<Node*> nodes;
    nodes.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        nodes.push_back(new Node(static_cast<int>(i)));
    }

    // 打乱顺序再链接，确保内存地址不连续（加剧 cache miss）
    std::shuffle(nodes.begin(), nodes.end(), std::default_random_engine{42});
    for (size_t i = 0; i < N - 1; ++i) {
        nodes[i]->next = nodes[i + 1];
    }
    Node* head = nodes[0];

    // 测试无 prefetch 版本
    auto start = std::chrono::high_resolution_clock::now();
    volatile long long sum1 = sumListWithoutPrefetch(head);
    auto end = std::chrono::high_resolution_clock::now();
    auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 测试有 prefetch 版本
    //start = std::chrono::high_resolution_clock::now();
    //volatile long long sum2 = sumListWithPrefetch(head);
    //end = std::chrono::high_resolution_clock::now();
    //auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Without prefetch: " << time1 << " ms, sum = " << sum1 << "\n";
    //std::cout << "With    prefetch: " << time2 << " ms, sum = " << sum2 << "\n";
    //std::cout << "Speedup: " << (time1 / static_cast<double>(time2)) << "x\n";

    // 清理内存
    for (auto* node : nodes) delete node;

    return 0;
}
