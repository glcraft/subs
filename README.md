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

- [x] Intégration module mathématique (ExprTk)
- [x] Intégration module mathématique (FParser)
- [ ] Intégration bibliothèque json (https://github.com/nlohmann/json)
  - prendra en charge le json en plus du regex
- [ ] Documentation sur le développement
- [ ] Traduction en anglais
- [ ] Ajout ICU (pour l'unicode et traitement avancée des caractères)