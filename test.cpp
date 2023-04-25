#include <dirent.h>
#include <iostream>

int main() {
    DIR* dir;
    struct dirent* entry;
    if ((dir = opendir("../")) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            std::cout << entry->d_name << std::endl;
        }
        closedir(dir);
    } else {
        std::cerr << "Failed to open directory." << std::endl;
        return 1;
    }
    return 0;
}