#include <cassert>
#include <ctime>
#include <iostream>
#include <memory>
#include "MemoryPool.hpp"

template <typename T>
struct StackNode {
    T data;
    StackNode* next;
    StackNode(T t_data, StackNode* p = nullptr) : data(t_data), next(p) {}
};

template <typename T, typename Alloc = std::allocator<T>>
class Stack_alloc {
   public:
    using Node = StackNode<T>;
    using Allocator = typename Alloc::template rebind<Node>::other;

    Stack_alloc() : head(nullptr), allocator() {}
    ~Stack_alloc() { clear(); }

    bool empty() { return head == nullptr; }

    void clear() {
        Node* now = head;
        while (now != nullptr) {
            Node* temp = now->next;
            allocator.destroy(now);
            allocator.deallocate(now, 1);
            now = temp;
        }
        head = nullptr;
    }

    void push(T x) {
        Node* newNode = allocator.allocate(1);
        allocator.construct(newNode, x, head);
        head = newNode;
    }

    T pop() {
        T result = head->data;
        Node* temp = head->next;
        allocator.destroy(head);
        allocator.deallocate(head, 1);
        head = temp;
        return result;
    }

    T top() { return head->data; }

   private:
    Allocator allocator;
    Node* head;
};

using namespace std;

const int REPS = 50;
const int ELEMS = 1000000;

int main() {
    clock_t start;
    Stack_alloc<int, std::allocator<int>> stackDefault;
    start = clock();
    for (int j = 0; j < REPS; j++) {
        assert(stackDefault.empty());
        for (int i = 0; i < ELEMS / 4; i++) {
            stackDefault.push(i);
            stackDefault.push(i);
            stackDefault.push(i);
            stackDefault.push(i);
        }
        for (int i = 0; i < ELEMS / 4; i++) {
            stackDefault.pop();
            stackDefault.pop();
            stackDefault.pop();
            stackDefault.pop();
        }
    }
    std::cout << "Default Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl;

    Stack_alloc<int, MemoryPool<int>> stackPool;
    start = clock();
    for (int j = 0; j < REPS; j++) {
        assert(stackPool.empty());
        for (int i = 0; i < ELEMS / 4; i++) {
            stackPool.push(i);
            stackPool.push(i);
            stackPool.push(i);
            stackPool.push(i);
        }
        for (int i = 0; i < ELEMS / 4; i++) {
            stackPool.pop();
            stackPool.pop();
            stackPool.pop();
            stackPool.pop();
        }
    }
    std::cout << "MemoryPool Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl;
    return 0;
}
