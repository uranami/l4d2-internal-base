#include "../lib.h"
#include <iomanip>
#include <ios>
#include <sstream>

namespace Hikari
{
    Address_t Memory_t::FindPattern( ModuleInfo_t info, const char* pattern, bool relative ) const
    {
        if ( !strlen( pattern ) )
            return {};

        const auto bytes = PatternToBytes( pattern );

        for ( const auto segments = info.Segments(); const auto& [ address, data, size, name ] : segments )
        {
            if ( Address_t result = FindPattern( data, size, bytes ) )
            {
                return relative ? result.Relative() : result;
            }
        }

        return {};
    }

    std::vector<int> Memory_t::PatternToBytes( const char* pattern )
    {
        auto bytes = std::vector<int>{};
        const auto start = const_cast<char*>( pattern );
        const auto end = const_cast<char*>( pattern ) + strlen( pattern );

        for ( auto current = start; current < end; ++current )
        {
            if ( *current == '?' )
            {
                ++current;

                if ( *current == '?' )
                    ++current;

                bytes.push_back( -1 );
            } else
            {
                bytes.push_back( strtoul( current, &current, 16 ) );
            }
        }
        return bytes;
    }

    std::uintptr_t Memory_t::FindPattern( const std::uint8_t* data, std::size_t size, const std::vector<int>& pattern )
    {
        std::uintptr_t result{};
        for ( auto i = 0UL; i < size - pattern.size(); ++i )
        {
            bool found = true;

            for ( auto s = 0UL; s < pattern.size(); ++s )
            {
                if ( data[ i + s ] != pattern[ s ] && pattern[ s ] != -1 )
                {
                    found = false;
                    break;
                }
            }

            if ( found )
            {
                result = reinterpret_cast<uintptr_t>( &data[ i ] );
                break;
            }
        }
        return result;
    }

    std::string bytes_to_ida_pattern( byte* bytes, size_t size )
    {
        std::stringstream ida_pattern;
        ida_pattern << std::hex << std::setfill( '0' );
        for ( size_t i = 0; i < size; i++ )
        {
            if ( const int32_t current_byte = bytes[ i ]; current_byte != 255 )
                ida_pattern << std::setw( 2 ) << current_byte;
            else
                ida_pattern << std::setw( 1 ) << "?";

            if ( i != size - 1 )
                ida_pattern << " ";
        }

        return ida_pattern.str();
    }

    bool get_section_info( uintptr_t base_address, const std::string& section_name, uintptr_t& section_start, uintptr_t& section_size )
    {
        const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>( base_address );
        if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
            return false;

        const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS32>( base_address + dos_header->e_lfanew );
        if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
            return false;

        auto section_header = IMAGE_FIRST_SECTION( nt_headers );
        uint16_t number_of_sections = nt_headers->FileHeader.NumberOfSections;
        while ( number_of_sections > 0 )
        {
            // If we're at the right section
            if ( !strcmp( section_name.c_str(), reinterpret_cast<const char*>( section_header->Name ) ) )
            {
                section_start = base_address + section_header->VirtualAddress;
                section_size = section_header->SizeOfRawData;
                return true;
            }

            section_header++;
            number_of_sections--;
        }

        return false;
    }

    std::vector<Address_t> Memory_t::GetXrefsTo( uintptr_t address, uintptr_t start, uintptr_t size )
    {
        std::vector<Address_t> xrefs = {};

        // Convert the address over to an IDA pattern string
        const std::string ida_pattern = bytes_to_ida_pattern( ( byte* )&address, 4 );

        // Get the end of the section (in our case the end of the .rdata section)
        const uintptr_t end = start + size;
        while ( start && start < end )
        {
            auto xref = FindPattern( reinterpret_cast<std::uint8_t*>( start ), size, PatternToBytes( ida_pattern.c_str() ) );

            // If the xref is 0 it means that there either were no xrefs, or there are no remaining xrefs.
            // So we should break out of our loop, otherwise it will keep on trying to look for xrefs.
            if ( !xref )
                break;

            // We've found an xref, save it in the vector, and add 4 to start, so it wil now search for xrefs
            // from the previously found xref untill we're at the end of the section, or there aren't any xrefs left.
            xrefs.push_back( xref );
            start = xref + 4;
        }

        return xrefs;
    }

    Address_t Memory_t::GetVTablePointer( ModuleInfo_t info, std::string table_name )
    {
        const std::string type_descriptor_name = ( ".?AV" ) + table_name + ( "@@" );

        std::string ida_pattern = bytes_to_ida_pattern( ( unsigned char* )type_descriptor_name.data(), type_descriptor_name.size() );
        auto rtti_type_descriptor = FindPattern( info, ida_pattern.c_str() );
        if ( !rtti_type_descriptor )
            return {};

        rtti_type_descriptor.Offset( -0x8 );

        uintptr_t rdata_start = 0, rdata_size = 0;
        if ( !get_section_info( info.BaseAddress(), ".rdata", rdata_start, rdata_size ) )
            return {};

        const auto xrefs = GetXrefsTo( rtti_type_descriptor, rdata_start, rdata_size );
        for ( const uintptr_t xref : xrefs )
        {
            // xref - 0x8 = offset of this vtable in complete class (from top)
            // So if it's 0 it means it's the class we need, and not some class it inherits from (again, opening up client_panorama.dll in IDA will help you understand)
            const int32_t offset_from_class = *reinterpret_cast<int32_t*>( xref - 0x8 );
            if ( offset_from_class != 0 )
                continue;

            // We've found the correct xref, the object locator is 0xC bytes before the xref. (Again, client_panorama.dll yada yada yada)
            const uintptr_t object_locator = xref - 0xC;

            // Now we need to get an xref to the object locator, as that's where the vtable is located
            {
                // Convert the object locator address to an IDA pattern
                ida_pattern = bytes_to_ida_pattern( ( byte* )&object_locator, 4 );

                const uintptr_t vtable_address = FindPattern( reinterpret_cast<std::uint8_t*>( rdata_start ), rdata_size, PatternToBytes( ida_pattern.c_str() ) ) + 0x4;

                // Here I'm checking for <= 4 as we're adding 0x4 to it. So if the pattern scan returns 0 we still head the fuck out
                if ( vtable_address <= 4 )
                    return {};

                // We've now found the vtable address, however, we probably want a pointer to the vtable (which is in .text).
                // To do this, we're going to find a reference to the vtable address, and use that as pointer.

                // If you don't need a pointer to the vtable in your implementation however, just return vtable_address
                uintptr_t text_start = 0, text_size = 0;
                if ( !get_section_info( info.BaseAddress(), ( ".text" ), text_start, text_size ) )
                    return {};

                // Convert the vtable address to an IDA pattern
                ida_pattern = bytes_to_ida_pattern( ( byte* )&vtable_address, 4 );
                return FindPattern( reinterpret_cast<std::uint8_t*>( text_start ), text_size, PatternToBytes( ida_pattern.c_str() ) );
            }
        }

        // We for some odd reason didn't find any valid xrefs
        return {};
    }
}
