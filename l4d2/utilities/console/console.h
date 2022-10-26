#pragma once

namespace Hikari
{
    struct Console_t
    {
        Console_t();
        ~Console_t();

        bool Initialize();
        void Destory();
    private:
        void Log( std::string_view str );

    public:
        void Info( std::string_view str );

        void Error( std::string_view str );

        void Success( std::string_view str );

    private:
        bool _initialized = false;
    };

    inline std::unique_ptr<Console_t> Logger;
}
