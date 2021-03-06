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
/*-*- 参数 TYPE_KANDR TYPE_ALLMAN -*-*/
/*-*- FIXME: TYPE_KANDR 尚未完善。-*-*/
/*-*- FIXME: Case 的支持尚未完善。-*-*/
/*-*-   C语言不支持 // 单行注释。 -*-*/
/*-*-     K&R风格：我的缩进风格   -*-*/
/*-*-     BSD风格：左大括号换行   -*-*/
/*-*- 操作符左右空格，逗号后空格。-*-*/
/*-*- 注释和字符串中的内容不改变。-*-*/
/*-*-       缩进均使用Tab字符     -*-*/
/*-*-      变量名不做风格处理     -*-*/

char* ChangeCodingStyle(char* Source, int Type)
{
	/*-*- TODO: 完善TYPE_KANDR -*-*/
	int i, j, Total = -1, MultiNote = 0, LineNote = 0, String = 0, Tab = 0, Sharp = 0, Ignore = 0, Case = 0, If = 0, IfBracket = 0;
	char* Res = (char*)malloc(sizeof(char) * (strlen(Source) * 3));
	for (i = 0; Source[i]; ++i) {
		if (MultiNote || LineNote || String)
			Res[++Total] = Source[i];
		else {
			if (Source[i] == '/') {
				if (Source[i + 1] == '/')
					LineNote = 1;
				else if (Source[i + 1] == '*')
					MultiNote = 1;
			}
			if (MultiNote || LineNote || (i > 0 && Source[i - 1] == '\'' && Source[i + 1] == '\'') || (i > 1 && Source[i - 1] == '\\' && Source[i - 2] == '\'' && Source[i + 1] == '\'') || Ignore || Sharp) {
				Res[++Total] = Source[i];
			}
			else {
				switch (Source[i]) {
					case '^': case '&': case '|':
					case '<': case '>':
						/* Alone */
						if (Source[i - 1] != Source[i] && Source[i] != Source[i + 1]) {
							if (Source[i] == '&')
								Res[++Total] = Source[i];
							else {
								Res[++Total] = ' ';
								Res[++Total] = Source[i];
								Res[++Total] = ' ';
							}
						}
						/* Double */
						else if (Source[i - 1] == Source[i]) {
							Res[++Total] = Source[i]; Res[++Total] = ' ';
						}
						else if (Source[i] == Source[i + 1]) {
							Res[++Total] = ' '; Res[++Total] = Source[i];
						}
						break;
					case '+': case '-': case '*': case '/':
						Res[++Total] = Source[i];
						break;
					case '{':
						if (Type == TYPE_KANDR)
							Res[++Total] = ' ';
						else if (Type == TYPE_ALLMAN) {
							Res[++Total] = '\n';
							for (j = 0; j < Tab; ++j)
								Res[++Total] = '\t';
						}
						++Tab;
						Res[++Total] = Source[i];
						Res[++Total] = '\n';
						for (j = 0; j < Tab; ++j)
							Res[++Total] = '\t';
						break;
					case '}':
						--Total;
						--Tab;
						Res[++Total] = Source[i];
						Res[++Total] = '\n';
						for (j = 0; j < Tab; ++j)
							Res[++Total] = '\t';
						break;
					case 'e':
						Res[++Total] = Source[i];
						if (i > 2 && Source[i - 3] == 'e' && Source[i - 2] == 'l' && Source[i - 1] == 's')
							Res[++Total] = ' ';
						break;
					case 'f':
						Res[++Total] = Source[i];
						if (i > 0 && Source[i - 1] == 'i') {
							If = 1;
						}
						break;
					case ';':
						Res[++Total] = Source[i]; Res[++Total] = '\n';
						if (i > 4 && Source[i - 5] == 'b' && Source[i - 4] == 'r' && Source[i - 3] == 'e' && Source[i - 2] == 'a' && Source[i - 1] == 'k' && Case) {
							--Tab;
							--Case;
						}
						for (j = 0; j < Tab; ++j)
							Res[++Total] = '\t';
						break;
					case ',':
						Res[++Total] = Source[i]; Res[++Total] = ' ';
						break;
					case '\t': case '\v': case '\r': case '\n':
						break;
					case '#':
						Sharp = 1;
						Res[++Total] = Source[i];
						break;
					default:
						Res[++Total] = Source[i];
						break;
				}
			}
		}
		if (Source[i] == '"' && !MultiNote && !LineNote) {
			if (i > 0 && ((Source[i - 1] == '\'' && Source[i + 1] == '\'') || Source[i - 1] == '\\'))
				;
			else
				String = !String;
		}
		if (!String && !MultiNote && !LineNote && !((i > 0 && Source[i - 1] == '\'' && Source[i + 1] == '\'') || (i > 1 && Source[i - 1] == '\\' && Source[i - 2] == '\'' && Source[i + 1] == '\''))) {
			if (Source[i] == ')') {
				--Ignore;
				if (If) {
					--IfBracket;
					if (IfBracket == 0) {
						If = 0;
						Res[++Total] = ' ';
					}
				}
			}
			else if (Source[i] == '(') {
				++Ignore;
				if (If) {
					++IfBracket;
				}
			}
			else if (Source[i] == '\n') {
				Sharp = 0;
			}
		}
		if (Source[i] == '/' && Source[i - 1] == '*' && MultiNote) {
			MultiNote = 0;
			Res[++Total] = '\n';
			for (j = 0; j < Tab; ++j)
				Res[++Total] = '\t';
		}
	}
	Res[++Total] = 0;
	return Res;
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

#ifndef NDEBUG
int main()
{
	char s[1000005] = "", in[1000005];
	/*-*- Debug -*-*/
	while (gets(in)) {
		strcat(s, in);
		strcat(s, "\n");
	}
	/*puts("!!!");
	struct StringSet Hash;
	Init(&Hash);
	printf("%d\n", Exist(&Hash, "123"));
	Insert(&Hash, "123");
	printf("%d\n", Exist(&Hash, "123"));
	puts(RemoveNotes(s));
	printf("%d\n", CheckBrackets(s));*/
	puts(ChangeCodingStyle(s, TYPE_ALLMAN));
	/* puts(ChangeCodingStyle(s, TYPE_KANDR)); */
	return 0;
}
#endif
