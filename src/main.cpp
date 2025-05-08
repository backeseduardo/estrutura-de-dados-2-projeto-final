#include <iostream>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include "./fs.cpp"

void read_directory(fs::node *root, std::string &path, int level = 0)
{
  DIR *dir = opendir(path.c_str());
  if (!dir)
  {
    std::cout << "dir not found" << std::endl;
    exit(1);
  }

  struct dirent *entry;
  while ((entry = readdir(dir)))
  {
    std::string name = entry->d_name;
    auto type = entry->d_type;
    if (name == "." || name == "..")
      continue;
    // Ignore hidden files
    if (name.front() == '.')
      continue;
    if (!fs::is_dir(type) && !fs::is_file(type))
      continue;

    std::string fullPath = path + "/" + entry->d_name;
    auto child = fs::create(name, fs::is_dir(entry->d_type) ? 'd' : ' ', 0);
    fs::addchild(root, child);

    if (fs::is_dir(entry->d_type))
    {
      read_directory(child, fullPath, level + 1);
    }
  }

  closedir(dir);
}

int main(int argc, char *argv[])
{
  std::string path = (argc > 1) ? argv[1] : ".";
  auto root = fs::create(path, 'd', 0);
  read_directory(root, path);

  fs::foreach (root, [](auto n, int level)
               { std::cout << n->size << " " << std::string(level * 2, ' ') << n->name << std::endl; });
}
