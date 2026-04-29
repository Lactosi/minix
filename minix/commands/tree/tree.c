#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void print_tree(const char *path, int depth, int last[]) {
    DIR *dir = opendir(path);
    if (!dir) return;
    struct dirent *entry;
    struct stat st;

    char *names[1024];
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        names[count] = strdup(entry->d_name);
        count++;
    }

    closedir(dir);

    for (int i = 0; i < count; i++) {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, names[i]);

        lstat(fullpath, &st);

        for (int d = 0; d < depth; d++) {
            if (last[d])
                printf("    ");
            else
                printf("|   ");
        }

        if (i == count - 1) {
            printf("`-- %s\n", names[i]);
            last[depth] = 1;
        } else {
            printf("|-- %s\n", names[i]);
            last[depth] = 0;
        }

        if (S_ISDIR(st.st_mode)) {
            print_tree(fullpath, depth + 1, last);
        }

        free(names[i]);
    }
}

int main(int argc, char *argv[]) {
    const char *start_path = (argc > 1) ? argv[1] : ".";
    int last[100] = {0};
    printf("%s\n", start_path);
    print_tree(start_path, 0, last);
    return 0;
}
