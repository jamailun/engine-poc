#pragma once

#include <filesystem>
#include <fstream>

namespace engine {

namespace fs = std::filesystem;
using recursive_directory_iterator = fs::recursive_directory_iterator;

/**
 * Get the child path of a folder.
 */
std::string get_child_path(const std::string& folder_path, const std::string& file_name);

/**
 * Get the child file of a folder.
 */
fs::directory_entry get_child_file(const std::string& folder_path, const std::string& file_name);

/**
 * Get the child file of a folder.
 */
fs::directory_entry get_child_file(std::string& folder_path, const char* file_name);

} // namespace engine
