#ifndef CE3D2_MODELS_LOADERS_ALL_H
#define CE3D2_MODELS_LOADERS_ALL_H

#include <boost/filesystem/path.hpp>

#include "CE3D2/models/LineModel.h"
#include "CE3D2/models/loaders/WavefrontObj.h"


namespace CE3D2
{
namespace Models
{
namespace Loaders
{
    /// Loads a model from file.
    ///
    /// This function determines the right loader-function via the file
    /// extension. Current supported extensions and their formats are:
    /// - Wavefront Obj (`load_wavefront_obj()`)
    ///   - `.obj`
    ///
    /// @param filename The filename to load the model from.
    /// @returns        The loaded model.
    LineModel load(std::string filename)
    {
        std::string extension = boost::filesystem::path(filename).extension()
            .string().substr(1);

        if (filename == "obj")
        {
            return load_wavefront_obj(filename);
        }
        else
        {
            throw std::invalid_argument(
                "Unrecognized file extension: ." + extension);
        }
    }
}
}
}

#endif
