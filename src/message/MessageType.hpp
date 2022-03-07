#pragma once

namespace Message
{
    enum class Direction
    {
        Request = 1,
        Response
    };

    namespace Radio
    {
        enum class Type
        {
            Pair = 1,
            Register,
            Alive,
            State,
            Command
        };
    };
};