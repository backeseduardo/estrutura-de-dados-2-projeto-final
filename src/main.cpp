#include <dirent.h>
#include <sys/stat.h>

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <utility>

#include "./fs.cpp"

bool endsWith(const std::string &str, const std::string &end) {
  if (end.size() > str.size()) return false;
  return str.compare(str.size() - end.size(), end.size(), end) == 0;
}

void read_directory(fs::node *root, const std::string &path, int level = 0) {
  DIR *dir = opendir(path.c_str());
  if (!dir) {
    std::cout << "dir not found" << std::endl;
    exit(1);
  }

  struct dirent *entry;
  while ((entry = readdir(dir))) {
    std::string name = entry->d_name;
    auto d_type = entry->d_type;
    if (name == "." || name == "..") continue;
    // Ignora arquivos ocultos
    if (name.front() == '.') continue;
    if (!fs::is_dir(d_type) && !fs::is_file(d_type)) continue;

    std::string full_path = path + "/" + entry->d_name;

    auto child = fs::create(name, full_path, fs::is_dir(d_type) ? 'd' : 'f');
    fs::addchild(root, child);

    if (fs::is_dir(d_type)) {
      read_directory(child, full_path, level + 1);
      root->size += child->size;
    } else {
      // update file size
      struct stat statBuf;
      if (stat(full_path.c_str(), &statBuf) == -1) continue;
      child->size = statBuf.st_size;
      root->size += statBuf.st_size;
      root->n_files += 1;
    }
  }

  closedir(dir);
}

std::vector<fs::node *> find_biggest_files(fs::node *root) {
  fs::node *biggest = nullptr;
  std::vector<fs::node *> files;
  fs::foreach (root, [&biggest, &files](fs::node *n, int level) {
    if (n->type != 'f') return;
    if (biggest == nullptr) {
      biggest = n;
      files.push_back(n);
      return;
    }
    if (n->size == biggest->size) {
      files.push_back(n);
      return;
    }
    if (n->size > biggest->size) {
      biggest = n;
      files.clear();
      files.push_back(n);
    }
  });
  return files;
}

std::vector<fs::node *> find_most_crowded_directories(fs::node *root) {
  fs::node *dir = nullptr;
  std::vector<fs::node *> directories;
  fs::foreach (root, [&dir, &directories](fs::node *n, int level) {
    if (n->type != 'd') return;
    if (dir == nullptr) {
      dir = n;
      directories.push_back(n);
      return;
    }
    if (n->n_files == dir->n_files) {
      directories.push_back(n);
      return;
    }
    if (n->n_files > dir->n_files) {
      dir = n;
      directories.clear();
      directories.push_back(n);
    }
  });
  return directories;
}

std::vector<fs::node *> find_empty_directories(fs::node *root) {
  std::vector<fs::node *> directories;
  fs::foreach (root, [&directories](fs::node *n, int level) {
    if (n->type == 'd' && n->children.size() == 0) {
      directories.push_back(n);
    }
  });
  return directories;
}

std::vector<fs::node *> find_bigger_files_than(fs::node *root, int size) {
  std::vector<fs::node *> files;
  fs::foreach (root, [&size, &files](fs::node *n, int level) {
    if (n->type != 'f') return;
    if (n->size > size) files.push_back(n);
  });
  return files;
}

std::vector<fs::node *> find_files_by_extension(fs::node *root,
                                                const std::string &extension) {
  std::vector<fs::node *> files;
  fs::foreach (root, [extension, &files](fs::node *n, int level) {
    if (n->type != 'f') return;
    if (endsWith(n->name, extension)) files.push_back(n);
  });
  return files;
}

//// Strings dos Menus Principal e da parte de Pesquisa

void exibirMenuPrincipal() {
  std::cout << "==============================\n"
               "       MENU PRINCIPAL         \n"
               "==============================\n"
               "1. Exibir a árvore completa\n"
               "2. Exportar a árvore para HTML\n"
               "3. Pesquisar\n"
               "0. Sair\n"
               "Escolha uma opção: ";
}

