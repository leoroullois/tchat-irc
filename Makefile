# Makefile avec macros : utilisation de variables prédéfinies
EXEC = main
O_DIR = obj/
SRC_DIR= src/
OPT = -c -Wall 

all: $(EXEC)
	echo "compilation terminée"

$(EXEC): $(O_DIR)utils.o  $(O_DIR)server.o $(O_DIR)client.o $(O_DIR)main.o 
	gcc $? -o $@

$(O_DIR)utils.o: $(SRC_DIR)utils.c $(SRC_DIR)utils.h
	gcc $(OPT) $(SRC_DIR)utils.c -o $@

$(O_DIR)server.o: $(O_DIR)utils.o $(SRC_DIR)server.c $(SRC_DIR)server.h
	gcc $(OPT) $(SRC_DIR)server.c -o $@

$(O_DIR)client.o: $(O_DIR)utils.o $(SRC_DIR)client.c $(SRC_DIR)client.h
	gcc $(OPT) $(SRC_DIR)client.c -o $@

$(O_DIR)main.o: main.c $(SRC_DIR)utils.h $(SRC_DIR)server.h $(SRC_DIR)client.h
	gcc $(OPT) main.c -o $@

clean:
	rm $(O_DIR)client.o $(O_DIR)server.o $(O_DIR)main.o $(EXEC)
