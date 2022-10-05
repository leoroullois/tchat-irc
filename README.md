# Tchat in C

Program that allow you to tchat with others.

## Compilation :

Run the following command :

```bash
gcc main.c -o main
```

And then, to create a server run :

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
