#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

#define TABLE_SIZE 1000

unsigned int hash(const char *key)
{
	unsigned long value = 0;
	unsigned int i = 0;
	unsigned int key_len = strlen(key);

	/* mai multe runde de inmultiri */
	for (i = 0; i < key_len; ++i)
		value = value * 37 + key[i];
	/* ne asiguram ca 0 <= value < TABLE_SIZE */
	value = value % TABLE_SIZE;
	return value;
}

entry_t *ht_pair(const char *key, const char *value)
{
	/* alocam o intrare (entry) */
	entry_t *entry = malloc(sizeof(entry_t) * 1);
	entry->key = malloc(strlen(key) + 1);
	entry->value = malloc(strlen(value) + 1);
	
	/* copiaza cheia si valoarea */
	strcpy(entry->key, key);
	strcpy(entry->value, value);
	/* next e initializat cu NULL, dar poate primi o valoare ulterior */
	entry->next = NULL;
	return entry;
}

ht_t *ht_create(void)
{
	int i = 0;
	/* alocam spatiu pentru tabela */
	ht_t *hashtable = malloc(sizeof(ht_t) * 1);

	/* alocam spatiu pentru intrarile din tabela */
	hashtable->entries = malloc(sizeof(entry_t *) * TABLE_SIZE);
	/* initializam fiecare intrare cu NULL */
	for (i = 0; i < TABLE_SIZE; ++i)
		hashtable->entries[i] = NULL;
	return hashtable;
}

void ht_set(ht_t *hashtable, const char *key, const char *value)
{
	unsigned int slot = hash(key);
	entry_t *prev;
	/* incercam sa cautam o intrare */
	entry_t *entry = hashtable->entries[slot];
	
	/* daca nu gasim nicio intrare, inseamna ca locul e liber */
	if (entry == NULL)
	{
		hashtable->entries[slot] = ht_pair(key, value);
		return;
	}
	/* parcurgem intrarile pana cand gasim cheia cautata sau
	   ajungem la sfarsitul listei */
	while (entry != NULL)
	{
		/* verificam cheie */
		if (strcmp(entry->key, key) == 0)
		{
			/* match found, replace value */
			free(entry->value);
			entry->value = malloc(strlen(value) + 1);
			strcpy(entry->value, value);
			return;
		}
		/* iteram la intrarea urmatoare */
		prev = entry;
		entry = prev->next;
	}
	/* daca nu s-a gasit nicio intrare cu acea cheie, adauga una noua */
	prev->next = ht_pair(key, value);
}

char *ht_get(ht_t *hashtable, const char *key)
{
	unsigned int slot = hash(key);

	/* incercam sa gasim o intrare valida */
	entry_t *entry = hashtable->entries[slot];

	/* daca nu gasim intrarea, inseamna ca locul e liber */
	if (entry == NULL)
	{
		return NULL;
	}
	/* parcurgem intrarile/itemii */
	while (entry != NULL)
	{
		/* returnam valoarea daca o gasim */
		if (strcmp(entry->key, key) == 0)
		{
			return entry->value;
		}
		/* iteram itemii */
		entry = entry->next;
	}
	/* nu s-a gasit itemul cu cheia cautata */
	return NULL;
}

void ht_del(ht_t *hashtable, const char *key)
{
	unsigned int bucket = hash(key);
	entry_t *prev;
	int idx = 0;

	/* incercam sa gasim un bucket valid */
	entry_t *entry = hashtable->entries[bucket];

	/* daca nu gasim bucket, inseamna ca locul e liber */
	if (entry == NULL)
	{
		return;
	}

	/* parcurgem itemii pana gasim unul cu aceeasi
		cheie sau pana ajungem la sfarist */
	while (entry != NULL)
	{
		/* verificam cheia */
		if (strcmp(entry->key, key) == 0)
		{
			/* first item and no next entry */
			if (entry->next == NULL && idx == 0)
			{
				hashtable->entries[bucket] = NULL;
			}

			/* primul item din intrarea next */
			if (entry->next != NULL && idx == 0)
			{
				hashtable->entries[bucket] = entry->next;
			}

			/* utlimul item */
			if (entry->next == NULL && idx != 0)
			{
				prev->next = NULL;
			}

			/* item din mijloc */
			if (entry->next != NULL && idx != 0)
			{
				prev->next = entry->next;
			}

			/* stergem itemul  */
			free(entry->key);
			free(entry->value);
			free(entry);

			return;
		}

		/* iteram prin lista */
		prev = entry;
		entry = prev->next;

		++idx;
	}
}

void ht_dump(ht_t *hashtable)
{
	int i = 0;
	for (i = 0; i < TABLE_SIZE; ++i)
	{
		entry_t *entry = hashtable->entries[i];

		if (entry == NULL)
		{
			continue;
		}

		printf("intrare[%2d]: ", i);

		for (;;)
		{
			printf("%s=%s\n", entry->key, entry->value);

			if (entry->next == NULL)
			{
				break;
			}

			entry = entry->next;
		}

	}
}

void ht_del_all(ht_t *hashtable)
{
	int i = 0;
	for (; i < TABLE_SIZE; ++i)
	{
		if (hashtable->entries[i] != NULL)
		{
			if ((hashtable->entries[i]->key) != NULL)
			{
				free(hashtable->entries[i]->key);
			}
			if ((hashtable->entries[i]->value) != NULL)
			{
				free(hashtable->entries[i]->value);
			}
			if (hashtable->entries[i] != NULL)
			{
				free(hashtable->entries[i]);
			}
		}
	}
	if (hashtable->entries != NULL)
	{
		free(hashtable->entries);
	}
	if (hashtable != NULL)
	{
		free(hashtable);
	}
}
