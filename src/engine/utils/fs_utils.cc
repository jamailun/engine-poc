#include <engine/utils/fs_utils.hh>

using namespace engine;

std::string engine::get_child_path(const std::string& folder_path, const std::string& file_name) {
    return folder_path + (folder_path.ends_with("/") ? "" : "/") + file_name;
}

fs::directory_entry engine::get_child_file(const std::string& folder_path, const std::string& file_name) {
    std::string new_path = get_child_path(folder_path, file_name);
    return fs::directory_entry (new_path);
}

fs::directory_entry engine::get_child_file(std::string& folder_path, const char* file_name) {
    return get_child_file(folder_path, std::string(file_name));
}
