#ifndef CE3D2_MODELS_LOADERS_WAVEFRONTOBJ_H
#define CE3D2_MODELS_LOADERS_WAVEFRONTOBJ_H

#include <istream>
#include <fstream>
#include <bits/unordered_map.h>

#include "CE3D2/models/LineModel.h"


namespace CE3D2
{
namespace Models
{
namespace Loaders
{
    enum WavefrontObjCommands {
        vt, f, o
    };

    LineModel load_wavefront_obj(std::string filename)
    {
        std::ifstream file(filename, std::ios::in);
        LineModel model = load_wavefront_obj(file);
        file.close();
        return model;
    }

    LineModel load_wavefront_obj(std::istream& stream)
    {
        // Build command map.
        std::unordered_map<std::string, WavefrontObjCommands> command_map = {
            {"vt", WavefrontObjCommands::vt},
            {"f", WavefrontObjCommands::f},
            {"o", WavefrontObjCommands::o}
        };

        while (!stream.eof())
        {
            std::string command;
            stream >> command;

            try
            {
                command_map.at(command);
            }
            catch (std::out_of_range ex)
            {
                // Unknown command, ignore.
            }

            if (command == "#")
            {

            }
            else if (command == "vt")
            {

            }
            else
            {
                // Invalid command, throw error.
            }
        }
    }
}
}
}

#endif
