#pragma once

namespace Messages
{

namespace App
{
    struct Quit{};
}

namespace Canvas
{
    struct SaveToFile
    {
        std::string filename;
    };
    struct LoadFromFile
    {
        std::string filename;
    };
}

}
