#ifndef CE3D2_MODELS_LOADERS_WAVEFRONTOBJ_H
#define CE3D2_MODELS_LOADERS_WAVEFRONTOBJ_H

#include <fstream>
#include <istream>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "CE3D2/models/LineModel.h"


// TODO Document special capabilities:
// TODO   - As many values supported for `v`, not just 3 or 4
// TODO   - Material stmts and other commands are unsupported (usemtl, vn, vt, g, s, different
// TODO     face syntaxes like 2//1)
// TODO   - Comments have to be in a separate in a line and line has to start with `#`

namespace CE3D2
{
namespace Models
{
namespace Loaders
{
    std::vector<std::shared_ptr<LineModel>>
    load_wavefront_obj(std::string const& filename);

    std::vector<std::shared_ptr<LineModel>>
    load_wavefront_obj(std::istream& stream);
}
}
}

#endif
