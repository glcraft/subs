# Subs - Application 1.0

## Présentation

SubsApp est un lecteur de fichier traitant les informations reçues avec le subs. Le programme lit un fichier ou un texte, cherche selon le critère de sélection puis traite la recherche en subs.

## Aide

```
subs <input_method> <input> <subs> [options...] [files...]
Input methods :
    regex       takes regex in input. subs variables refer 
                to the regex position (ex: $1;)

(no options for now...)

Regex available special variables
    $size;      number of regex captures
    $line;      line number
    $filename;  filename (same as files arguments)
```

## Exemple (via Bash)

Change les mots d'un fichiers en majuscule.
```bash
find . -type f | subsapp regex '(\w+)' '{uppercase($1;)}' myfile.txt
```