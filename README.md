# Subs

## Présentation

Subs est un langage de substitution permettant de traiter les informations d'une recherche ou de données textuelles. 

Retrouvez les différentes documentations autour de ce projet.

* [Syntaxe](doc/syntaxe.md)
* [Application](doc/subsapp.md)

(Une documentation sur la partie développement est prévue)

## Exemple

### Texte en majuscule

Texte d'entrée 
```
Ceci est un exemple, et voici un texte.
```

Recherche en regex
```
(\w+)
```

Traitement en subs
```
{uppercase($1;)}
```

Résultat
```
CECI EST UN EXEMPLE, ET VOICI UN TEXTE.
```

## TODO

- [x] Intégration module mathématique (ExprTk) (1.1.0)
- [x] Intégration module mathématique (FParser) (1.1.1)
- [x] Intégration bibliothèque json (https://github.com/nlohmann/json) (1.2.0)
  - prendra en charge le json en plus du regex
- [x] Renommer des fichiers avec le programme (1.3.0)
- [ ] Documentation sur le développement
- [ ] Traduction en anglais
- [ ] Ajout ICU (pour l'unicode et traitement avancée des caractères)