# Subs - Application 1.2.0

## Présentation

SubsApp est un lecteur de fichier traitant les informations reçues avec le subs. Le programme lit un fichier ou un texte, cherche selon le critère de sélection puis traite la recherche en subs.

## Aide

```
Subs application 1.2.0
subs <input_method> <input> <subs> [options...] [files...]
Input methods :
    help        display this.
    regex       takes regex in input. subs variables refer 
                to the regex position (ex: $1;)
    json        takes json syntaxe. subs variables is a 
                json path (ex: $ob1.ob2[3].string;)
                Note : if file(s) is specified, 'input' 
                doesn't count, and vice versa...

Regex available special variables
    size        number of regex captures
    line        line number
    filename    filename (same as files arguments)
```

## Exemple (via Bash)

Change les mots d'un fichiers en majuscule.
```bash
find . -type f | subsapp regex '(\w+)' '{uppercase($1;)}' myfile.txt
```