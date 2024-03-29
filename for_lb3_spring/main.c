#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

int validator(const char* str, const char* file_name, int count){
    int flag = 0;
    if (strlen(file_name) == 5 && str[count] == file_name[0]){
            flag++;
    }
    return flag;
}

int not_cur_parent(char* d_name){
    if (strcmp(d_name, ".") != 0 && strcmp(d_name, "..") != 0){
        return 1;
    }
    else return 0;
}

char* pathcat(const char* dir_name, const char* file_name){
    char* full_path = (char*) calloc(strlen(dir_name) + strlen(file_name) + 2, sizeof(char));
    sprintf(full_path,"%s/%s", dir_name, file_name);
    return full_path;
}


void list_dir(const char* dir_name, const char* str, int index, FILE* file){
    DIR* dir = opendir(dir_name);
    if (dir){
        struct dirent* dir_element = readdir(dir);
        while (dir_element) {
            if (dir_element->d_type == DT_REG){
                if (validator(str, dir_element->d_name, index)){
                        fprintf(file, "%s/%s\n", dir_name, dir_element->d_name);
                }
            }
            else if (dir_element->d_type == DT_DIR && not_cur_parent(dir_element->d_name)){
                char* new_dir = pathcat(dir_name, dir_element->d_name);
                list_dir(new_dir, str, index, file);
            }
            dir_element = readdir(dir);
        }
        closedir(dir);
    }
    else
        printf("Can't open the directory\n");
}

int main() {
    char string[1000];
    const char* dir = "./tmp";
    fgets(string,1000,stdin);
    FILE* result_file = fopen("result.txt","w");
    result_file = fopen("result.txt","w");
    for (int i = 0; i < strlen(string); i++){
        list_dir(dir, string, i, result_file);
    }
    fclose(result_file);
    return 0;
}