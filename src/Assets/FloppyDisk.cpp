#include "AssetLoader.hpp"

#include <cassert>
#include <vector>

namespace Assets
{
namespace
{
constexpr unsigned char floppyDiskPixels[] = {
"############  "
"#  #   ## # # "
"#  #   ## #  #"
"#  #   ## #  #"
"#  #      #  #"
"#  ########  #"
"#            #"
"#  ########  #"
"# #        # #"
"# #        # #"
"# #        # #"
"# #        # #"
"# #        # #"
"##############"
};

constexpr int floppyDiskWidth = 14;
constexpr int floppyDiskHeight = 14;

}

Icon LoadFloppyDiskIconRGBA()
{
    size_t mappingSize = floppyDiskWidth * floppyDiskHeight;
    std::vector<unsigned char> pixels(mappingSize * 4);

    for (size_t i = 0; i < mappingSize; i++)
    {
        unsigned char symbol = floppyDiskPixels[i];
        unsigned char color[ 4 ] = {};
        switch (symbol)
        {
        case ' ': // transparent
            color[ 0 ] = 0;
            color[ 1 ] = 0;
            color[ 2 ] = 0;
            color[ 3 ] = 0;
            break;
        case '#': // black
            color[ 0 ] = 0;
            color[ 1 ] = 0;
            color[ 2 ] = 0;
            color[ 3 ] = 255;
            break;
        default:
            assert(false);
        }

        pixels[ i * 4 + 0 ] = color[ 0 ];
        pixels[ i * 4 + 1 ] = color[ 1 ];
        pixels[ i * 4 + 2 ] = color[ 2 ];
        pixels[ i * 4 + 3 ] = color[ 3 ];
    }

    Icon icon{pixels, floppyDiskWidth, floppyDiskHeight};
    return icon;
}

}