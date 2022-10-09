# Tchat in C

Program that allow you to tchat with others.

## Compilation :

Run the following command :

```bash
gcc main.c -o main
```

## Usage :

To create a server run :

```bash
./main -s
```

Then, create a reader to read the incoming & outcoming messages :

```bash
./main -r <@IP> <port>
```

Finally, create a writer :

```bash
./main -w <@IP> <port> <pseudo>
```

# Makefile helpers

Specials variables :
- $@ nom cible
- $< nom première dépendance
- $^ liste dépendances
- $? liste dépendances plus récentes que la cible
- $* nom fichier, sans son extension
