#include <iostream>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[])
{
    DIR *dir = opendir("./");
    if (!dir)
    {
        closedir(dir);
        return 1;
    }

    dirent *file;
    while (file = readdir(dir))
    {
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
            continue;
        cout << file->d_name << " ";
        switch (file->d_type)
        {
        case DT_DIR:
            cout << "d" << " ";
            break;
        case DT_REG:
            cout << "f" << " ";
            struct stat sb;
            lstat(file->d_name, &sb);
            cout << sb.st_size;
            break;
        default:
            break;
        }
        cout << endl;
    }

    closedir(dir);
}