void exibirMenuPesquisa() {
  std::cout << "==============================\n"
               "        MENU PESQUISA         \n"
               "==============================\n"
               "1. Maior arquivo\n"
               "2. Arquivos com mais do que N bytes\n"
               "3. Pasta com mais arquivos\n"
               "4. Arquivos por extensão\n"
               "5. Pastas vazias\n"
               "6. Voltar ao menu principal\n"
               "0. Sair do Programa\n"
               "Escolha uma opção: ";
}

int main(int argc, char *argv[]) {
  // Faz o carregamento da estrutura do sistema de arquivos para a memória
  std::filesystem::path path = (argc > 1)
                                   ? std::filesystem::weakly_canonical(argv[1])
                                   : std::filesystem::current_path();
  auto root = fs::create(path, path, 'd');
  read_directory(root, path);

  bool running = true;
  int opcaoPrincipal, opcaoPesquisa;

  do {
    exibirMenuPrincipal();
    std::cin >> opcaoPrincipal;

    switch (opcaoPrincipal) {
      case 1:
        std::cout << "\n";

        fs::foreach (root, [](auto n, int level) {
          std::cout << std::string(level * 4, ' ');
          std::cout << n->name;
          std::cout << " (";
          if (n->type == 'd') {
            std::cout << n->children.size();
            std::cout << " filhos, ";
          }
          std::cout << n->size << " bytes";
          std::cout << ")\n";
        });
        std::cout << "\n";
        break;

      case 2:
        std::cout << "Exportar Arvore em HTML\n";
        // TODO RAFAEL
        break;

      case 3:
        do {
          exibirMenuPesquisa();
          std::cin >> opcaoPesquisa;

          std::vector<fs::node *> biggest_files;
          std::vector<fs::node *> crowded_dirs;
          std::vector<fs::node *> empty_dirs;
          int size;
          std::vector<fs::node *> files_bigger_than;
          std::string extension;
          std::vector<fs::node *> files_extension;

          switch (opcaoPesquisa) {
            case 1:
              std::cout << "Maior(es) Arquivo(s):\n";
              biggest_files = find_biggest_files(root);
              for (auto f : biggest_files) {
                std::cout << f->full_path << " (" << f->size << " bytes)\n";
              }
              std::cout << "\n";
              break;

            case 2:
              std::cout << "Informe o tamanho em bytes: ";
              std::cin >> size;
              std::cout << "Arquivos com mais do que N bytes(N=" << size
                        << ")\n";
              files_bigger_than = find_bigger_files_than(root, size);
              for (auto f : files_bigger_than) {
                std::cout << f->full_path << " (" << f->size << " bytes)\n";
              }
              std::cout << "\n";
              break;

            case 3:
              std::cout << "Pasta com mais arquivos\n";
              crowded_dirs = find_most_crowded_directories(root);
              for (auto d : crowded_dirs) {
                std::cout << d->full_path << " (" << d->n_files << " filhos, "
                          << d->size << " bytes)\n";
              }
              std::cout << "\n";
              break;

            case 4:
              std::cout << "Informe a extensão: ";
              std::cin >> extension;
              std::cout << "Arquivos por extensao (" << extension << "):\n";
              files_extension = find_files_by_extension(root, extension);
              for (auto f : files_extension) {
                std::cout << f->full_path << " (" << f->size << " bytes)\n";
              }
              std::cout << "\n";
              break;

            case 5:
              std::cout << "Pastas vazias\n";
              empty_dirs = find_empty_directories(root);
              for (auto d : empty_dirs) {
                std::cout << d->full_path << "\n";
              }
              std::cout << "\n";
              break;

            case 6:
              std::cout << "\nVoltando ao menu principal...\n";
              break;

            case 0:
              std::cout << "\nSaindo...\n";
              running = false;
              break;

            default:
              std::cout << "Opcao Invalida! Digite Novamente.\n";
              break;
          }

        } while (opcaoPesquisa != 6 && running);

        break;

      case 0:
        std::cout << "\nSaindo...\n";
        std::cout << "Processo Finalizaddo...\n";
        running = false;
        break;

      default:
        std::cout << "Opcao Invalida, Digite Novamente!\n";
        break;
    }

  } while (running);
}