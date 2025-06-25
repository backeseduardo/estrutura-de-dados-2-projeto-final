#include <dirent.h>

#include <functional>
#include <string>
#include <vector>

namespace fs {
const int ERROR_SELF_REFERENCE = 1;
struct node {
  std::string name;
  std::string full_path;
  unsigned char type;
  int size;
  int n_files;
  std::vector<node *> children;
};

node *create(const std::string &name, const std::string &full_path,
             unsigned char type) {
  node *n = new node;
  n->name = name;
  n->full_path = full_path;
  n->type = type;
  n->size = 0;
  n->n_files = 0;
  return n;
}

int addchild(node *root, node *child) {
  if (root == child) return ERROR_SELF_REFERENCE;
  root->children.push_back(child);
  return -1;
}

void foreach (node *root, std::function<void(node *n, int level)> callback,
              int level = 0) {
  if (!root) return;
  callback(root, level);
  std::size_t size = root->children.size();
  if (size == 0) return;
  for (int i = 0; i < size; i++)
    fs::foreach (root->children[i], callback, level + 1);
}

bool is_dir(unsigned char d_type) { return d_type == DT_DIR; }

bool is_file(unsigned char d_type) { return d_type == DT_REG; }
}  // namespace fs
