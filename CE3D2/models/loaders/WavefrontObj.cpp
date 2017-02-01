#include "CE3D2/models/loaders/WavefrontObj.h"

#include "CE3D2/models/loaders/FileFormatException.h"


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

    std::vector<std::shared_ptr<LineModel>>
    load_wavefront_obj(std::string const& filename)
    {
        std::ifstream file(filename, std::ios::in);
        auto model = load_wavefront_obj(file);
        file.close();
        return model;
    }

    std::vector<std::shared_ptr<LineModel>>
    load_wavefront_obj(std::istream& stream)
    {
        // Build command map.
        std::unordered_map<std::string, WavefrontObjCommands> command_map = {
            {"v", WavefrontObjCommands::vertex},
            {"f", WavefrontObjCommands::face},
            {"o", WavefrontObjCommands::object},
            {"#", WavefrontObjCommands::comment}
        };

        std::vector<std::shared_ptr<LineModel>> models;
        std::shared_ptr<LineModel> current_model;

        std::cout << "!!! ENTERED !!!" << std::endl;

        while (true)
        {
            std::string line;
            std::getline(stream, line);

            std::istringstream line_stream(line);

            std::string command;
            line_stream >> command;

            if (line_stream.eof())
            {
                break;
            }
            else if (line_stream.fail())
            {
                // TODO Can this occur?
            }

            std::cout << "!!! IN LOOP !!!" << std::endl;

            WavefrontObjCommands cmd;
            try
            {
                cmd = command_map.at(command);
            }
            catch (std::out_of_range)
            {
                // Unknown command.
                // TODO Make proper error and message (including the invalid command)
                std::cout << "!!! ERROR !!! '" << command << "'" << std::endl;
                throw new FileFormatException("Unknown specifier: " + command);
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
                    // TODO This check should be done for all commandos.
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
