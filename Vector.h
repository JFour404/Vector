#pragma once
#include <cstddef>   // size_t
#include <memory>    // allocator
#include <algorithm> // std::copy, std::move, etc.
#include <iterator>  // std::reverse_iterator

template <typename T>
class Vector {
public:
    
    // Member types
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::allocator<T> allocator_type;

private:
    // Private member variables
    size_type m_limit;          // m_limit of the vector (maximum number of elements it can hold)
    T* m_data;                    // Pointer to the first element
    size_type sz;               // Size of the vector (number of elements)
    allocator_type alloc;       // Allocator object

    void deallocate(pointer p) {
        std::allocator_traits<allocator_type>::deallocate(alloc, p, m_limit);
    }
public:

    // Constructor
    Vector() noexcept : m_data(nullptr), sz(0), m_limit(0) {}

    // Constructor with initial size and value
    explicit Vector(size_type count, const T& value = T()) : m_data(nullptr), sz(count), m_limit(count) {
       m_data = allocate(m_limit);
       uninitialized_fill(m_data, m_data + sz, value);
    }

    // Copy constructor
    Vector(const Vector& other) : m_data(nullptr), sz(other.sz), m_limit(other.m_limit) {
        m_data = allocate(m_limit);
        uninitialized_copy(other.m_data, other.m_data + sz, m_data);
    }

    // Move constructor
    Vector(Vector&& other) noexcept : m_data(other.m_data), sz(other.sz), m_limit(other.m_limit) {
        other.m_data = nullptr;
        other.sz = 0;
        other.m_limit = 0;
    }

    // Destructor
    ~Vector() {
        clear();
        deallocate(m_data);
    }

    // Copy assignment operator
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            clear();
            deallocate(m_data);

            sz = other.sz;
            m_limit = other.m_limit;

