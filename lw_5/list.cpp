#include <iostream>
#include <cstdlib>
#include <map>
#include <concepts>

using namespace std;

template<class T, size_t BLOCK_SIZE>
class Allocator {
    T* _memory;
    size_t _memory_size;

public:
    static_assert(BLOCK_SIZE > 0);
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    template<class U> 
    struct rebind { 
        using other = Allocator<U, BLOCK_SIZE>; 
    };

    pointer address(reference value) const { 
        return &value; 
    }

    Allocator(): _memory(reinterpret_cast<T*>(::operator new(BLOCK_SIZE * sizeof(T)))), _memory_size(0) {}
    
    ~Allocator() {
        delete[] _memory;
    }

    pointer allocate(size_type num, const void* hint = 0) {
        if (_memory_size + num > BLOCK_SIZE)
            throw bad_alloc();
        pointer result = &_memory[_memory_size];
        _memory_size += num;
        return result;
    }

    void construct(pointer p, const value_type& value) {
        new ((void*)p) value_type(value);
    }

    void destroy(pointer p) {
        p->~value_type();
    }

    void deallocate(pointer p, size_type num) {
        //?
    }

    size_type max_size() const {
        return BLOCK_SIZE;
    }
};

template<typename T>
concept Node = requires(T node) {
    { node.value } -> convertible_to<typename T::value_type>;
    { node.next } -> convertible_to<T*>;
} or requires(T node) {
    { node.value } -> convertible_to<typename T::value_type>;
    { node.next } -> convertible_to<T*>;
    { node.prev } -> convertible_to<T*>;
};

template<Node T>
class Iterator {
    T* ptr;

public:
    using value_type = typename T::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = ptrdiff_t;
    using iterator_category = bidirectional_iterator_tag;

    Iterator(T* p = nullptr): ptr(p) {}

    reference operator *() const {
        return ptr->value;
    }

    pointer operator ->() const {
        return &(ptr->value);
    }

    Iterator& operator ++() {
        ptr = ptr->next;
        return *this;
    }

    Iterator operator ++(int) {
        Iterator tmp(ptr);
        operator ++();
        return tmp;
    }
    
    Iterator& operator --() {
        if (ptr)
            ptr = ptr->prev;
        return *this;
    }
    
    Iterator operator --(int) {
        Iterator tmp(ptr);
        operator --();
        return tmp;
    }

    bool operator ==(const Iterator& other) const {
        return ptr == other.ptr;
    }

    bool operator !=(const Iterator& other) const {
        return ptr != other.ptr;
    }
};

template<typename T, size_t BLOCK_SIZE, template<class, size_t> class Alloc>
class List {
    struct Node {
        using value_type = T;
        T value;
        Node* next;

        Node(const T& v, Node* n): value(v), next(n) {}
    };

    using NodeAllocator = Alloc<Node, BLOCK_SIZE>;

    NodeAllocator allocator;
    Node* head;

public:
    using NodeType = Node;
    using ValueType = T;
    using iterator = Iterator<Node>;
    using const_iterator = Iterator<Node>;

    List(): head(nullptr) {}

    ~List() {
        while (head) {
            Node* temp = head;
            head = head->next;
            allocator.destroy(temp);
            //allocator.deallocate(temp, 1);
        }
    }

    void push_front(const T& value) {
        Node* new_node = allocator.allocate(1);
        allocator.construct(new_node, Node(value, head));
        head = new_node;
    }

    void pop_front() {
        if (head) {
            Node* old_head = head;
            head = head->next;
            allocator.destroy(old_head);
            allocator.deallocate(old_head, 1);
        }
    }

    T& front() {
        if(head)
            return head->value;
    }

    const T& front() const {
        if(head)
            return head->value;
    }

    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }

    /*const_iterator begin() const {
        return const_iterator(head);
    }

    const_iterator end() const {
        return const_iterator(nullptr);
    }*/
};


int main() {
    map<int, int, less<int>, Allocator<pair<const int, int>, 10>> m;
    for (int i = 0; i < 5; ++i)
        m[i] = i * 10;
    for (const auto& p: m)
        cout << p.first << " " << p.second << '\n';
    
    List<int, 10, Allocator> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    for (auto it = list.begin(); it != list.end(); ++it)
        cout << *it << " ";
    
    cout << '\n';
    return 0;
}
