/*
 * Magimagi @ C Project
 *
 */

#ifndef _STRING_H
#include <string.h>
#endif
#ifndef _STDIO_H
#include <stdio.h>
#endif
#ifndef _STDLIB_H
#include <stdlib.h>
#endif
#ifndef _HELPERS_H
#include "helpers.h"
#endif

/*-*-       Functions Begin       -*-*/

/*-*-         Basic Level         -*-*/
/*-*-         1. 删除注释         -*-*/
/*-*- 删除多行注释和C++的单行注释 -*-*/
/*-*- 传入源代码字符串指针Source，-*-*/
/*-*- 源代码必须要包含换行符信息，-*-*/
/*-*- 返回一个字符串指针表示结果。-*-*/
/*-*- 传入的字符串不会被函数修改。-*-*/
/*-*- 用完传回的串后要记得Free掉。-*-*/

char* RemoveNotes(const char* Source)
{
	char* Res = (char*)malloc(sizeof(char) * (strlen(Source) + 5));
	int i, MultiNote = 0, LineNote = 0, String = 0, Total = -1;
	for (i = 0; Source[i]; ++i) {
		if (MultiNote || LineNote || (Source[i] == '\n' && (Total == -1 || (Total >= 0 && Res[Total] == '\n'))))
			;
		else {
			if (!String) {
				if (Source[i] == '/') {
					if (Source[i + 1] == '/')
						LineNote = 1;
					else if (Source[i + 1] == '*')
						MultiNote = 1;
				}
			}
			if (!LineNote && !MultiNote)
				Res[++Total] = Source[i];
		}
		if (Source[i] == '"' && !MultiNote && !LineNote) {
			if (i > 0 && ((Source[i - 1] == '\'' && Source[i + 1] == '\'') || Source[i - 1] == '\\'))
				;
			else
				String = !String;
		}
		if (i > 0 && MultiNote && Source[i] == '/' && Source[i - 1] == '*')
			MultiNote = 0;
		if (LineNote && Source[i] == '\n')
			LineNote = 0;
	}
	Res[++Total] = 0;
	return Res;
}

/*-*-         Basic Level         -*-*/
/*-*-         2. 括号匹配         -*-*/
/*-*- 返回第一个括号匹配失败位置。-*-*/
/*-*- 匹配成功返回-1，不修改原串。-*-*/
/*-*- 不会检查被注释的代码的匹配。-*-*/
/*-*- 不会检查字符和字符串的括号。-*-*/
/*-*- 会检查单引号和双引号的匹配。-*-*/

int CheckBrackets(const char* Source)
{
	int Len = strlen(Source);
	int* Stack = (int*)malloc(sizeof(int) * (Len + 5));
	int i, Tail = 0, MultiNote = 0, LineNote = 0, Res = -1, String = 0, Check = 0, StringPos = -1;
	for (i = 0; Source[i]; ++i) {
		if (MultiNote || LineNote || String)
			;
		else {
			if (Source[i] == '/') {
				if (Source[i + 1] == '/')
					LineNote = 1;
				else if (Source[i + 1] == '*')
					MultiNote = 1;
			}
			else if (Source[i] == '(' || Source[i] == '[' || Source[i] == '{') {
				if (i > 0 && Source[i - 1] == '\'' && Source[i + 1] == '\'')
					;
				else
					Stack[++Tail] = i;
			}
			else if (Source[i] == ')' || Source[i] == ']' || Source[i] == '}') {
				if (i > 0 && Source[i - 1] == '\'' && Source[i + 1] == '\'')
					;
				else {
					if (Tail > 0 && Source[Stack[Tail]] == HelperMatch(Source[i]))
						--Tail;
					else {
						Res = Stack[Tail];
						free(Stack);
						return Res;
					}
				}
			}
			else if (Source[i] == '\'') {
				Check = 0;
				if ((i + 2 < Len && Source[i + 2] == '\'') || (i + 3 < Len && Source[i + 1] == '\\' && Source[i + 3] == '\''))
					Check = 1;
				if (i > 1 && Source[i - 2] == '\'' && Source[i - 1] == '\\' && Source[i + 1] == '\'')
					Check = 1;
				if ((i > 1 && Source[i - 2] == '\'') || (i > 2 && Source[i - 2] == '\\' && Source[i - 3] == '\''))
					Check = 1;
				if (!Check) {
					free(Stack);
					return i;
				}
			}
		}
		if (Source[i] == '"' && !LineNote && !MultiNote) {
			if (i > 0 && ((Source[i - 1] == '\'' && Source[i + 1] == '\'') || Source[i - 1] == '\\'))
				;
			else {
				String = !String;
				StringPos = i;
			}
		}
		if (i > 0 && MultiNote && Source[i] == '/' && Source[i - 1] == '*')
			MultiNote = 0;
		if (LineNote && Source[i] == '\n')
			LineNote = 0;
	}
	if (Tail != 0)
		Res = Stack[Tail];
	if (String)
		Res = StringPos;
	free(Stack);
	return Res;
}

/*-*-         Basic Level         -*-*/
/*-*-         3. 编程风格         -*-*/
/*-*- XXX： 先检查括号匹配再调用。-*-*/
/*-*- 返回新字符串 使用完记得Free -*-*/
/*-*- 可选参数TYPE_KANDR TYPE_BSD -*-*/
/*-*-     K&R风格：我的缩进风格   -*-*/
/*-*-     BSD风格：左大括号换行   -*-*/
/*-*- 操作符左右空格，逗号后空格。-*-*/
/*-*- 注释和字符串中的内容不改变。-*-*/
/*-*-       缩进均使用Tab字符     -*-*/
/*-*-      变量名不做风格处理     -*-*/

char* ChangeCodingStyle(char* Source, int Type)
{
	/*-*- TODO -*-*/
	return Source;
}


/*-*-        Middle Level         -*-*/
/*-*-        1. 检查标识符        -*-*/


/*-*-        Middle Level         -*-*/
/*-*-        2. 资源利用性        -*-*/

/*-*-          Top Level          -*-*/
/*-*-        1. 高级标识符        -*-*/

/*-*-          Top Level          -*-*/
/*-*-         2. 错误提示         -*-*/

/*-*-          Top Level          -*-*/
/*-*-         3. 风格评分         -*-*/

/*-*-          Top Level          -*-*/
/*-*-         4. 程序对比         -*-*/

/*-*- Functions End -*-*/

#ifndef _RELEASE
int main()
{
	char s[10005] = "", in[10005];
	/*-*- Debug -*-*/
	while (scanf("%[^\n]", in) != EOF) {
		strcat(s, in);
		strcat(s, "\n");
	}
	struct StringSet Hash;
	Init(&Hash);
	printf("%d\n", Exist(&Hash, "123"));
	Insert(&Hash, "123");
	printf("%d\n", Exist(&Hash, "123"));
	puts(RemoveNotes(s));
	printf("%d\n", CheckBrackets(s));
	return 0;
}
#endif
