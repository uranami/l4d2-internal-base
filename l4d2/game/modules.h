#pragma once

namespace Game
{
    struct Modules_t
    {
        explicit Modules_t(HMODULE handle);
        explicit Modules_t(const std::string& name);

    private:
        Hikari::ModuleInfo_t _info{};
        void* _interface_list{};

    public:
        Hikari::Address_t FindPattern(const char* pattern, bool relative = false) const;
        Hikari::Address_t GetVTablePointer(const std::string& vtable) const;

        void* GetInterfaceList() const
        {
            return this->_interface_list;
        }

        Hikari::ModuleInfo_t Info()
        {
            return this->_info;
        }
    };

    namespace Modules
    {
        inline std::unique_ptr<Modules_t> Client;
        inline std::unique_ptr<Modules_t> Engine;
        inline std::unique_ptr<Modules_t> Direct9;
        inline std::unique_ptr<Modules_t> MaterialSystem;
        inline std::unique_ptr<Modules_t> StudioRender;
        inline std::unique_ptr<Modules_t> VStdLib;
        inline std::unique_ptr<Modules_t> MatSurface;
        inline std::unique_ptr<Modules_t> VGui;
        inline std::unique_ptr<Modules_t> Server;
        inline std::unique_ptr<Modules_t> shaderapidx9;

    }
}