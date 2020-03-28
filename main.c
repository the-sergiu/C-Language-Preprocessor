#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#define BUF_SIZE 256

char *concat(const char *s1, const char *s2)
{
	char *result;

	result = malloc(strlen(s1) + strlen(s2) + 1);
	if (result == NULL) {
		perror("Eroare\n");
		exit(EXIT_FAILURE);
	}
	strcpy(result, s1);
	strcat(result, s2);

	return result;
}
void removeChar(char *str, char garbage)
{
	char *src, *dst;

	for (src = dst = str; *src != '\0'; src++) {
		*dst = *src;
		if (*dst != garbage)
			dst++;
	}
	*dst = '\0';
}
/* Returns last index of x if it is present. Else returns -1. */
int findLastIndex(const char *str, char x)
{
	int index;
	unsigned int i;

	index = -1;
	for (i = 0; i < strlen(str); i++)
		if (str[i] == x)
			index = i;
	return index;
}
/* Function to replace a string with another string */
char *replace(char *s, char *src, char *dsn)
{
	int n = 0;
	int src_len = strlen(src);
	int dsn_len = strlen(dsn);
	char *result;
	char *p, *t;
	char *q;

	for (p = s; (p = strstr(p, src)) != NULL; p += src_len)
		n++;
	result = malloc(strlen(s) + n * (src_len - dsn_len) + 1);
	p = s;
	t = result;

	if (n != 0) {
		for (; (q = strstr(p, src)) != NULL; p = q + src_len) {
			memcpy(t, p, q - p);
			t += q - p;
			memcpy(t, dsn, dsn_len);
			t += dsn_len;
		}
	}
	strcpy(t, p);
	if (result != NULL)
		free(result);
	return t;
}
void replaceAll(char *str, const char *oldWord, const char *newWord)
{
	char *pos, temp[BUF_SIZE];
	int index = 0;
	int owlen;

	owlen = strlen(oldWord);
	/*
	 * Repeat till all occurrences are replaced.
	 */
	while ((pos = strstr(str, oldWord)) != NULL) {
		/* Bakup current line */
		strcpy(temp, str);
		/* Index of current found word */
		index = pos - str;
		/* Terminate str after word found index */
		str[index] = '\0';
		/* Concatenate str with new word */
		strcat(str, newWord);
		/* Concatenate str with remaining words after
		 *   oldword found index.
		 */
		strcat(str, temp + index + owlen);
	}
}
void freeString(char *string)
{
	if (string != NULL) {
		free(string);
		string = NULL;
	}
}
void checkAllocFile(FILE *file)
{
	if (file == NULL) {
		perror("Alocare fisier esuata!\n");
		exit(EXIT_FAILURE);
	}
}

void checkAllocString(char *string)
{
	if (string == NULL)
		perror("Nu s-a alocat spatiu pentru string!\n");
}

char *ifdef(char buf[BUF_SIZE], FILE *fin, ht_t *ht)
{
	char define[7], temp[30], sys[30];

	if (sscanf(buf, "%s%s", define, sys) == 0)
		exit(EXIT_FAILURE);
	strcpy(temp, sys);
	//ceva = strstr(buf, sys);
	//token = strtok(ceva, "\n");
	replaceAll(buf, define, "");
	replaceAll(buf, sys, "");
	if (ht_get(ht, temp) != NULL) {
		/* Daca e in HashMap */
	} else {
		/* Daca nu e in HashMap elimini pana la #endif */
		while (fgets(buf, BUF_SIZE, fin) != NULL) {
			if (strstr(buf, "#endif"))
				break;
		}
	}
	return buf;
}

