#pragma once

#include <limits>
#include <string>
#include <type_traits>

#include "StlAllocatorMockState.hpp"

template <class T>
class StlAllocatorMock
{
  public:
    typedef typename std::remove_const<T>::type value_type;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    template <class U> struct rebind
    {
        typedef StlAllocatorMock<U> other;
    };

    // The default constructor is deleted to detect any cases where the state
    // of the allocator will be lost.
    // TODO: Uncomment this to find incorrect uses of the constructor.
    //StlAllocatorMock() = delete;
    StlAllocatorMock()
    {
        FAIL("Default constructor should not be used.");
    }

    explicit StlAllocatorMock(const StlAllocatorMockState* pState)
        : m_pState(pState)
    {}

    template <class U>
    StlAllocatorMock(const StlAllocatorMock<U>& stlAllocator)
        : m_pState(stlAllocator.m_pState)
    {}

    StlAllocatorMock(const StlAllocatorMock& stlAllocator)
        : m_pState(stlAllocator.m_pState)
    {}

    StlAllocatorMock& operator=(const StlAllocatorMock& rhs)
    {
        if (this != &rhs)
        {
            m_pState = rhs.m_pState;
        }
        return *this;
    }

    ~StlAllocatorMock()
    {}

    pointer address(reference x) const
    {
        return std::addressof(x);
    }

    const_pointer address(const_reference x) const
    {
        return std::addressof(x);
    }

    pointer allocate(size_type count, const void* = nullptr)
    {
        pointer ptr = static_cast<pointer>(m_pState->Allocate(sizeof(T) * count));

        if (nullptr == ptr)
        {
            throw std::bad_alloc();
        }

        return ptr;
    }

    void deallocate(pointer p, size_type = 0)
    {
        m_pState->Free(p);
    }

    template <class U>
    void construct(pointer p, U&& val)
    {
        new(reinterpret_cast<void*>(p)) T(std::forward<U>(val));
    }

    size_type max_size() const
    {
        size_type count = std::numeric_limits<size_type>::max() / sizeof(T);
        return count > 0 ? count : 1;
    }

    void destroy(const_pointer p)
    {
        p->~T();
    }

    bool operator==(const StlAllocatorMock& rhs) const
    {
        return *m_pState == *rhs.m_pState;
    }

    bool operator!=(const StlAllocatorMock& rhs) const
    {
        return !(this->operator==(rhs));
    }

  private:
    // Required to allow the copy instructor for a different templated type to
    // access the private members of this class.
    template <typename U>
    friend class StlAllocatorMock;

    const StlAllocatorMockState* m_pState;
};
