#include <stdlib.h>
#include "stringUtil.h"

static int _compare(const void* a, const void* b) {
    return (*(char*)a - *(char*)b);
}

void StringUtil_ReplaceSubString(char* str1, const char* str2, const char* str3, char* saveStr){
    int str1Len = strlen(str1);
    int str2Len = strlen(str2);
    int str3Len = strlen(str3);

    // 创建一个临时字符数组，用于保存替换后的结果
    char tempStr[str1Len * (str3Len - str2Len) + 1];

    int i, j, k;
    int matchFound;

    for (i = 0, k = 0; i < str1Len; i++) {
        matchFound = 0;

        // 在 str1 中寻找 str2 的匹配
        if (i <= str1Len - str2Len) {
            for (j = 0; j < str2Len; j++) {
                if (str1[i + j] != str2[j]) {
                    break;
                }
            }

            if (j == str2Len) {
                // 找到了 str2 的匹配，将 str3 复制到临时字符数组中
                for (j = 0; j < str3Len; j++) {
                    tempStr[k++] = str3[j];
                }

                i += str2Len - 1;
                matchFound = 1;
            }
        }

        if (!matchFound) {
            // 在 str1 中未找到 str2 的匹配，将当前字符复制到临时字符数组中
            tempStr[k++] = str1[i];
        }
    }

    tempStr[k] = '\0';  // 在临时字符数组末尾添加空字符

    // 将临时字符数组中的结果复制回 str1
    strcpy(saveStr, tempStr);
}

void StringUtil_ReplaceCharWithSubString(char* str, char ch, const char* substring, char* saveStr){
    int strLen = strlen(str);
    int subLen = strlen(substring);
    int count = 0;

    // 计算需要插入的子字符串的总长度
    for (int i = 0; i < strLen; i++) {
        if (str[i] == ch) {
            count++;
        }
    }

    // 计算替换后的字符串的总长度
    int newLen = strLen + (subLen - 1) * count;

    // 创建一个临时字符数组，用于保存替换后的结果
    char tempStr[newLen + 1];

    int j = 0;
    for (int i = 0; i < strLen; i++) {
        if (str[i] == ch) {
            // 将子字符串插入到临时字符数组中
            for (int k = 0; k < subLen; k++) {
                tempStr[j++] = substring[k];
            }
        } else {
            // 复制原始字符到临时字符数组中
            tempStr[j++] = str[i];
        }
    }

    tempStr[j] = '\0';  // 在临时字符数组末尾添加空字符

    // 将临时字符数组中的结果复制回 str
    strcpy(saveStr, tempStr);
}

void StringUtil_ReplaceChar(char* str, char oldChar, char newChar, char* saveStr) {
    int length = strlen(str);
    strcpy(saveStr, str);

    for (int i = 0; i < length; i++) {
        if (saveStr[i] == oldChar) {
            saveStr[i] = newChar;
        }
    }
}

void StringUtil_ReplaceLettersWithChar(char* str, char replaceChar, char* saveStr) {
    int length = strlen(str);
    strcpy(saveStr, str);

    for (int i = 0; i < length; i++) {
        if (isalpha(str[i])) {
            saveStr[i] = replaceChar;
        }
    }
}

void StringUtil_ReplaceLettersWithString(char* str, const char* replaceStr, char* saveStr) {
    int strLen = strlen(str);
    int replaceLen = strlen(replaceStr);

    // 创建一个临时字符数组，用于保存替换后的结果
    char tempStr[strLen * replaceLen + 1];

    int tempIndex = 0;
    for (int i = 0; i < strLen; i++) {
        if (isalpha(str[i])) {
            // 将替换字符串复制到临时字符数组中
            for (int j = 0; j < replaceLen; j++) {
                tempStr[tempIndex++] = replaceStr[j];
            }
        } else {
            // 复制原始字符到临时字符数组中
            tempStr[tempIndex++] = str[i];
        }
    }

    tempStr[tempIndex] = '\0';  // 在临时字符数组末尾添加空字符

    // 将临时字符数组中的结果复制回 str
    strcpy(saveStr, tempStr);
}

void StringUtil_GetLettersAndNumber(char* str, char* saveStr){
    int j = 0;
    for(int i = 0; i < strlen(str); i++){
        if(str[i] >= 'a' && str[i] <= 'z'){
            saveStr[j++] = str[i];
        }else if(str[i] >= 'A' && str[i] <= 'Z'){
            saveStr[j++] = str[i];
        }else if(str[i] >= '0' && str[i] <= '9'){
            saveStr[j++] = str[i];
        }
    }
}

char* StringUtil_FindChar(char* str, char charCode){
    while(*str != '\0'){
        if(*str == charCode)
            return str;
        str++;
    }
    return NULL;
}

int StringUtil_Segment(char* inputStr, const char separators, char* saveStr1, char* saveStr2){
    char* _temp = StringUtil_FindChar(inputStr, separators);
    if(_temp != NULL){
        memcpy(saveStr1, inputStr, _temp - inputStr);
        saveStr1[_temp - inputStr] = '\0';
        strcpy(saveStr2, _temp + 1);
        return 0;
    }
    return -1;
}

int StringUtil_StringMath(char* inputStr, const char* matchStr[], int matchStrLen){
    for(int i = 0; i < matchStrLen; i++){
        if(strcmp(inputStr, matchStr[i]) == 0){
            return i;
        }
    }
    return -1;
}

int StringUtil_PreLettersAndNumber(char* inputStr, char* saveStr){
    int _j = 0;
    for(int i = 0; i < strlen(inputStr); i++){
        if((inputStr[i] >= 'A' && inputStr[i] <= 'Z') ||
           (inputStr[i] >= 'a' && inputStr[i] <= 'z') ||
           (inputStr[i] >= '0' && inputStr[i] <= '9')){
            saveStr[_j++] = inputStr[i];
        }
    }
    saveStr[_j] = '\0';

    return _j;
}

int StringUtil_PreLettersAndNumber(char* inputStr, int len,  char* saveStr){
    int _j = 0;
    for(int i = 0; i < len; i++){
        if((inputStr[i] >= 'A' && inputStr[i] <= 'Z') ||
           (inputStr[i] >= 'a' && inputStr[i] <= 'z') ||
           (inputStr[i] >= '0' && inputStr[i] <= '9')){
            saveStr[_j++] = inputStr[i];
        }
    }
    saveStr[_j] = '\0';

    return _j;
}
char* StringUtil_FindSubString(char* inputStr, char* subStr){
    int len = strlen(inputStr);
    int subLen = strlen(subStr);

    if (len < subLen) {
        return NULL;
    }

    for (int i = 0; i <= len - subLen; i++) {
        int j;
        for (j = 0; j < subLen; j++) {
            if (inputStr[i + j] != subStr[j]) {
                break;
            }
        }
        if (j == subLen) {
            return (char*)&inputStr[i];
        }
    }

    return NULL;
}

void StringUtil_Sort(char* inputStr){
    int len = strlen(inputStr);
    qsort(inputStr, len, sizeof(char), _compare);
}

int StringUtil_FilterLettersAndNumber(char* inputStr, char* saveStr){
    int _j = 0;
    for(int i = 0; i < strlen(inputStr); i++){
        if(!((inputStr[i] >= 'A' && inputStr[i] <= 'Z') ||
           (inputStr[i] >= 'a' && inputStr[i] <= 'z') ||
           (inputStr[i] >= '0' && inputStr[i] <= '9'))){
            saveStr[_j++] = inputStr[i];
        }
    }
    saveStr[_j] = '\0';

    return _j;
}