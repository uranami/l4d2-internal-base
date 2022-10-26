#pragma once

template < typename T >
struct UtlMemory
{
    T& operator[]( int i ) const noexcept
    {
        return _memory[ i ];
    }

    T* _memory;
    int _allocationCount;
    int _growSize;
};

struct UtlString
{
    UtlMemory<char> buffer;
    int length;

    const char* data() const noexcept
    {
        return buffer._memory;
    }
};
