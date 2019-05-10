# Subs - Syntaxe

## Expression

L'expression est un texte formaté et conditionné pour former un nouveau texte. celui ci prend en entrée une serie de valeur rangé dans des conteneurs et en sortie, un texte formaté au besoin. La syntaxe est particuliere mais lisible.
```
Bonjour $name;, {exists(age)?vous avez $age; aujourd'hui:nous ne connaissont pas votre age}
```
Résultat (nom=Nom prenom, age=23): `Bonjour Nom prenom, vous avez 23 aujourd'hui`

Plusieurs caractères spéciaux sont utilisés pour formater le texte final. Pour utiliser l'un de ces caractères dans le texte final, un caractère escape "\\" est prévu à cette effet.
```
Bonjour \$name\;, \{exists(age)?vous avez \$age\; aujourd'hui:nous ne connaissont pas votre age\}
```
Résultat : `Bonjour $name;, {exists(age)?vous avez $age; aujourd'hui:nous ne connaissont pas votre age}`

## Arguments - Variables

Les valeurs d'entrées sont utilisable grâce aux arguments. Les arguments dans une expression doivent commencer par un "$" et finir par un ";" : `$nom;`. Le nom de celle-ci doit être exacte (sensible à la casse, au espaces blancs et à tout autre caractère).
En fonction des conteneurs, le nom peut etre un nombre (pour un [vecteur stadard](http://fr.cppreference.com/w/cpp/container/vector) ou un [retour regex](http://en.cppreference.com/w/cpp/regex/regex_match) en C++), un texte (pour les [maps](http://fr.cppreference.com/w/cpp/container/map), toujours en C++) ou encore un objet (pour les json). 

Voir l'exemple dans [Expression](#expression)

**Note pour les developpeurs** : D'autres implémentations peuvent être facilement ajouté ou modifié si besoin en dérivant la classe mère ou un de ses enfants. Pour accéder à une variable, la fonction "access" doit être définie dans la liste des fonctions de la classe. Voir une des classe enfant pour un exemple. 

## Les blocs conditionnés

Dans le cas où un texte doit être affiché sous certaines conditions, un ou plusieurs blocs conditionnels peuvent être utilisés. Un bloc conditionné commence par "{" et finissent par "}". Les blocs fonctionnent presque de la même manière qu'un opérateur ternaire : entre les deux acollades, une condition est suivi d'un "?" puis du texte à afficher si la condition est vrai. Dans le cas où un texte particulier doit être affiché lorsque la condition est fausse, le bloc est suivi d'un ":" puis du texte à afficher : `{condition?texte vrai:texte faux}`. Des opérateurs sont disponible pour les conditions, à savoir **!** (not), **&** (and), **|** (or). La condition peut contenir autant d'espaces blancs, ceux-ci ne seront pas comptés. Les textes et les paramètres des conditions sont formatés en [expression](#expression).

Pour retrouver les fonctions d'analyses disponibles, voir la partie "[Conditions disponibles](#conditions-disponibles)"
```
les numéros sont {compare($num1;,$num2;)?identiques ($num1;):différents($num1;,$num2;)}
```

## Les fonctions

Tout comme les blocs conditionnés, les blocs de fonctions commencent par "{" et finissent par "}". La fonction peut avoir besoin de paramètres, ceux ci sont renseignable entre parenthèse "(" et ")" et si plusieurs paramèters sont nécéssaires, ceux ci devront être séparé d'une virgule ",". Le bloc de fonction se forme donc de la manière suivante : `{nom_fonction(param1,param2,...)}`. Ceux ci peuvent aussi être enchainé à la suite : `{fonction1()fonction2(texte1,texte2)}`. Les paramètres sont formatés comme une [expression basique](#expression) avant d'être passés à la fonction. 

A noter que les espaces blancs entre les fonctions ne sont pas comptés, et qu'une fonction sans nom est un commentaire (voir l'exemple ci dessous)

Pour retrouver les fonctions disponibles, voir la partie "[Fonctions disponibles](#fonctions-disponibles)"
```
{Uppercase(en maj-)   (voici un commentaire)Lowercase(-EN MIN)}
```
Resultat : `EN MAJ--en min`

# Fonctions spéciales

## Les fonctions lambdas

Une fonction spéciale a été intégré permettant d'éviter de répéter une action prédéfinie : les fonctions lambda. Grâce à celle-ci, il est possible de créer ses propres fonctions avec les moyens du bord. Avec un nom et ces actions dans son paramètre (sous forme d'une simple [expression](#expression)), vous pouvez les appeler n'importe où une fois définies.`{nom_lambda(actions)}`

Les paramètres des fonctions lambdas commence de 0, il n'y a pas de limite de nombre de parametre à ceux-ci. Pour comprendre, voir l'exemple ci dessous.

A noter qu'une fonction lambda peut en appeler une autre si celle ci a été défini avant et qu'une fonction lambda **1** défini dans une autre fonction lambda **2** ne peut être appelé à l'exterieur de lambda **1** du à sa portée.
```
{lambda(afficher_nom,{capitalize($1;)} {uppercase($0;)})}{afficher_nom($nom1;,$prenom1;)} et {afficher_nom($nom2;,$prenom2;)} se connaissent.
```
Resultat (nom1=van dame, prenom1=jean-claude, nom2=willis, prenom2=bruce): `Jean-Claude VAN DAME et Bruce WILLIS se connaissent.`

## La fonction "maths"

La fonction "maths" (ce nom change en fonction de l'implémentation) est une fonction traitant des formules mathématiques. Selon l'implémentation, la fonction peut calculer des formules standard (calcul basique et non prioritaire) jusqu'au formule scientifique (fonctions mathématiques et prioritaire). La fonction "maths" peut être utilisé en tant que fonction ou [expression](#expression). Si c'est une expression, tout résultat différent de 0 sera vrai.

Quelques exemples (avec l'intégration de la bibliothèque fparser) :
```
nb1 et 32 font {fparser($nb1;+32)}.
```
Résultat (nb1=4): `nb1 et 32 font 36.`
```
nb1 au carré fait {fparser($nb1;*$nb1;)}. Autre manière : {fparser($nb1;^2)}.
```
Résultat (nb1=4): `nb1 au carré fait 16. Autre manière : 16`

Si la fonction fparser est utilisé dans une condition, la condition sera vrai si la valeur est différente de 0.
```
nb1 et 32 sont {fparser($nb1;=32)?égaux:différent}.
```
Résultat (nb1=32): `nb1 et 32 sont égaux.`
```
nb1, nb2 et nb3 : {fparser($nb1;*$nb2;*nb3;)?Au moins une des valeurs est nulles:Elle sont toutes différente de zéro}.
```
Résultat (nb1=nb2=32, nb3=0): `nb1, nb2 et nb3 : Au moins une des valeurs est nulles.`
```
test {fparser($nb1;<100 & 32%2=0)?:non} passé.
```
Résultat (nb1=32): `test passé.`
## Les boucles
Les fonctions spéciales **repeat** et **while** offrent la capacité de répéter une expression plusieurs fois. 

Dans le cas de la fonction repeat (`repeat(nb,expression)`), le premier argument correspond au nombre de de fois que l'expression du second argument doit être répété. Le nombre est une expression, il est donc tout à fait possible de mettre un argument ou une fonction quelconque à condition que ce soit strictement un **nombre entier positif ou nul** (l'aide de la fonction intégrée "num" décrite dans [Fonctions disponibles](#fonctions-disponibles) peut être utile dans ce cas là). 
```
-{repeat({maths(2+2)},hey you\,)}-
```
Résultat : `-hey you,hey you,hey you,hey you,-`

La fonction while (`repeat(condition,expression)`) prend comme premier argument une condition. Tant que cette condition est vrai, l'expression est répété (détails des conditions dans [Les blocs conditionnés](#les-blocs-conditionnés)).
```
-{while(maths({counter()}<5),|hey you|)}-
```
Résultat : `-|hey you||hey you||hey you||hey you||hey you|-`

<hr/>

# Conditions disponibles

* **maths**(expr) : *Condition spéciale, le nom dépend de l'implémentation*. Traitement des nombres. Vrai si l'expression n'est pas nulle. Pour plus de détails, voir la partie "[La fonction maths](#la-fonction-maths)"

* **equals**(txt1,txt2): vrai si les deux textes sont égaux.
    ```
    {equals($numero;,0612345678)?Numéro identique:Numéro différent}
    ```
    Résultat (numero=0612345678): Numéro identique
    
* **empty**(txt): vrai si le texte est vide. Attention, si les valeurs n'existe pas, peut provoquer une erreur, tester l'existant avant.
    ```
    {empty($nom;$prenom;)?Personne non renseigné:Personne \: $nom; $prenom;}
    ```
    Résultat (nom et prenom existant mais vide): `Personne non renseigné`
* **exists**(name): retourne si la valeur existe (Note pour les développeurs : fonction à implémenter dans la classe enfant)
    ```
    Ex: {exists(valeur1)?valeur1 n'existe pas:valeur1=$valeur1;}
    ```
    Résultat (valeur1 inexistant): `valeur1 n'existe pas`

# Fonctions disponibles

* **access**(name) : *Fonction spéciale*. retourne la valeur de "name". même resultat que "$name;" (Note pour les développeurs : fonction à implémenter **obligatoirement** dans la classe enfant sous peine de ne pas pouvoir utiliser les variables.)
    ```
    -{access(nom)},$nom;-
    ```
    Résultat (name=valeur): `-valeur,valeur-`

* **lambda**(nom,fonction): *Fonction spéciale*. voir la partie "[Les fonctions lambdas](#les-fonctions-lambdas)"
* **maths**(expr): *Fonction spéciale, le nom dépend de l'implémentation*. voir la partie "[La fonction maths](#la-fonction-maths)"
* (txt): *Fonction spéciale*. Commentaire
    ```
    -{(commentaire non affiché)}-
    ```
    Résultat: `--`
* **uppercase**(txt) : met toute la chaine en majuscule. Exemple :
    ```
    {uppercase(en majuscule)}
    ```
    Résultat: `EN MAJUSCULE`
* **lowercase**(txt) : met toute la chaine en majuscule. Exemple :
    ```
    {lowercase(EN MINUSCULE)}
    ```
    Résultat: `en minuscule`
* **capitalize**(txt): Pour chaque mot, le premier caractère en majuscule, le reste en minuscule. Exemple :
    ```
    {capitalize(JE SUIS en capiTALisé)}
    ```
    Résultat: `Je Suis En Capitalisé`
* **camelcase**(txt): Pour chaque mot, le premier caractère en majuscule, le reste en minuscule. Exemple :
    ```
    {capitalize(JE SUIS en caMElCASe)}
    ```
    Résultat: `je Suis En Camelcase`
* **trim**(txt): enlève les espaces avant et apres la chaine. Exemple :
    ```
    Ex: -{trim(   texte   )}-
    ```
    Résultat: `-texte-`
* **alpha-alphanum-num**(txt) : retourne les caractères correspondants au nom de la méthode. Exemple :
    ```
    -{trim(Alphanum(abdt43987er°+=+=3242))}-
    ```
    Résultat: `-abdt43987er3242-`
* **count_chars** : Compte le nombre de caractère dans la chaine. Exemple :
    ```
    {maths({count_chars($num_tel;)}=10)?numéro de téléphone=$num_tel;:Le numéro de téléphone est mal formé}.
    ```
    Résultat (num_tel=061234): `Le numéro de téléphone est mal formé.`
* **line_number**() : En fonction de l'implémentation, donne la ligne actuelle lors de l'analyse
* **counter**() : Est un simple compteur. S'incrémente à chaque utilisation. Exemple:
    ```
    -{counter()}-{counter()}-{counter()}-
    ```
    Résultat : `-1-2-3-`
* **shuffle**(txt) : Mélange la chaine
    ```
    -{shuffle(hello world)}-
    ```
    Résultat : `-llerw hdloo-`
* **randomize**(nbchars,(opt)chars) : Crée une chaine de caractère long de "nbchars" avec les caractères dans "chars". Si chars n'est pas défini, les lettres minuscule et majuscule, les nombres et l'espace sont pris par défaut pour former la chaine.
    ```
    -{randomize(20)}-{randomize(10,0123456789ABCDEF)}-
    ```
    Résultat : `-AwtcLTkIdAsNmfOXNLcU-432FCD004E-`
* **base64_encode**(txt) : convertit la chaine passé en base64.
    ```
    -{base64_encode(hello world)}-
    ```
    Résultat : `-aGVsbG8gd29ybGQ=-`
* **base64_decode**(txt) : convertit la chaine en base64 passé en texte normal.
    ```
    -{base64_decode(aGVsbG8gd29ybGQ=)}-
    ```
    Résultat : `-hello world-`
