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

//// Strings dos Menus Principal e da parte de Pesquisa

void exibirMenuPrincipal()
{
  std::cout << "==============================" << std::endl;
  std::cout << "       MENU PRINCIPAL         " << std::endl;
  std::cout << "==============================" << std::endl;
  std::cout << "1. Exibir a árvore completa" << std::endl;
  std::cout << "2. Exportar a árvore para HTML" << std::endl;
  std::cout << "3. Pesquisar" << std::endl;
  std::cout << "0. Sair" << std::endl;
  std::cout << "Escolha uma opção: ";
}

void exibirMenuPesquisa()
{
  std::cout << "\n==============================" << std::endl;
  std::cout << "        MENU PESQUISA         " << std::endl;
  std::cout << "==============================" << std::endl;
  std::cout << "1. Maior arquivo" << std::endl;
  std::cout << "2. Arquivos com mais do que N bytes" << std::endl;
  std::cout << "3. Pasta com mais arquivos" << std::endl;
  std::cout << "4. Arquivos por extensão" << std::endl;
  std::cout << "5. Pastas vazias" << std::endl;
  std::cout << "6. Voltar ao menu principal" << std::endl;
  std::cout << "0. Sair do Programa" << std::endl;
  std::cout << "Escolha uma opção: ";
}


int main(int argc, char *argv[])
{
  std::string path = (argc > 1) ? argv[1] : ".";
  auto root = fs::create(path, 'd', 0);
  read_directory(root, path);

  fs::foreach (root, [](auto n, int level)
        {std::cout << n->size << " " << std::string(level * 2, ' ') << n->name << std::endl; });

  bool running = true;
  int opcaoPrincipal, opcaoPesquisa;

  do{

    exibirMenuPrincipal();
    std::cin >> opcaoPrincipal;

    switch (opcaoPrincipal)
    {
    case 1:
        std:: cout << "Exibir Arvore Completa\n";
        fs::foreach (root, [](auto n, int level)
               { std::cout << n->size << " " << std::string(level * 2, ' ') << n->name << std::endl; });
      break;
    
    case 2:
      std:: cout << "Exportar Arvore em HTML\n";
      break;

    case 3:
        do{
          exibirMenuPesquisa();
          std:: cin >> opcaoPesquisa;

          switch (opcaoPesquisa) {
            case 1:
                std::cout << "\nPesquisar Maior Arquivo\n";
                // Funcao Maior Arquivo
                break;

            case 2:
                std::cout << "\nArquivos com mais do que N bytes\n";
                // Funcao Arquivo com mais N Bytes
                break;

            case 3:
                std::cout << "\nPasta com mais arquivos\n";
                // Funcao para pasta com mais arquivos
                break;

            case 4:
                std::cout << "\nArquivos por extensao\n";
                // Funcao para listar arquivos por extensão
                break;

            case 5:
                std::cout << "\nPastas vazias\n";
                // Funcao para listar pastas vazias
                break;

            case 6:
                std::cout << "\nVoltando ao menu principal...\n";
                break;
            
            case 0:
              std:: cout << "\nSaindo...\n";
              running = false;
              break;

            default:
                std::cout << "Opcao Invalida! Digite Novamente.\n";
                break;
        }

        }while (opcaoPesquisa != 6 && running);

      break;
      
    case 0:
      std:: cout << "\nSaindo...\n";
      std:: cout << "Processo Finalizaddo...\n";
      running = false;
    break;

    default:
      std:: cout << "Opcao Invalida, Digite Novamente!\n";
      break;
    }

  }while(running);
}