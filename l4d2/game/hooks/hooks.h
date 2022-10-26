#pragma once

namespace Game
{
    #define CREATE_HOOK_DEFINE(name, type) \
    struct name { \
        using fn = std::remove_pointer_t<type>; \
        static fn Hook; \
        static fn* Original; \
    }; \
    inline name::fn* name::Original;

    CREATE_HOOK_DEFINE( PaintTraverse, void(__fastcall*)(void*, void*, unsigned int, bool, bool) );
    CREATE_HOOK_DEFINE( Paint, void(__fastcall*)(void*, void*, unsigned int) );
    CREATE_HOOK_DEFINE( CHLCreateMove, void(__fastcall*)(void*, void*, int, float, bool) );
    CREATE_HOOK_DEFINE( LevelInitPostEntity, void(__fastcall*)(void*, void*) );
    CREATE_HOOK_DEFINE( LevelShutdown, void(__fastcall*)(void*, void*) );

    struct Hook_t
    {
    private:
        struct Detour_t
        {
            Detour_t();
            ~Detour_t();

            void Hook( void* source, void* target, void** original, const std::string& name );
            void Unhook();

        private:
            void* _source;
            void* _target;
            std::string _name;
            bool _hooked;
        };

        Detour_t _PaintTraverse;
        Detour_t _Paint;
        Detour_t _FrameStageNotify;
        Detour_t _CHLCreateMove;
        Detour_t _LevelInitPostEntity;
        Detour_t _LevelShutdown;

    public:
        Hook_t();
        ~Hook_t();
    };

    inline std::unique_ptr<Hook_t> Hooks;
}
