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
    enum WavefrontObjCommands
    {
        comment, face, object, vertex
    };

    std::vector<std::shared_ptr<LineModel>> load_wavefront_obj(std::string const& filename)
    {
        std::ifstream file(filename, std::ios::in);
        auto model = load_wavefront_obj(file);
        file.close();
        return model;
    }

    std::vector<std::shared_ptr<LineModel>> load_wavefront_obj(std::istream& stream)
    {
        // Build command map.
        std::unordered_map<std::string, WavefrontObjCommands> command_map = {
            {"vt", WavefrontObjCommands::vertex},
            {"f", WavefrontObjCommands::face},
            {"o", WavefrontObjCommands::object},
            {"#", WavefrontObjCommands::comment}
        };

        std::vector<std::shared_ptr<LineModel>> models;
        std::shared_ptr<LineModel> current_model;

        while (!stream.eof())
        {
            std::string line;
            std::getline(stream, line);

            std::istringstream line_stream(line);

            std::string command;
            line_stream >> command;

            WavefrontObjCommands cmd;
            try
            {
                cmd = command_map.at(command);
            }
            catch (std::out_of_range)
            {
                // Unknown command.
                // TODO Make proper error and message (including the invalid command)
                throw new error_t();
            }

            switch (cmd)
            {
                case WavefrontObjCommands::comment:
                {
                    // TODO
                    break;
                }
                case WavefrontObjCommands::face:
                {
                    // TODO
                    break;
                }
                case WavefrontObjCommands::object:
                {
                    // TODO Check for empty strings.
                    std::string name;
                    line_stream >> name;
                    current_model = std::make_shared<LineModel>(name);
                    models.push_back(current_model);
                    break;
                }
                case WavefrontObjCommands::vertex:
                {
                    if (!current_model)
                    {
                        // Push back new model without name.
                        current_model = std::make_shared<LineModel>();
                        models.push_back(current_model);
                    }

                    std::vector<PrecisionType> vector;

                    while (true)
                    {
                        PrecisionType value;
                        line_stream >> value;

                        if (line_stream.eof())
                        {
                            break;
                        }
                        else if (line_stream.fail())
                        {
                            // TODO Throw error
                        }

                        vector.push_back(value);
                    }

                    current_model->vectors().emplace_back(vector.size());
                    std::copy(vector.cbegin(), vector.cend(), current_model->vectors().back().begin());

                    break;
                }
            }
        }

        return models;
    }
}
}
}

#endif
