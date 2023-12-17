#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

typedef struct{
    wchar_t* str;
    int size;
    int wordCount;
    wchar_t** words;
    wchar_t** punctuations;
} Sentence;

typedef struct{
    Sentence* sentences;
    int size;
    int count;
} Text;

Sentence* readSentence(){
    Sentence* sentence = (Sentence*)malloc(sizeof(Sentence));
    sentence->str = NULL;
    sentence->size = 0;
    return sentence;
}

void getCommand(int* command){
    wscanf(L"%d", command);
}

int getWords(wchar_t sentence[]) {
    int wordCount = 0;
    wchar_t sep[] = L" .,\t";
    for (int i = 0; i < wcslen(sentence); i++) {
        if (!wcschr(sep, sentence[i]) && (wcschr(sep, sentence[i + 1]) || sentence[i + 1] == L'\0')) {
            wordCount++;
        }
    }
    return wordCount;
}

void deleteLongSentences(Text* text) {
    for (int i = 0; i < text->count; i++) {
        if (text->sentences[i].wordCount > 10) {
            text->sentences[i].str = NULL;
        }
    }
}

void replaceEnding(wchar_t* word){
    int wordLen = wcslen(word);
    if (wordLen >= 4) {
        if (wcscmp(&word[wordLen - 4], L"ться") == 0){
            wcscpy(&word[wordLen - 4], L"тся");
            return;
        }
    }
    if (wordLen >= 3) {
        if (wcscmp(&word[wordLen - 3], L"тся") == 0){
            word = (wchar_t*) realloc(word, wordLen+2);
            wcscpy(&word[wordLen - 3], L"ться");
            word[wordLen + 1] = L'\0';
        }
    }
}

void changeTense(Text* text){
    for (int i = 0;i < text->count;i++){
        for(int k = 0;k < text->sentences[i].wordCount;k++){
            replaceEnding(text->sentences[i].words[k]);
        }
    }
}

void wordToUpper(wchar_t* string) {
    for (int i = 0; i < wcslen(string);i++){
        string[i] = towupper(string[i]);
    }
}

void findWordInOtherSentences(Text* text) {
    if (text->sentences[0].wordCount >= 2){
        wchar_t* copy = (wchar_t*) calloc(wcslen(text->sentences[0].words[1]) + 1, sizeof(wchar_t));
        wcscpy(copy,text->sentences[0].words[1]);
        wordToUpper(copy);
        for (int i = 1; i < text->count;i++){
            if (text->sentences[i].str != NULL) {
                for (int j = 0;j < text->sentences[i].wordCount;j++) {
                    wchar_t* copy_j = (wchar_t*) calloc(wcslen(text->sentences[i].words[j]) + 1, sizeof(wchar_t));
                    wcscpy(copy_j, text->sentences[i].words[j]);
                    wordToUpper(copy_j);
                    if (wcscmp(copy_j, copy) == 0) {
                        wprintf(L"\033[1;32m%ls\033[0m%ls", text->sentences[i].words[j],text->sentences[i].punctuations[j]);
                    }
                    else {
                        wprintf(L"%ls%ls", text->sentences[i].words[j],text->sentences[i].punctuations[j]);
                    }
                }
                wprintf(L"\n");
            }
        }
    }    
}

int cmp(const void* a, const void* b){
    Sentence* first = (Sentence*)a;
    Sentence* second = (Sentence*)b;

    return first->wordCount - second->wordCount;
}

void deleteDuplicates(Text* text) {
    wchar_t* copy_i;
    wchar_t* copy_j;
    for (int i = 0; i < text->count; i++) {
        if (text->sentences[i].str != NULL) {
            wchar_t* copy_i = (wchar_t*) calloc(wcslen(text->sentences[i].str) + 1, sizeof(wchar_t));
            wcscpy(copy_i, text->sentences[i].str);
            wordToUpper(copy_i);
            for (int j = i + 1; j < text->count; j++) {
                if (text->sentences[j].str != NULL) {
                    wchar_t* copy_j = (wchar_t*) calloc(wcslen(text->sentences[j].str) + 1, sizeof(wchar_t));
                    wcscpy(copy_j, text->sentences[j].str);
                    wordToUpper(copy_j);
                    if (wcscmp(copy_i,copy_j) == 0) {
                        text->sentences[j].str = NULL;
                    }
                }
            }
        }
    }
}

Text* createText(){
    Text* text = (Text*)malloc(sizeof(Text));
    text->sentences = (Sentence*)malloc(sizeof(Sentence));
    text->size = 0;
    text->count = 0;
    return text;
}

