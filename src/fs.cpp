#include <dirent.h>
#include <fstream>
#include <iostream>
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

void export_HTML(fs::node *root, const std::string &filename = "estrutura.html") {
  std::ofstream html(filename);
  if (!html.is_open()) {
    std::cerr << "Erro ao criar o arquivo '" << filename << "'\n";
    return;
  }

  //cabeçalho HTML 
  html << "<!DOCTYPE html>\n<html>\n<head>\n"
       << "<meta charset=\"ISO-8859-1\">\n"  
       << "<title>Arvore de Diretorios</title>\n"
       << "<style>\n"
       << "body { font-family: monospace; background: #f0f0f0; padding: 20px; }\n"
       << ".dir { font-weight: bold; color: darkblue; }\n"
       << ".file { color: darkgreen; }\n"
       << "</style>\n</head>\n<body>\n"
       << "<h1>Arvore de Diretorios</h1>\n<pre>\n";

  fs::foreach(root, [&html](fs::node *n, int level) {
    std::string indent(level*4, ' '); 
    html << indent;

    if (n->type == 'd') {
      //dir em azul + nfilhos e size
      html << "<span class='dir'>" << n->name << "/</span> ";
      html << "(" << n-> children.size() << " filhos, " << n->size << " bytes)\n";
    } else {
      //file: em verde e size
      html << "<span class='file'>" << n->name << "</span> ";
      html << "(" << n->size << " bytes)\n";
    }
  });

  html << "</pre>\n</body>\n</html>\n";
  html.close();
}



}  // namespace fs
