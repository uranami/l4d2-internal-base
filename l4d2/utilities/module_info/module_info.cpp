#include "../lib.h"

Hikari::ModuleInfo_t::ModuleInfo_t(): _base_address( 0 ), _size( 0 ), _handle( nullptr )
{
}

Hikari::ModuleInfo_t::ModuleInfo_t( const std::string& module_name )
{
    GetModuleInfo( module_name );
}

#ifdef WINDOWS
Hikari::ModuleInfo_t::ModuleInfo_t( const HMODULE handle )
{
    GetModuleInfo( handle );
}
#endif

Hikari::ModuleInfo_t::~ModuleInfo_t()
{
    this->_segments.clear();
    this->_name.clear();
    this->_base_address = this->_size = 0;
}

void Hikari::ModuleInfo_t::GetModuleInfo( const std::string& module_name )
{
    #ifdef WINDOWS
    const auto handle = GetModuleHandleA( module_name.c_str() );
    if ( !handle )
        throw std::runtime_error( std::format( ( "Failed to get module handle for {}" ), module_name ) );

    const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>( handle );

    if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
        throw std::runtime_error( std::format( ( "Invalid dos magic for {}" ), module_name ) );

    const auto nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>( reinterpret_cast<std::uint8_t*>( handle ) + dos_header->e_lfanew );

    if ( nt_header->Signature != IMAGE_NT_SIGNATURE )
        throw std::runtime_error( std::format( ( "Invalid nt signature for {}" ), module_name ) );

    this->_handle = handle;
    this->_name = module_name;
    this->_base_address = reinterpret_cast<uintptr_t>( handle );
    this->_size = nt_header->OptionalHeader.SizeOfImage;

    auto section = IMAGE_FIRST_SECTION( nt_header );

    for ( auto i = 0; i < nt_header->FileHeader.NumberOfSections; i++, section++ )
    {
        const auto is_executable = ( section->Characteristics & IMAGE_SCN_MEM_EXECUTE ) != 0;

        if ( const auto is_readable = ( section->Characteristics & IMAGE_SCN_MEM_READ ) != 0; is_executable && is_readable )
        {
            const auto start = reinterpret_cast<uintptr_t>( handle ) + section->VirtualAddress;
            const auto size = ( std::min )( section->SizeOfRawData, section->Misc.VirtualSize );
            this->_segments.emplace_back( start, reinterpret_cast<std::uint8_t*>( start ), size );
        }
    }

    Logger->Success( std::format( ( "{} | base_addr:{:#09X} | size:{:#09X} | _segments.size():{}" ), _name, _base_address, _size, _segments.size() ) );
    #else
    // TODO: Linux support
    #endif
}

#ifdef WINDOWS
void Hikari::ModuleInfo_t::GetModuleInfo( HMODULE handle )
{
    if ( !handle )
        throw std::runtime_error( std::format( ( "Invalid module handle" ) ) );

    const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>( handle );

    if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
        throw std::runtime_error( std::format( ( "Invalid dos magic" ) ) );

    const auto nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>( reinterpret_cast<std::uint8_t*>( handle ) + dos_header->e_lfanew );

    if ( nt_header->Signature != IMAGE_NT_SIGNATURE )
        throw std::runtime_error( std::format( ( "Invalid nt signature" ) ) );

    this->_handle = handle;
    this->_name = ( "Hikari" );
    this->_base_address = reinterpret_cast<uintptr_t>( handle );
    this->_size = nt_header->OptionalHeader.SizeOfImage;

    auto section = IMAGE_FIRST_SECTION( nt_header );

    for ( auto i = 0; i < nt_header->FileHeader.NumberOfSections; i++, section++ )
    {
        if ( !strcmp( reinterpret_cast<char*>( section->Name ), ".text" ) )
        {
            const auto start = reinterpret_cast<uintptr_t>( handle ) + section->VirtualAddress;
            const auto size = ( std::min )( section->SizeOfRawData, section->Misc.VirtualSize );
            this->_segments.emplace_back( start, reinterpret_cast<std::uint8_t*>( start ), size, reinterpret_cast<const char*>( section->Name ) );
        }
    }

    Logger->Success( std::format( ( "{} | base_addr:{:#09X} | size:{:#09X} | _segments.size():{}" ), _name, _base_address, _size, _segments.size() ) );
    #else
    // TODO: Linux support
    #endif
}

std::string Hikari::ModuleInfo_t::Name()
{
    return _name;
}

std::vector<Hikari::ModuleInfo_t::Segment_t>& Hikari::ModuleInfo_t::Segments()
{
    return _segments;
}

std::uintptr_t Hikari::ModuleInfo_t::BaseAddress() const
{
    return _base_address;
}

std::size_t Hikari::ModuleInfo_t::Size() const
{
    return _size;
}

void* Hikari::ModuleInfo_t::Handle() const
{
    return _handle;
}

FARPROC Hikari::ModuleInfo_t::GetProc( std::string_view proc_name )
{
    if ( !this->_handle )
        throw std::runtime_error( std::format( ( "Invalid module handle for {} when getting ProcAddress" ), this->_name ) );

    #ifdef WINDOWS
    if ( const auto address = GetProcAddress( this->_handle, proc_name.data() ); address )
    {
        return address;
    }
    #else
    // TOOD: Linux support
    #endif

    return nullptr;
}
