#pragma once

#include <set>

class StlAllocatorMockState
{
  public:

    StlAllocatorMockState(bool failAllAllocations = false)
        : m_allocatedBlocks(), m_failAllAllocations(failAllAllocations)
    {}

    ~StlAllocatorMockState()
    {
        AssertNoOutstandingBlocks();
    }

    void* Allocate(const size_t cb) const
    {
        if (m_failAllAllocations)
        {
            return nullptr;
        }

        void* pBlock = ::malloc(cb);
        m_allocatedBlocks.emplace(pBlock);
        return pBlock;
    }

    void Free(void* pv) const
    {
        if (0 == m_allocatedBlocks.erase(pv))
        {
            throw "Free of unknown block";
        }

        ::free(pv);
    }

    void AssertNoOutstandingBlocks() const
    {
        CHECK(m_allocatedBlocks.empty());
    }

    bool operator==(const StlAllocatorMockState& rhs) const
    {
        return this == &rhs;
    }

  private:
    mutable std::set<void*> m_allocatedBlocks;
    const bool m_failAllAllocations;
};
