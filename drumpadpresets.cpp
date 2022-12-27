#include "drumpadpresets.h"

namespace drumpad_presets
{

bool File::operator==(const File &other) const
{
    return filename == other.filename &&
           color == other.color &&
           stopOnRelease == other.stopOnRelease &&
           looped == other.looped &&
           choke == other.choke;
}

}