void if_func(char buf[256], FILE *fin, ht_t *ht)
{
	char *token = NULL, *ceva;
	char sys[30], temp[30], temp2[30], define[10];
	int counter = 0;

	if (sscanf(buf, "%s%s", define, sys) == 0)
		exit(EXIT_FAILURE);
	/* Daca elementul exista in hashmap, il retinem */
	if (ht_get(ht, sys)) {
	/* Inlocuim valoarea cheii pentru testare conditie */
		strcpy(temp, ht_get(ht, sys));
	} else {
		/* Copiere pt testare conditie */
		strcpy(temp, sys);
	}
	/* Stergem randul cu directiva */
	ceva = strstr(buf, sys);
	token = strtok(ceva, "\n");
	replaceAll(buf, define, "");
	replaceAll(buf, sys, "");
	/* Daca conditia IF evalueaza FALSE */
	if (!atoi(temp)) {
		while (fgets(buf, BUF_SIZE, fin) != NULL) {
			while (fgets(buf, BUF_SIZE, fin) != NULL) {
				if (strstr(buf, "#endif") ||
				strstr(buf, "#else") ||
				strstr(buf, "#elseif") ||
				strstr(buf, "#elif"))
					break;
			}
			/* #ELSEIF || #ELIF */
			if (strstr(buf, "#elseif") || strstr(buf, "#elif")){
				sscanf(buf, "%s%s", define, sys);
				/* Daca elementul exista in hashmap, il retinem */
				if (ht_get(ht, sys)) {
					/* Inlocuim valoarea cheii  */
					strcpy(temp2, ht_get(ht, sys));
				} else {
					strcpy(temp2, sys);
				} /* Copiere pt testare conditie */
				/* Stergem randul cu directiva */
				ceva = strstr(buf, sys);
				token = strtok(ceva, "\n");
				replaceAll(buf, define, "");
				replaceAll(buf, sys, "");
				/* Daca conditia #elseif evalueaza false */
				if (!atoi(temp2)) {
					while (fgets(buf, BUF_SIZE, fin) != NULL) {
						if (strstr(buf, "#endif") || strstr(buf, "#else")
						|| strstr(buf, "#elseif") || strstr(buf, "#elif")) {
							break;
						}
					}
				} else { /* Daca conditia #elseif evalueaza true */
					/* Parcurgem si afisam tot pana la #endif */
					while (fgets(buf, BUF_SIZE, fin) != NULL) {
						if (strstr(buf, "#endif")) {
							counter = 1;
							break;
						} /* Daca gasim #else, stergem randul */
						else if (strstr(buf, "#else")) {
							sscanf(buf, "%s", define);
							ceva = strstr(buf, define);
							token = strtok(ceva, "\n");
							replaceAll(buf, define, "");
							/* Daca cond init = TRUE */
							while (fgets(buf, BUF_SIZE, fin) != NULL) {
								if (strstr(buf, "#endif")) {
									counter = 1;
									break;
								}
							}
							/* iesim din bucla de parcurgere */
							if (counter == 1) {
								counter = 0;
								break;
							}
						} else {
							fputs(buf, stdout);
						}
					}
					/* Daca s-a eliminat codul ce tine de #else
					iesim din bucla de parcurgere */
					if (counter == 1) {
						counter = 0;
						break;
					}
				}
			}
			if (strstr(buf, "#else")) {
				/* Daca gasim #else, stergem randul */
				sscanf(buf, "%s", define);
				ceva = strstr(buf, define);
				token = strtok(ceva, "\n");
				replaceAll(buf, define, "");
				/* Parcurgem si afisam tot pana la #endif */
				while (fgets(buf, BUF_SIZE, fin) != NULL) {
					if (strstr(buf, "#endif")) {
						counter = 1;
						break;
					} else {
						fputs(buf, stdout);
					}
				}
				/* Daca s-a eliminat codul ce tine de #else
				iesim din bucla de parcurgere */
				if (counter == 1) {
					counter = 0;
					break;
				}
			}
			else if (strstr(buf, "#endif")) {
				break;
			}
		}
	} else { /* Daca conditia evalueaza TRUE */
		while (fgets(buf, BUF_SIZE, fin) != NULL)
		{
			if (strstr(buf, "#endif"))
				break;
			else if (strstr(buf, "#else")) {
				/* Daca gasim #else, stergem randul */
				sscanf(buf, "%s", define);
				ceva = strstr(buf, define);
				token = strtok(ceva, "\n");
				replaceAll(buf, define, "");
				/* Daca cond initiala e TRUE, inseamna ca tot dupa
					else pana la endif trebuie sa dispara */
				while (fgets(buf, BUF_SIZE, fin) != NULL) {
					if (strstr(buf, "#endif")) {
						counter = 1;
						break;
					}
				}
				/* Daca s-a eliminat codul ce tine de #else
				iesim din bucla de parcurgere */
				if (counter == 1) {
					counter = 0;
					break;
				}
			} else fputs(buf, stdout);
		}
	}
}

char *define_func(char buf[BUF_SIZE], ht_t *ht)
{
	char sys[30], map[30], *ceva, *token, define[7];

	if(sscanf(buf, "%s %s %s ", define, sys, map) == 0)
		perror("Eroare sscanf!\n");
	/* Se citeste intregul string dupa sys */
	ceva = strstr(buf, map);
	/* Se elimina /n */
	token = strtok(ceva, "\n");
	ht_set(ht, sys, ceva);

	return buf;
}

