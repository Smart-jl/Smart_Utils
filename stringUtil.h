#ifndef __STRING_UTIL_H
#define __STRING_UTIL_H

#include <string.h>
#include <ctype.h>

/* 寻找单个字符 */
char* StringUtil_FindChar(char* str, char charCode);
/* 寻找子串返回子串的首地址 */
char* StringUtil_FindSubString(char* inputStr, char* subStr);
/* 替换字符串 */
void StringUtil_ReplaceSubString(char* str1, const char* str2, const char* str3, char* saveStr);
/* 替换单个字符 */
void StringUtil_ReplaceChar(char* str, char oldChar, char newChar, char* saveStr);
/* 单个字符替换为子串 */
void StringUtil_ReplaceCharWithSubString(char* str, char ch, const char* substring, char* saveStr);
/* 替换所有字母为字符 */
void StringUtil_ReplaceLettersWithChar(char* str, char replaceChar, char* saveStr);
/* 替换所有字母为字符串 */
void StringUtil_ReplaceLettersWithString(char* str, const char* replaceStr, char* saveStr) ;
/* 字符串分割 */
int StringUtil_Segment(char* inputStr, const char separators, char* saveStr1, char* saveStr2);
/* 字符串全匹配 */
int StringUtil_StringMath(char* inputStr, const char* matchStr[], int matchStrLen);
/* 保证字符串只有字母和数字 */
int StringUtil_PreLettersAndNumber(char* inputStr, char* saveStr);
int StringUtil_PreLettersAndNumber(char* inputStr, int len,  char* saveStr);
/* 字符串整理 */
void StringUtil_Sort(char* inputStr);
/* 过滤子母和数字 只留下符号 */
int StringUtil_FilterLettersAndNumber(char* inputStr, char* saveStr);
#endif