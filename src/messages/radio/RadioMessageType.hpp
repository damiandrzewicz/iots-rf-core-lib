#pragma once

enum class RadioMessageType
{
    Pair = 1,
    Register,
    Alive,
    BatteryState,
    GeneralState,
    Command
};