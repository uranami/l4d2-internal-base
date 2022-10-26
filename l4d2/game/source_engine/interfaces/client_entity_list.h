#pragma once

struct IClientEntityList_t
{
    template < class T >
    T Get(int index)
    {
        return Hikari::Memory->VirtualCall<T, 3>(this, index);
    }

    template < class T >
    T GetFromHandle(int index)
    {
        return Hikari::Memory->VirtualCall<T, 4>(this, index);
    }

    auto GetHighestEntityIndex()
    {
        return Hikari::Memory->VirtualCall<int, 6>(this);
    }
};
