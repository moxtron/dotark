#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <cstdlib>

namespace fs = std::filesystem;

fs::path get_dotfiles_dir() {
    //get home dir and create dotfile directory
    fs::path home_dir = std::getenv("HOME");
    fs::path dotfiles_dir = home_dir / ".dotfiles";

    if (!fs::exists(dotfiles_dir)) {
        fs::create_directory(dotfiles_dir);
    }

    return dotfiles_dir;
}

std::string rename_dotfile(std::string dotfile) {
    return dotfile.replace(0,1, "dot-");
}
void manage_dotfile(const std::string& filepath) {
    fs::path source_path =fs::absolute(filepath);

    if (!fs::exists(source_path)) {
        throw std::runtime_error("File does not exist: " + source_path.string());
    }
    if (fs::is_symlink(filepath)) {
        throw std::runtime_error("Can't link a symlink: " + filepath);
    }
    fs::path dotfiles_dir = get_dotfiles_dir();
    fs::path filename = source_path.filename();
    fs::path dest_path = dotfiles_dir / rename_dotfile(filename);

    fs::rename(source_path, dest_path);
    fs::create_symlink(dest_path, source_path);

    std::cout << "Succesfully moved " << filename << " to " << dest_path << std::endl;
    std::cout << "Created symlink at " << source_path << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <dotfile>" << std::endl;
    }
    try {
        manage_dotfile(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
