#include "../framework.h"
#include "modules.h"

namespace Game
{
    Modules_t::Modules_t( HMODULE handle )
    {
        this->_info = Hikari::ModuleInfo_t( handle );
    }

    Modules_t::Modules_t( const std::string& name )
    {
        this->_info = Hikari::ModuleInfo_t( name );


        if (const auto relative_address = reinterpret_cast<std::uintptr_t>(_info.GetProc("CreateInterface")) + 0x5; relative_address > 0x5)
        {
            const auto interface_fn = relative_address + 4U + *reinterpret_cast<std::int32_t*>(relative_address);
            this->_interface_list = **reinterpret_cast<void***>(interface_fn + 0x6);
        }
    }

    Hikari::Address_t Modules_t::FindPattern( const char* pattern, bool relative ) const
    {
        return Hikari::Memory->FindPattern( this->_info, pattern, relative );
    }

    Hikari::Address_t Modules_t::GetVTablePointer( const std::string& vtable ) const
    {
        return Hikari::Memory->GetVTablePointer( this->_info, vtable );
    }
}
