#!/bin/bash

# Créer un répertoire
mkdir -p test_links

# Créer des fichiers
echo "Fichier original 1" > test_links/file1.txt
echo "Fichier original 2" > test_links/file2.txt

# Créer des liens symboliques
ln -s ./file1.txt test_links/link_to_file1
ln -s /etc/passwd test_links/link_to_passwd
ln -s ../file2.txt test_links/link_to_file2_relative

# Vérification
echo "Contenu de test_links :"
ls -l test_links
