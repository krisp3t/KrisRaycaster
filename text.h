#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <vector>

namespace KrisRaycaster
{
    using Byte_t = uint_fast8_t;

    static std::vector<Byte_t> ReadBytes(const std::string &filename)
    {
        std::ifstream inf{filename};
        std::vector<Byte_t> bytes;
        std::cout << "Current directory: " << std::filesystem::current_path() << "\n";
        if (!inf)
        {
            std::cerr << "Error: could not open file " << filename << std::endl;
            return bytes;
        }
        std::string content((std::istreambuf_iterator<char>(inf)),
                            std::istreambuf_iterator<char>()); // read file into a string
        std::istringstream iss(content);
        std::string hexValue;

        while (iss >> hexValue)
        {
            if (hexValue == "..")
            {
                bytes.push_back(0);
                continue;
            }
            auto byte = static_cast<Byte_t>(std::stoi(hexValue, nullptr, 16));
            bytes.push_back(byte);
        }
        return bytes;
    }
}