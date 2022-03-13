#pragma once

enum class RadioMessageType
{
    Pair = 1,
    Register,
    Alive,
    GeneralState,
    Command
};