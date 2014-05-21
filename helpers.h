#define _HELPERS_H

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _TIME_H
#include <time.h>
#endif

#define TYPE_KANDR	1
#define TYPE_ALLMAN	2

char HelperMatch(char c)
{
	switch (c) {
		case ')':
			return '(';
		case ']':
			return '[';
		case '}':
			return '{';
		default:
			return 0;
	}
}

/*-*-       String HashSet        -*-*/
/*-*-          0. Init            -*-*/
/*-*-          1. Insert          -*-*/
/*-*-          2. Delete          -*-*/
/*-*-          3. Exist           -*-*/
/*-*-    每个方法都是就地操作     -*-*/
/*-*-         不处理冲突          -*-*/

#define SET_SIZE	100005
#define SET_PRIME	100003

struct StringSet
{
	short Table[SET_SIZE];
};

unsigned HashFunction(char *str)
{
	unsigned int hash = 0;
	while (*str)
		hash = (*str++) + (hash << 6) + (hash << 16) - hash;
	return hash % SET_PRIME;
}

void Init(struct StringSet* S)
{
	memset(S->Table, 0, sizeof(S->Table));
	return;
}
void Insert(struct StringSet* S, char *str)
{
	int Pos = HashFunction(str);
	S->Table[Pos] = 1;
	return;
}

void Delete(struct StringSet* S, char *str)
{
	int Pos = HashFunction(str);
	S->Table[Pos] = 0;
	return;
}

int Exist(struct StringSet* S, char *str)
{
	int Pos = HashFunction(str);
	return (int)S->Table[Pos];
}

