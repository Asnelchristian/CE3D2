#ifndef CE3D2_MODELS_LOADERS_WAVEFRONTOBJ_H
#define CE3D2_MODELS_LOADERS_WAVEFRONTOBJ_H

#include <istream>
#include <fstream>

#include "CE3D2/models/LineModel.h"


namespace CE3D2
{
namespace Models
{
namespace Loaders
{
    LineModel load_wavefront_obj(std::string filename)
    {
        std::ifstream file(filename, std::ios::in);
        LineModel model = load_wavefront_obj(file);
        file.close();
        return model;
    }

    LineModel load_wavefront_obj(std::istream& stream)
    {

    }
}
}
}

#endif
