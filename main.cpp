#include <iostream>
#include <format>
#include <fstream>
#include <filesystem>
#include <vector>
#include "config.h"
#include "gdelta.h"
#include "argh.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    argh::parser cmdl(argc, argv);

    if (cmdl[1] == "help")
    {
        std::cout << std::format("gdelta v{}.{}", gdelta_VERSION_MAJOR, gdelta_VERSION_MINOR) << std::endl;
        std::cout << "Usage: gdelta (d)iff  <OLD> <NEW> <DIF>" << std::endl;
        std::cout << "              (p)atch <OLD> <DIF> <NEW>" << std::endl;
        std::cout << "              help" << std::endl;
        return 0;
    }
    else if (cmdl[1] == "d" || cmdl[1] == "diff")
    {
        if (!cmdl(4))
        {
            std::cerr << std::format("{}: too few arguments", cmdl[0]) << std::endl;
            return 1;
        }

        fs::path old_path(cmdl[2]);
        fs::path new_path(cmdl[3]);
        fs::path dif_path(cmdl[4]);

        std::vector<char> old_buf;
        if (std::ifstream old_stream = std::ifstream(old_path, std::ios::binary | std::ios::ate))
        {
            old_buf.resize(old_stream.tellg());
            old_stream.seekg(0);
            old_stream.read(old_buf.data(), old_buf.size());
            std::cerr << std::format("old_stream: read {} bytes", old_buf.size()) << std::endl;
        }

        std::vector<char> new_buf;
        if (std::ifstream new_stream = std::ifstream(new_path, std::ios::binary | std::ios::ate))
        {
            new_buf.resize(new_stream.tellg());
            new_stream.seekg(0);
            new_stream.read(new_buf.data(), new_buf.size());
            std::cerr << std::format("new_stream: read {} bytes", new_buf.size()) << std::endl;
        }

        uint8_t *dif_buf = nullptr;
        uint32_t dif_buf_size = 0;

        gencode((uint8_t *)new_buf.data(), new_buf.size(), (uint8_t *)old_buf.data(), old_buf.size(), &dif_buf, &dif_buf_size);

        if (std::ofstream dif_stream = std::ofstream(dif_path, std::ios::binary | std::ios::trunc))
        {
            dif_stream.write((const char *)dif_buf, dif_buf_size);
        }

        return 0;
    }
    else if (cmdl[1] == "p" || cmdl[1] == "patch")
    {
        if (!cmdl(4))
        {
            std::cerr << std::format("{}: too few arguments", cmdl[0]) << std::endl;
            return 1;
        }

        fs::path old_path(cmdl[2]);
        fs::path dif_path(cmdl[3]);
        fs::path new_path(cmdl[4]);

        std::vector<char> old_buf;
        if (std::ifstream old_stream = std::ifstream(old_path, std::ios::binary | std::ios::ate))
        {
            old_buf.resize(old_stream.tellg());
            old_stream.seekg(0);
            old_stream.read(old_buf.data(), old_buf.size());
            std::cerr << std::format("old_stream: read {} bytes", old_buf.size()) << std::endl;
        }

        std::vector<char> dif_buf;
        if (std::ifstream dif_stream = std::ifstream(dif_path, std::ios::binary | std::ios::ate))
        {
            dif_buf.resize(dif_stream.tellg());
            dif_stream.seekg(0);
            dif_stream.read(dif_buf.data(), dif_buf.size());
            std::cerr << std::format("dif_stream: read {} bytes", dif_buf.size()) << std::endl;
        }

        uint8_t *new_buf = nullptr;
        uint32_t new_buf_size = 0;

        gdecode((uint8_t *)dif_buf.data(), dif_buf.size(), (uint8_t *)old_buf.data(), old_buf.size(), &new_buf, &new_buf_size);

        if (std::ofstream new_stream = std::ofstream(new_path, std::ios::binary | std::ios::trunc))
        {
            new_stream.write((const char *)new_buf, new_buf_size);
        }

        return 0;
    }
    else
    {
        std::cerr << std::format("{}: unknown subcommand", cmdl[0]) << std::endl;
        return 1;
    }
}