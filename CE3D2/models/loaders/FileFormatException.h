#ifndef CE3D2_MODELS_LOADERS_FILEFORMATEXCEPTION_H
#define CE3D2_MODELS_LOADERS_FILEFORMATEXCEPTION_H

#include <stdexcept>


namespace CE3D2
{
namespace Models
{
namespace Loaders
{
    class FileFormatException: public std::runtime_error
    {
    public:
        FileFormatException(std::string const& message);
    };
}
}
}

#endif
