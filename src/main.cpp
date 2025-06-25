#include <dirent.h>
#include <sys/stat.h>

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <utility>

#include "./fs.cpp"

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

void find_biggest_file(fs::node *root) {
  if (!root) return;

  fs::node *biggest = nullptr;
  fs::foreach (root, [&biggest](fs::node *n, int level) {
    if (n->type == 'd') return;

    if (biggest == nullptr || n->size > biggest->size) {
      biggest = n;
    }
  });

  std::cout << "\n" << biggest->full_path << " " << biggest->size << "\n";
}

void find_most_crowded_directory(fs::node *root) {
  fs::node *dir = nullptr;
  fs::foreach (root, [&dir](fs::node *n, int level) {
    if (n->type == 'd' && (dir == nullptr || n->n_files > dir->n_files)) {
      dir = n;
    }
  });
  std::cout << dir->full_path << " " << dir->n_files << "\n";
}

void find_empty_directories(fs::node *root) {
  fs::foreach (root, [](fs::node *n, int level) {
    if (n->type == 'd' && n->children.size() == 0) {
      std::cout << n->full_path << "\n";
    }
  });
}

//// Strings dos Menus Principal e da parte de Pesquisa

void exibirMenuPrincipal() {
  std::cout << "\n==============================\n"
               "       MENU PRINCIPAL         \n"
               "==============================\n"
               "1. Exibir a árvore completa\n"
               "2. Exportar a árvore para HTML\n"
               "3. Pesquisar\n"
               "0. Sair\n"
               "Escolha uma opção: ";
}

void exibirMenuPesquisa() {
  std::cout << "\n==============================\n"
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
        break;

      case 2:
        std::cout << "Exportar Arvore em HTML\n";
        break;

      case 3:
        do {
          exibirMenuPesquisa();
          std::cin >> opcaoPesquisa;

          switch (opcaoPesquisa) {
            case 1:
              std::cout << "\nMaior Arquivo:\n";
              find_biggest_file(root);
              break;

            case 2:
              std::cout << "\nArquivos com mais do que N bytes\n";
              // Funcao Arquivo com mais N Bytes
              break;

            case 3:
              std::cout << "\nPasta com mais arquivos\n";
              find_most_crowded_directory(root);
              break;

            case 4:
              std::cout << "\nArquivos por extensao\n";
              // Funcao para listar arquivos por extensão
              break;

            case 5:
              std::cout << "\nPastas vazias\n";
              find_empty_directories(root);
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