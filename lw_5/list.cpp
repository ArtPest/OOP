#include<iostream>
#include<cstdlib>
#include<map>

using namespace std;

template<typename T>
struct Node {
    T value;
    Node<T>* next;
};

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

    Allocator(): _memory(new value_type[BLOCK_SIZE * sizeof(value_type)]), _memory_size(0) {}
    
    ~Allocator() {
        delete[] _memory;
    }

    pointer allocate(size_type num) {
        if((_memory_size + num * sizeof(value_type)) > BLOCK_SIZE * sizeof(value_type))
            throw bad_alloc();
        pointer result = reinterpret_cast<pointer>(_memory + _memory_size);
        _memory_size += num * sizeof(value_type);
        return result;
    }

    void construct(pointer p, const value_type& value) {
        new ((void*)p) value_type(value);
    }
    
    void destroy(pointer p) {
        p -> ~value_type();
    }

    void deallocate(pointer p, size_type n) {
        delete[] _memory;
        _memory = new value_type[BLOCK_SIZE * sizeof(value_type)];
        _memory_size = 0;
    }
};

template <typename T>
class Iterator {
    Node<T>* node;
    
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = ptrdiff_t;
    using iterator_category = forward_iterator_tag;
    
    Iterator(): node(nullptr) {}
    
    Iterator(Node<T>* _node): node(_node) {}

    reference operator *() const {
        return node -> value;
    }

    pointer operator ->() const {
        return &(node -> value);
    }

    Iterator& operator ++() {
        node = node -> next;
        return *this;
    }

    Iterator operator ++(int n) {
        Iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
        return lhs.node == rhs.node;
    }

    friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
        return !(lhs == rhs);
    }
};

template<typename T, size_t Size, class Allocator>
class List {
    using NodeAllocator = typename Allocator::template rebind<Node<T>>::other;
    NodeAllocator allocator;
    Node<T>* head;
    
public:
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    
    void push_front(T value) {
        Node<T>* new_node = allocator.allocate(1);
        allocator.construct(new_node, Node<T>{value, move(head)});
        head = new_node;
    }

    void pop_front() {
        Node<T>* old_head = head;
        head = head -> next;
        allocator.destroy(old_head);
        allocator.deallocate(old_head, 1);
    }
    
    T& front() {
        return head -> value;
    }
    
    const T& front() const {
        return head -> value;
    }

    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }

    const_iterator begin() const {
        return const_iterator(head);
    }

    const_iterator end() const {
        return const_iterator(nullptr);
    }
};

int main() {
    map<int, int, less<int>, Allocator<pair<const int, int>, 10>> m;
    for(int i = 0; i < 5; ++i)
        m[i] = i * 10;
    for(const auto& i: m)
        cout << i.first << " " << i.second << '\n';
    List<int, 10, Allocator<int, 10>> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    for(auto it = list.begin(); it != list.end(); ++it)
        cout << *it << " ";
    cout << '\n';
    return 0;
}
