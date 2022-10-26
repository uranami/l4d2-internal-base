#pragma once

namespace Hikari
{
    struct ModuleInfo_t
    {
        struct Segment_t
        {
            std::uintptr_t address;
            std::uint8_t* data;
            std::size_t size;
            const char* name;
        };

        ModuleInfo_t();

        /**
         * \brief Getting module info (base_address, size, readable & executable segments) by name
         * \param module_name The module's name
         */
        explicit ModuleInfo_t( const std::string& module_name );

        #ifdef WINDOWS
        /**
         * \brief Getting module info (base_address, size, readable & executable segments) by handle
         * \param handle The module handle
         */
        explicit ModuleInfo_t( HMODULE handle );
        #endif
        ~ModuleInfo_t();

    private:
        std::string _name;
        std::vector<Segment_t> _segments;
        std::uintptr_t _base_address;
        std::size_t _size;
#ifdef WINDOWS
        HMODULE _handle;
#endif

        void GetModuleInfo( const std::string& module_name );
        #ifdef WINDOWS
        void GetModuleInfo( HMODULE handle );
        #endif
    public:
        std::string Name();
        std::vector<Segment_t>& Segments();
        std::uintptr_t BaseAddress() const;
        std::size_t Size() const;
        void* Handle() const;
        FARPROC GetProc( std::string_view proc_name );
    };
}