char* include_func(char buf[BUF_SIZE], const char *infile)
{
	FILE *incl;
	int length = 0;
	char *token, *ceva, temp[30], sys[30], define[10];
	unsigned int i = 0;
	char path[50] = {'\0'}, aux[BUF_SIZE];

	/* Citim cei 2 termeni */
	sscanf(buf, "%s%s", define, sys);
	/* Eliminam " " din nume */
	if (sys != NULL)
		removeChar(sys, '\"');
	strcpy(temp, sys);
	/* Eliminam randul cu directiva */
	ceva = strstr(buf, sys);
	token = strtok(ceva, "\n");
	replaceAll(buf, define, "");
	replaceAll(buf, sys, "");
	removeChar(buf, '\"');
	/* Luam infile si obtinem path-ul */
	length = findLastIndex(infile, '/');
	for (i = 0; i < strlen(infile) - length + 3; i++)
	{
		path[i] = infile[i];
	}
	/* Incercam deschiderea fisierului din #include */
	incl = fopen(strcat(path, temp), "r+");
	/* Parcurgem biblioteca si o scriem la stdout */
	checkAllocFile(incl);
	while (fgets(aux, BUF_SIZE, incl) != NULL)
	{
		aux[strcspn(aux, "\n\r")] = '\0';
		fputs(aux, stdout);
	}
	fclose(incl);
	return buf;
}

int parser(const char *infile, const char *outfile, ht_t *ht, char** dir)
{
	FILE *fin = NULL, *fout = NULL;
	char buf[BUF_SIZE], aux[BUF_SIZE], *token = NULL, *ceva;
	char sys[30], define[10], value[10];
	int sareLinie = 0, i = 0;
	 /*Stdin - Stdout */
	if (infile == NULL && outfile == NULL) {
		/* Parsare efectiva */
		while (fgets(buf, BUF_SIZE, stdin) != NULL)
		{
			fputs(buf, stdout);
		}
	} else if (infile != NULL && outfile == NULL) {/* Fin - Stdout */
		fin = fopen(infile, "r+"); /* Deschidere fisiere */
		checkAllocFile(fin);  /* Parsare efectiva */
		while (fgets(buf, BUF_SIZE, fin) != NULL)
		{
			/* Conditie care elimina \n la inceputul outputului */
			if (sareLinie == 1)
				buf[strcspn(buf, "\n\r")] = '\0';
			/* Reinitializam variabila */
			sareLinie = 0;
			/* Cautare si inlcouire din HashMap in Buf */
			strcpy(aux, buf);

			token = strtok(aux, "( ),.-=+' '");
			while (token != NULL)
			{
				if (ht_get(ht, token) != NULL) {
					strcpy(value, ht_get(ht, token));
					replaceAll(buf, token, value);
				}
				token = strtok(NULL, "( ),.-=+' '");
			}
			/* DEFINE */
			if (strstr(buf, "#define")) { /*Daca randul citit contine #define */
				strcpy(aux, define_func(buf, ht));
				strcpy(buf, aux);
				sareLinie = 1;
			} /* Daca am avut define, sarim randul respectiv in output*/
			/* INCLUDE */
			if (strstr(buf, "#include")) {
				strcpy(aux, include_func(buf, infile));
				strcpy(buf, aux);
			}
			/* IFDEF */
			if (strstr(buf, "#ifdef")) {
				strcpy(aux, ifdef(buf, fin, ht));
				strcpy(buf, aux);
			}
			/* UNDEF */
			if (strstr(buf, "#undef")) {
				sscanf(buf, "%s%s", define, sys);
				ht_del(ht, sys);
				sareLinie = 1;
			}
			/* IF */
			if (strstr(buf, "#if"))
			{
				if_func(buf, fin, ht);
			}
			/* #ENDIF */
			if (strstr(buf, "#endif")) {
				sscanf(buf, "%s", define);
				ceva = strstr(buf, define);
				token = strtok(ceva, "\n");
				replaceAll(buf, define, "");
			}
			if (sareLinie == 0){
				fputs(buf, stdout);
			}
		}
	}
	else if (infile != NULL && outfile != NULL) /* Fin - Fout */
	{
		/* Deschidere fisiere */
		fin = fopen(infile, "r+");
		checkAllocFile(fin);
		fout = fopen(outfile, "w+");
		checkAllocFile(fout);
		/* Parsare efectiva */
		while (fgets(buf, BUF_SIZE, fin) != NULL)
		{
			fputs(buf, fout);
		}
	}
	/* Dealocari */
	if (token != NULL) {
		free(token);
	}
	if (fout != NULL) {
		fclose(fout);
	}
	if (fin != NULL) {
		fclose(fin);
	}
	return 0;
}

