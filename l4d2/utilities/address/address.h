#pragma once

#include <cstdint>

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define PAD( size ) uint8_t MACRO_CONCAT( _pad, __COUNTER__ )[ size ];

namespace Hikari
{
    template < typename type = std::uintptr_t >
    struct AddressBase_t
    {
        type ptr;

        AddressBase_t() : ptr{}
        {
        }

        AddressBase_t( type src ) : ptr{ src }
        {
        }

        AddressBase_t( type* src ) : ptr{ type( src ) }
        {
        }

        AddressBase_t( void* src ) : ptr{ type( src ) }
        {
        }

        AddressBase_t( const void* src ) : ptr{ type( src ) }
        {
        }

        ~AddressBase_t() = default;

        operator type() const
        {
            return ptr;
        }

        operator void*() const
        {
            return reinterpret_cast<void*>( ptr );
        }

        bool IsValid()
        {
            #ifdef ENVIRONMENT32
            return ptr >= 0x1000 && ptr < 0xFFFFFFEF;
            #else
            return ptr >= 0x1000 && ptr < 0x7FFFFFFEFFFF;
            #endif
        }

        template < typename T = type >
        T Cast()
        {
            return T( ptr );
        }

        template < typename T = AddressBase_t<type> >
        T Relative( ptrdiff_t offset = 0x1 )
        {
            // An explanation about how to get the actual address from relative signatures in case you are pepega:
            // Let's take these lines of opcodes as example:

            // .text:100683D2  E8 99 F5 FF FF                                                  call    sub_10067970            ; x86, function
            // .text:100683D7  83 C4 18                                                        add     esp, 18h

            // .text:1400C2786 48 8D 0D 33 EB D1 01                                            lea     rcx, qword_141DE12C0    ; x64, pointer/table or whaterver it is
            // .text:1400C278D E8 0E 04 68 00                                                  call    sub_140742BA0

            // In order to get the actual address, we need to get the relative address first
            // so we need to do base_address + offset = relative_address, which is 0x100683D2 + 1 = 0x100683D3 for x86 example, and 0x1400C2786 + 3 = 1400C2789 for x64 example
            // the reason why we are adding different offset is to skip the opcode(s) that we aren't interested in ( E8 for x86 example, 48 8D 0D for x64 example )
            // both lead to the address of where relative address starts
            // next, we need to dereference it to read the relative address ( the address will never reach to std::numeric_limits<std::int32_t>::max() so we don't have to worry about it )
            // so the actual_relative_address will be 0xFFFFF599 or 0x1D1EB33 from the example
            // then we can get the actual address with this formula:
            // (1)relative_signature_address + offset + sizeof(std::int32_t) + actual_relative_address = actual_address

            // (1): This refers to the address where the given signature points to, you can think it as 0x100683D2 or 0x1400C2786 from the example

            type base = ptr + offset;
            const auto relative_address = *reinterpret_cast<std::int32_t*>( base );
            base += sizeof( std::int32_t ) + relative_address;

            return T( base );
        }

        template < typename T = AddressBase_t<type> >
        T SelfRelative( ptrdiff_t offset = 0x1 )
        {
            ptr = Relative( offset );
            return *this;
        }

        template < typename T = AddressBase_t<type> >
        T FindOpcode( std::uint8_t opcode, bool forward = true )
        {
            auto base = ptr;

            while ( true )
            {
                if ( const auto current_opcode = *reinterpret_cast<std::uint8_t*>( base ); current_opcode == opcode )
                    break;

                forward ? ++base : --base;
            }

            return T( base );
        }

        template < typename T = AddressBase_t<type> >
        T SelfFindOpcode( std::uint8_t opcode, bool forward = true )
        {
            ptr = FindOpcode( opcode, forward );
            return *this;
        }

        template < typename T = AddressBase_t<type> >
        T Dereference( uint8_t count = 1 )
        {
            type base = ptr;
            while ( count-- )
            {
                if ( T( base ).IsValid() )
                    base = *reinterpret_cast<type*>( base );
            }

            return T( base );
        }

        template < typename T = AddressBase_t<type> >
        T SelfDereference( uint8_t count = 1 )
        {
            ptr = Dereference( count );
            return *this;
        }

        template < typename T = AddressBase_t<type> >
        T Offset( ptrdiff_t offset )
        {
            return T( ptr + offset );
        }

        template < typename T = AddressBase_t<type> >
        T SelfOffset( ptrdiff_t offset )
        {
            ptr = Offset( offset );
            return *this;
        }
    };

    using Address_t = AddressBase_t<std::uintptr_t>;

    // using Address32_t = AddressBase_t<std::uint32_t>;
    // using Address64_t = AddressBase_t<std::uint64_t>;
}
