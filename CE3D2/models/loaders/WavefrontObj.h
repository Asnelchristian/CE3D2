#ifndef CE3D2_MODELS_LOADERS_WAVEFRONTOBJ_H
#define CE3D2_MODELS_LOADERS_WAVEFRONTOBJ_H

#include <fstream>
#include <istream>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "CE3D2/models/LineModel.h"


namespace CE3D2
{
namespace Models
{
namespace Loaders
{
    enum WavefrontObjCommands
    {
        vertex, face, object
    };

    std::vector<std::shared_ptr<LineModel>> load_wavefront_obj(std::string filename)
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
            {"o", WavefrontObjCommands::object}
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
                // Unknown command, ignore line. Comments also count as
                // unknown, so they are properly ignored.
                continue;
            }

            switch (cmd)
            {
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
                            // TODO? Ignore strategy or throw error?
                        }

                        vector.push_back(value);
                    }

                    current_model->vectors().emplace_back(vector.size());
                    std::copy(vector.cbegin(), vector.cend(), current_model->vectors().back().begin());

                    break;
                }
                case WavefrontObjCommands::face:
                    break;
            }
        }

        return models;
    }
}
}
}

#endif