int main(int argc, char **argv)
{
	int i, j, max_item = 3, counter = 0;                          /* Counters */
	char **sysmap = NULL, **dir, *infile = NULL, *outfile = NULL; /* Arg */
	ht_t *ht = NULL;                                              /* Hashmap */
	char *token = NULL, *symbol = NULL, *mapping = "";            /* Token + Symbol-Mapping */
	/* Initializare cu NULL */
	dir = malloc(max_item * sizeof(char *));
	sysmap = malloc(max_item * sizeof(char *));
	for (j = 0; j < 3; j++){
		dir[j] = NULL;
	}
	for (j = 0; j < max_item; j++){
		sysmap[j] = NULL;
	}

	j = 0;

	/* Citirea parametrilor */
	for (i = 1; i < argc; i++)
	{       /* param -D */
		if (strncmp(argv[i], "-D", 2) == 0)
		{
			if (strlen(argv[i]) == 2)
			{   /*-D nelegat */
				sysmap[j] = malloc(sizeof(char) * strlen(argv[i + 1]) + 1);
				checkAllocString(sysmap[j]);
				strcpy(sysmap[j], argv[i + 1]);
				i = i + 1;
				j = j + 1;
			}
			else if (strlen(argv[i]) > 2)
			{   /*-D legat */
				sysmap[j] = malloc(sizeof(char) * strlen(argv[i]));
				checkAllocString(sysmap[j]);
				strcpy(sysmap[j], argv[i] + 2);
				j = j + 1;
			}
		}
		else if (strncmp(argv[i], "-I", 2) == 0)
		{   /* Param -I */
			if (strlen(argv[i]) == 2)
			{   /* -I nelegat */
				dir[j] = malloc(sizeof(char) * strlen(argv[i + 1]) + 1);
				checkAllocString(dir[j]);
				strcpy(dir[j], argv[i + 1]);
				j = j + 1;
				i = i + 1;
			}
			else if (strlen(argv[i]) > 2)
			{   /*-I legat */
				dir[j] = malloc(sizeof(char) * strlen(argv[i]) + 1);
				checkAllocString(dir[j]);
				strcpy(dir[j], argv[i] + 2);
				j = j + 1;
			}
		} /* param -o */
		else if (strncmp(argv[i], "-o", 2) == 0)
		{
			if (strlen(argv[i]) == 2)
			{   /* -o nelegat */
				outfile = malloc(sizeof(char) * strlen(argv[i + 1]) + 1);
				checkAllocString(outfile);
				strcpy(outfile, argv[i + 1]);
				i = i + 1;
			}
			else if (strlen(argv[i]) > 2)
			{   /* -o legat */
				outfile = malloc(sizeof(char) * strlen(argv[i]));
				checkAllocString(outfile);
				strcpy(outfile, argv[i] + 2);
			}
		} /* bad param */
		else if ((strncmp(argv[i], "-", 1) == 0) && (strncmp(argv[i], "-I", 2) != 0
				|| strncmp(argv[i], "-D", 2) != 0 || strncmp(argv[i], "-o", 2) != 0))
		{
			printf("bad param\n");
			exit(EXIT_FAILURE);
		}
		else if (strncmp(argv[i] + strlen(argv[i]) - 2, "in", 2) == 0)
		{   /* infile */
			infile = malloc(sizeof(char) * strlen(argv[i]) + 1);
			strcpy(infile, argv[i]);
		}
		else if (strncmp(argv[i] + strlen(argv[i]) - 3, "out", 3) == 0)
		{   /* outfile */
			outfile = malloc(sizeof(char) * strlen(argv[i]) + 1);
			strcpy(outfile, argv[i]);
		}
		else
			exit(EXIT_FAILURE);
	}

	ht = ht_create();

	for (j = 0; j < 3; j++)
	{
		if (sysmap[j] != NULL)
		{
			token = strtok(sysmap[j], "=");
			while (token != NULL)
			{
				if (counter == 0)
				{
					symbol = malloc(sizeof(char) * strlen(token) + 1);
					checkAllocString(symbol);
					strcpy(symbol, token);
					counter++;
				}
				else if (counter == 1)
				{
					mapping = malloc(sizeof(char) * strlen(token) + 1);
					checkAllocString(mapping);
					strcpy(mapping, token);
					counter = 0;
				}
				ht_set(ht, symbol, mapping);
				token = strtok(NULL, "-");
			}
		}
		if (symbol != NULL) {
			free(symbol);
			symbol = NULL;
		}
		if (strcmp(mapping, "") != 0) {
			free(mapping);
			mapping = "";
		}
	}
	/* Parsare */
	parser(infile, outfile, ht, dir);
	/* Dealocari */
	freeString(token);
	for (j = 0; j < max_item; j++)
		freeString(sysmap[j]);
	free(sysmap);
	for (j = 0; j < max_item; j++)
		freeString(dir[j]);
	free(dir);
	freeString(infile);
	freeString(outfile);
	if (ht != NULL) {
		ht_del_all(ht);
	}
	freeString(symbol);
}