wchar_t* input(int* count){
    int size = 0;
    int capacity = 1;
    int flag = 0;
    wchar_t current_wch = getwchar();
    wchar_t* buf = (wchar_t*) malloc(sizeof(wchar_t));
    while(flag < 2){
        current_wch = getwchar();
        if (current_wch == L'\n') flag++;
        else flag = 0;
        if (size + 1 >= capacity){
            capacity *= 2;
            wchar_t* new_buf = (wchar_t*) realloc(buf,capacity * sizeof(wchar_t));
            buf = new_buf;
        }
        if (current_wch == L'.'){
            (*count)++;
        }
        buf[size++] = current_wch;
    }
    buf[size - 2] = L'\0';
    return buf;
}

void splitText(Text* text, wchar_t* input){
    text->sentences = (Sentence*) malloc(text->count * sizeof(Sentence));
    wchar_t* ptr;
    wchar_t* sentence = wcstok(input,L".", &ptr);
    int j = 0;
    while (sentence != NULL){
        while (iswspace(*sentence)){
            sentence++;
        }
        text->sentences[j].str = (wchar_t*)calloc(wcslen(sentence) + 2, sizeof(wchar_t));
        wcscpy(text->sentences[j].str, sentence);
        text->sentences[j].str[wcslen(sentence)] = L'.';
        text->sentences[j].size = wcslen(text->sentences[j].str);
        text->sentences[j].wordCount = getWords(sentence);
        j++;
        sentence = wcstok(NULL, L".", &ptr);
    }
}

void splitSentence(Text* text) {
    wchar_t* sep = L" .,";
    wchar_t* copy;
    int len = 0;
    for (int i = 0; i < text->count; i++) {
        len = wcslen(text->sentences[i].str);
        copy = wcsdup(text->sentences[i].str);
        text->sentences[i].words = (wchar_t**) malloc(text->sentences[i].wordCount * sizeof(wchar_t*));
        text->sentences[i].punctuations = (wchar_t**) malloc(text->sentences[i].wordCount * sizeof(wchar_t*));
        wchar_t* ptr;
        wchar_t* word = wcstok(copy, sep, &ptr);
        int j = 0;
        while (word != NULL) {
            wchar_t* wordCpy = (wchar_t*) calloc(wcslen(word)+1, sizeof(wchar_t));
            wcscpy(wordCpy, word);
            text->sentences[i].words[j] = wordCpy;
            j++;
            word = wcstok(NULL, sep, &ptr);
        }
        for (int k = 0; k < len; k++) {
            if (!wcschr(sep, text->sentences[i].str[k])) {
                text->sentences[i].str[k] = L'\0';
            }
        }
        j = 0;
        int none = 1;
        for (int k = 0; k < len; k++) {
            if (text->sentences[i].str[k] != L'\0' && none == 1) {
                text->sentences[i].punctuations[j] = &text->sentences[i].str[k];
                none = 0;
                j++;
            } else if (text->sentences[i].str[k] == L'\0') {
                none = 1;
            }
        }
    }
}

void output(Text* text, int value){
    if (value == 1){
        for (int i = 0; i < text->count;i++){
            for (int k = 0; k < text->sentences[i].wordCount;k++){
                wprintf(L"%ls%ls", text->sentences[i].words[k], text->sentences[i].punctuations[k]);
            }
            wprintf(L"\n");
        }
    }
    else{
        for (int i = 0; i < text->count; i++){
            if (text->sentences[i].str != NULL){
                wprintf(L"%ls\n", text->sentences[i].str);
            }
        }
    }
}

int main(){
    setlocale(LC_CTYPE,"");
    wprintf(L"Course work for option 5.9, created by Trofimov Vladislav.\n");
    int command;
    getCommand(&command);
    Text* text = createText();
    wchar_t* input_text = input(&(text->count));
    splitText(text,input_text);
    deleteDuplicates(text);
    switch(command){
        case 0:
            output(text,command);
            break;
        case 1:
            splitSentence(text);
            changeTense(text);
            output(text,command);
            break;
        case 2:
            splitSentence(text);
            findWordInOtherSentences(text);
            break;
        case 3:
            qsort(text->sentences,text->count,sizeof(Sentence),cmp);
            output(text,command);
        case 4:
            deleteLongSentences(text);
            output(text,command);
            break;
        case 5:
            wprintf(L"Функция 0: Вывод текста после первичной обязательной обработки.\nФункция 1: Изменить все слова в тексте заканчивающиеся на “ться” так чтобы они заканчивались на “тся” и наоборот.\nФункция 2: Вывести все предложения в которых встречается второе слово первого предложения. Данное слово необходимо выделить зеленым цветом.\nФункция 3: Отсортировать предложения по возрастанию количества слов в предложении.\nФункция 4: Удалить все предложения в которых больше 10 слов.\nФункция 5: Вывод справки о функциях, которые реализует программа.\n");
            break;
        default:
            wprintf(L"\033[1;31mError: Введённая команда не существует. Для ознакомления с командами введите 5.\033[0m\n");
            break;
    }
    int* x = (int*) calloc(10, sizeof(int));
}