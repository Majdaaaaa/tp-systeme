#ifndef MYSTRING_H
#define MYSTRING_H

#include <stddef.h>

// La structure string stocke une chaîne de caractères terminée par un '\0'
// dans un buffer de taille connue, pour éviter les débordements.

struct string {
  size_t capacity; // Taille du buffer .data
  size_t length;   // Taille de la chaîne, sans compter le '\0' terminal
  char * data;     // Contenu de la chaîne, avec un '\0' terminal
};

struct string * string_new(size_t capacity);
// Crée une nouvelle chaîne de longueur 0 et de capacité donnée, allouée
// sur le tas (c'est-à-dire avec malloc).

void string_delete(struct string * str);
// Détruit une chaîne, en libérant la mémoire occupée.

int string_append (struct string * dest, char * src);
// Met à jour dest en ajoutant src à la *fin*.
// Renvoie 0 en cas de réussite, -1 en cas d'échec.
// Doit échouer sans rien modifier si le buffer n'est pas assez grand.

void string_truncate (struct string * str, size_t nchars);
// Tronque la chaîne en enlevant les nchars derniers caractères
// (ou tous les caractères s'il y en a moins de nchars).

#endif // MYSTRING_H
