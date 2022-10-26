#pragma once

namespace Game::Addresses
{
    void Setup();

    inline Hikari::Address_t Paint;

    inline Hikari::Address_t StartDrawing;
    inline Hikari::Address_t FinishDrawing;
    inline Hikari::Address_t IsPlayerDead;
}