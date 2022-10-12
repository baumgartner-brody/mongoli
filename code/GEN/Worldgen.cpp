#include "Worldgen.h"

#include <iostream>
#include <fstream>
#include <direct.h>
#include <dirent.h>
#include <shlobj.h>

/* Currently windows only */

typedef std::basic_string<unsigned char> ustring; // unused

void WorldGen::test() {
    std::cout << "Called worldgen::test()\n";
    std::ofstream f("src/saves/test.txt");

    TCHAR appdata[MAX_PATH];

    SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdata);

    std::cout << "appdata: \"" << appdata << "\"\n";
    std::string s = (char*)appdata;
    std::cout << "s: " << s << '\n';
    

    mkdir((s + "\\.e").c_str());

    f << "Hello!\n";

    f.close();
}

int WorldGen::writeToFile(const std::string &fileName, const std::string &data) {

    std::ofstream f(fileName);

    int i = f.good();

    f << data;

    f.close();

    return i;

}

const std::vector<std::string> WorldGen::getDirectories(const std::string &root) {

    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> d_list;

    if ( (dir = opendir(root.c_str())) != NULL) {
        while ( (ent = readdir(dir)) != NULL) {

            /* Do not consider "." or ".." */
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;

            std::cout << root + '\\' + ent->d_name << '\n';
            DIR *d = opendir((root + '\\' + ent->d_name).c_str());
            if (d != NULL) {
                d_list.emplace_back(ent->d_name);
                closedir(d);
            }

        }
        closedir(dir);
    }

    return d_list; 

}