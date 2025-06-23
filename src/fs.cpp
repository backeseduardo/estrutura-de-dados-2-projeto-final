#include <functional>
#include <string>
#include <vector>

namespace fs {
const int ERROR_SELF_REFERENCE = 1;
struct node {
  std::string name;
  char type;
  int size;
  int hidden;
  std::vector<node *> _children;
};

node *create(std::string &name, char type) {
  node *n = new node;
  n->name = name;
  n->type = type;
  n->size = 0;
  n->hidden = 0;
  return n;
}

int addchild(node *root, node *child) {
  if (root == child) return ERROR_SELF_REFERENCE;
  root->_children.push_back(child);
  return -1;
}

using foreachcb = std::function<void(node *n, int level)>;

void foreach (node *root, foreachcb fn, int level = 0) {
  if (!root) return;
  if (root->hidden == 0) fn(root, level);
  std::size_t size = root->_children.size();
  if (size == 0) return;
  for (int i = 0; i < size; i++)
    fs::foreach (root->_children[i], fn, level + 1);
}

bool is_dir(unsigned char d_type) { return d_type == DT_DIR; }

bool is_file(unsigned char d_type) { return d_type == DT_REG; }
}  // namespace fs