            m_data = allocate(m_limit);
            uninitialized_copy(other.m_data, other.m_data + sz, m_data);
        }
        return *this;
    }

    // Move assignment operator
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            clear();
            deallocate(m_data);

            m_data = other.m_data;
            sz = other.sz;
            m_limit = other.m_limit;

            other.m_data = nullptr;
            other.sz = 0;
            other.m_limit = 0;
        }
        return *this;
    }

    // Element access
    reference operator[](size_type index) {
        return m_data[index];
    }

    const_reference operator[](size_type index) const {
        return m_data[index];
    }

    reference at(size_type index) {
        if (index >= sz)
            throw std::out_of_range("Vector::at() - Index out of range");
        return m_data[index];
    }

    const_reference at(size_type index) const {
        if (index >= sz)
            throw std::out_of_range("Vector::at() - Index out of range");
        return m_data[index];
    }

    reference front() {
        return m_data[0];
    }

    const_reference front() const {
        return m_data[0];
    }

    reference back() {
        return m_data[sz - 1];
    }

    const_reference back() const {
        return m_data[sz - 1];
    }

    pointer data() noexcept {
        return m_data;
    }

    const_pointer data() const noexcept {
        return m_data;
    }

    // Iterators
    iterator begin() noexcept {
        return m_data;
    }

    const_iterator begin() const noexcept {
        return m_data;
    }

    const_iterator cbegin() const noexcept {
        return m_data;
    }

    iterator end() noexcept {
        return m_data + sz;
    }

    const_iterator end() const noexcept {
        return m_data + sz;
    }

    const_iterator cend() const noexcept {
        return m_data + sz;
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    // Capacity
    bool empty() const noexcept {
        return sz == 0;
    }

    size_type size() const noexcept {
        return sz;
    }

    size_type capacity() const noexcept {
        return m_limit;
    }

    void reserve(size_type new_m_limit) {
        if (new_m_limit > m_limit) {
            pointer new_m_data = allocate(new_m_limit);
            std::uninitialized_move(m_data, m_data + sz, new_m_data);
            clear();
            deallocate(m_data);
            m_data = new_m_data;
            m_limit = new_m_limit;
        }
    }

    void shrink_to_fit() {
        if (sz < m_limit) {
            pointer new_m_data = allocate(sz);
            std::uninitialized_move(m_data, m_data + sz, new_m_data);
            clear();
            deallocate(m_data);
            m_data = new_m_data;
            m_limit = sz;
        }
    }

    // Modifiers
    void clear() noexcept {
        for (pointer p = m_data; p != m_data + sz; ++p) {
            p->~T();
        }
        sz = 0;
    }

    iterator insert(const_iterator pos, const T& value) {
        return insert(pos, 1, value);
    }

    iterator insert(const_iterator pos, size_type count, const T& value) {
        difference_type index = pos - m_data;
        if (count == 0)
            return m_data + index;
        if (sz + count > m_limit)
            reserve(grow_m_limit(sz + count));
        pointer insert_pos = m_data + index;
        if (insert_pos < m_data + sz)
            std::copy_backward(insert_pos, m_data + sz, m_data + sz + count);
        std::uninitialized_fill_n(insert_pos, count, value);
        sz += count;
        return m_data + index;
    }

    template <typename InputIterator>
    iterator insert(const_iterator pos, InputIterator first, InputIterator last) {
        difference_type index = pos - m_data;
        difference_type count = std::distance(first, last);
        if (count == 0)
            return m_data + index;
        if (sz + count > m_limit)
            reserve(grow_m_limit(sz + count));
        pointer insert_pos = m_data + index;
        if (insert_pos < m_data + sz)
            std::copy_backward(insert_pos, m_data + sz, m_data + sz + count);
        std::uninitialized_copy(first, last, insert_pos);
        sz += count;
        return m_data + index;
    }

    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last) {
        difference_type erase_count = last - first;
        difference_type index = first - m_data;
        pointer erase_pos = m_data + index;
        pointer new_end = erase_pos;

        // Manually copy the elements after the erase range
        for (pointer p = erase_pos + erase_count; p != m_data + sz; ++p) {
            *new_end = std::move(*p);
            ++new_end;
        }

        // Destroy the remaining elements
        for (pointer p = new_end; p != m_data + sz; ++p) {
            p->~T();
        }

        sz -= erase_count;
        return m_data + index;
    }


    void push_back(const T& value) {
        if (sz >= m_limit)
            reserve(grow_m_limit(sz + 1));
        std::allocator_traits<allocator_type>::construct(alloc, m_data + sz, value);
        ++sz;
    }

    void pop_back() {
        if (sz > 0) {
            --sz;
            std::allocator_traits<allocator_type>::destroy(alloc, m_data + sz);
        }
    }

    void resize(size_type count) {
        resize(count, T());
    }

    void resize(size_type count, const T& value) {
        if (count > sz) {
            if (count > m_limit)
                reserve(grow_m_limit(count));
            std::uninitialized_fill(m_data + sz, m_data + count, value);
        } else if (count < sz) {
            for (pointer p = m_data + count; p != m_data + sz; ++p) {
                p->~T();
            }
        }
        sz = count;
    }

    // Additional Member Functions
    void assign(size_type count, const T& value) {
        clear();
        if (count > m_limit)
            reserve(count);
        std::uninitialized_fill_n(m_data, count, value);
        sz = count;
    }

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last) {
        clear();
        size_type count = std::distance(first, last);
        if (count > m_limit)
            reserve(count);
        std::uninitialized_copy(first, last, m_data);
        sz = count;
    }

    allocator_type get_allocator() const {
        return alloc;
    }

    size_type max_size() const noexcept {
        return std::allocator_traits<allocator_type>::max_size(alloc);
    }

    iterator insert(iterator pos, size_type count, const T& value) {
        difference_type index = pos - m_data;
        if (count == 0)
            return m_data + index;
        if (sz + count > m_limit)
            reserve(grow_m_limit(sz + count));
        pointer insert_pos = m_data + index;
        if (insert_pos < m_data + sz)
            std::copy_backward(insert_pos, m_data + sz, m_data + sz + count);
        std::uninitialized_fill_n(insert_pos, count, value);
        sz += count;
        return m_data + index;
    }

    template <typename InputIterator>
    iterator insert(iterator pos, InputIterator first, InputIterator last) {
        difference_type index = pos - m_data;
        difference_type count = std::distance(first, last);
        if (count == 0)
            return m_data + index;
        if (sz + count > m_limit)
            reserve(grow_m_limit(sz + count));
        pointer insert_pos = m_data + index;
        if (insert_pos < m_data + sz)
            std::copy_backward(insert_pos, m_data + sz, m_data + sz + count);
        std::uninitialized_copy(first, last, insert_pos);
        sz += count;
        return m_data + index;
    }

    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        difference_type index = pos - m_data;
        if (sz >= m_limit)
            reserve(grow_m_limit(sz + 1));
        pointer insert_pos = m_data + index;
        if (insert_pos < m_data + sz)
            std::move_backward(insert_pos, m_data + sz, m_data + sz + 1);
        std::allocator_traits<allocator_type>::construct(
            alloc, insert_pos, std::forward<Args>(args)...);
        ++sz;
        return m_data + index;
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (sz >= m_limit)
            reserve(grow_m_limit(sz + 1));
        std::allocator_traits<allocator_type>::construct(
            alloc, m_data + sz, std::forward<Args>(args)...);
        ++sz;
    }

    template <typename InputIterator>
    void append_range(InputIterator first, InputIterator last) {
        size_type count = std::distance(first, last);
        if (sz + count > m_limit)
            reserve(grow_m_limit(sz + count));
        std::uninitialized_copy(first, last, m_data + sz);
        sz += count;
    }

    void swap(Vector& other) noexcept {
        using std::swap;
        swap(m_data, other.m_data);
        swap(sz, other.sz);
        swap(m_limit, other.m_limit);
        swap(alloc, other.alloc);
    }

private:
    // Private member functions
    size_type grow_m_limit(size_type new_size) const {
        size_type new_m_limit = m_limit ? m_limit : 1;
        while (new_m_limit < new_size)
            new_m_limit *= 2;
        return new_m_limit;
    }

    pointer allocate(size_type count);

};

template <typename T>
typename Vector<T>::pointer Vector<T>::allocate(size_type count) {
    return std::allocator_traits<allocator_type>::allocate(alloc, count);
}

// Non-member functions
template <typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(rhs < lhs);
}

template <typename T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    return rhs < lhs;
}

template <typename T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs < rhs);
}

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename InputIterator, typename Predicate>
InputIterator erase_if(InputIterator first, InputIterator last, Predicate pred) {
    first = std::find_if(first, last, pred);
    if (first != last) {
        for (InputIterator it = first; ++it != last;) {
            if (!pred(*it)) {
                *first++ = std::move(*it);
            }
        }
    }
    return first;
}


