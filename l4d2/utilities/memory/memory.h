#pragma once

namespace Hikari
{
    struct Memory_t
    {
        Address_t FindPattern( ModuleInfo_t info, const char* pattern, bool relative = false ) const;
        Address_t GetVTablePointer(ModuleInfo_t info, std::string table_name);

        template < size_t Index >
        static auto VirtualFunction( void* pInstance )
        {
            return ( *static_cast<uintptr_t**>( pInstance ) )[ Index ];
        }

        #ifdef WINDOWS
        template < typename T, size_t Index, typename... Args >
        constexpr T VirtualCall( void* pInstance, Args ... args )
        {
            using Fn = T(__thiscall*)( void*, decltype(args)... );
            return reinterpret_cast<Fn>( VirtualFunction<Index>( pInstance ) )( pInstance, args... );
        }

        template < typename T, size_t Index, typename... Args >
        constexpr T VirtualCallCdecl( void* pInstance, Args ... args )
        {
            using Fn = T(__cdecl*)( void*, decltype(args)... );
            return ( reinterpret_cast<Fn>( VirtualFunction<Index>( pInstance ) ) )( pInstance, args... );
        }
        #else
        template < typename T, size_t Index, typename... Args >
        constexpr T VirtualCall(void* pInstance, Args ... args)
        {
            using Fn = T(*)(void*, decltype(args)...);
            return reinterpret_cast<Fn>(VirtualFunction<Index>(pInstance))(pInstance, args...);
        }
        #endif

    private:
        static std::vector<int> PatternToBytes( const char* pattern );
        static std::uintptr_t FindPattern( const std::uint8_t* data, std::size_t size, const std::vector<int>& pattern );
        std::vector<Address_t> GetXrefsTo(uintptr_t address, uintptr_t start, uintptr_t size);
    };

    inline std::unique_ptr<Memory_t> Memory;
}
