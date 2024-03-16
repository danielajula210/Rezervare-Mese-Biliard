/**Sistem de rezervare mese biliard
Afacerea dispune de 6 mese de biliard. Aplicația oferă utilizatorului posibilitatea rezervării unei mese
de biliard oricând în intervalul orar limita_ora 15 :00 - 21:00 pe o durată aleasă de acesta (inclusiv minute).
Utilizatorul poate vizualiza disponibilitatea unei mese într-o anumită zi. Utilizatorul poate vizualiza
statistici despre fiecare masă în parte (cât de mult a fost utilzată în ultima lună ca număr de ore și minute,
numărul total de rezervări din ultima lună etc.).
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMITA_ORA 15
int limita_ora = LIMITA_ORA;

struct masa {
    char nr[50];
    char disponibilitate[100];
};

struct masa mese[100];

struct TreeNode {
    char nr[50];
    char disponibilitate[100];
    struct TreeNode* left;
    struct TreeNode* right;
};

struct HashNode {
    char nr[50];
    char disponibilitate[100];
    struct HashNode* next;
};

struct BST {
    struct TreeNode* root;
};

struct HashTable {
    struct HashNode* array[100];
};

struct BST bst;
struct HashTable hashTable;



void iesire();
void back();
void rezervare(int numarMasa);
void citire();
void afisareDetalii(int i);
void insertBST(struct BST* tree, char nr[50], char disponibilitate[100]);
void insertHashTable(struct HashTable* table, char nr[50], char disponibilitate[100]);
struct HashNode* searchHashTable(struct HashTable* table, char nr[50]);

void insertBST(struct BST* tree, char nr[50], char disponibilitate[100]) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    strcpy(newNode->nr, nr);
    strcpy(newNode->disponibilitate, disponibilitate);
    newNode->left = newNode->right = NULL;

    if (tree->root == NULL) {
        tree->root = newNode;
        return;
    }

    struct TreeNode* current = tree->root;
    while (1) {
        if (strcmp(nr, current->nr) < 0) {
            if (current->left == NULL) {
                current->left = newNode;
                return;
            }
            current = current->left;
        } else if (strcmp(nr, current->nr) > 0) {
            if (current->right == NULL) {
                current->right = newNode;
                return;
            }
            current = current->right;
        } else {
            /*printf("Masa cu numarul %s deja exista in arborele binar de cautare.\n", nr);*/
            free(newNode);
            return;
        }
    }
}

void insertHashTable(struct HashTable* table, char nr[50], char disponibilitate[100]) {
    struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
    strcpy(newNode->nr, nr);
    strcpy(newNode->disponibilitate, disponibilitate);
    newNode->next = NULL;

    int index = atoi(nr) % 100;

    if (table->array[index] == NULL) {
        table->array[index] = newNode;
    } else {
        newNode->next = table->array[index];
        table->array[index] = newNode;
    }
}

struct HashNode* searchHashTable(struct HashTable* table, char nr[50]) {
    int index = atoi(nr) % 100;

    struct HashNode* current = table->array[index];
    while (current != NULL) {
        if (strcmp(nr, current->nr) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void rezervare(int numarMasa) {
    FILE* fp = fopen("mesebiliard.txt", "a");
    int ora, minut, durata;
    char zi[9];
    printf("Introduceti ziua rezervarii: ");
    scanf("%s", zi);
    printf("Introduceti ora(format 24 de ore, orele 15-21): ");
    scanf("%d", &ora);
    if (ora < limita_ora || ora >= 21) {
        printf("Ne pare rau, rezervarile sunt disponibile intre orele %d:00 si 21:00.\n", limita_ora);
        return;
    }
    printf("Introduceti minutul: ");
    scanf("%d", &minut);
    printf("Introduceti durata rezervarii(in minute): ");
    scanf("%d", &durata);
    if (ora + (minut + durata) / 60 >= 21) {
        printf("Ne pare rau, nu putem accepta rezervari care se termina dupa ora 21:00.\n");
        return;
    } else {
        printf("\n");
        printf(" Masa a fost rezervata cu succes!\n");
        printf("\n");
        fprintf(fp, "In ziua de %s, masa de biliard cu nr %d este disponibila pentru rezervare incepand cu ora %d:%02d, pentru o durata de %d minute.\n", zi, numarMasa, ora, minut, durata);

        // Add reservation details only to the hash table
        insertHashTable(&hashTable, mese[numarMasa - 1].nr, mese[numarMasa - 1].disponibilitate);
    }
    fclose(fp);
}

void iesire() {
    printf("Iesire din program...\n");
    exit(0);
}

void back() {
    // Funcția pentru a reveni la meniul principal
    printf("Revenire la meniul principal...\n");
}

void citire() {
    // Funcția pentru citirea detaliilor meselor din fișier
    FILE* fp = fopen("mesebiliard.txt", "r");
    if (fp == NULL) {
        printf("Eroare la deschiderea fisierului mesebiliard.txt\n");
        exit(1);
    }

    int i = 0;
    while (fscanf(fp, "%s %s", mese[i].nr, mese[i].disponibilitate) == 2) {
        // Insert number of the table in BST
        insertBST(&bst, mese[i].nr, mese[i].disponibilitate);
        i++;
    }

    fclose(fp);
}

void afisare_detalii(int i){
    FILE* fp = fopen("mesebiliard.txt", "r");
    char line[256];
    int count=0;
    char aux[10] = "nr ";
    char nr_masa[2];
    sprintf(nr_masa, "%d", i+1);
    strcat(aux, nr_masa);
    int durata=0,ore=0, minute=0;
    while (fgets(line, sizeof(line), fp)){
        if (strstr(line,aux)!= NULL){
            count++;
            int durata;
            sscanf(line, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %d minute", &durata);
            minute=minute+durata;
        }
    }
    fclose(fp);
    ore=minute/60;
    minute=minute%60;
    printf("Detalii legate de masa cu nr %d\n",i+1);
    printf("\n");
    printf("- Disponibilitate: %s\n- Durata utilizare in ultima luna: %d ore si %d minute\n- Numar total de rezervari din ultima luna: %d\n",
        mese[i].disponibilitate,ore, minute, count);
}

/**void veziDetaliiRezervare(int numarMasa) {
    //afisareDetalii(numarMasa - 1);

    printf("1.Vezi detalii\n");
    printf("2.Rezerva masa\n");
    printf("3.Revenire la meniul principal\n");

    int optiune;
    do {
        printf("Introduceti optiunea dorita: ");
        scanf("%d", &optiune);
    } while (optiune < 1 || optiune > 3);

    switch (optiune) {
        case 1:
            afisareDetalii(numarMasa - 1);
            break;
        case 2:
            rezervare(numarMasa);
            break;
        case 3:
            back();
            break;
        default:
            printf("Optiune invalida!\n");
    }
}*/

int main() {

    bst.root = NULL;
    for (int i = 0; i < 100; i++) {
        hashTable.array[i] = NULL;
    }

     while (1 == 1) {
        system("cls");
        printf("    -= MENIU =-\n");
        printf("\n");
        printf("1. Rezervare mese biliard\n");
        printf("2. Iesire\n");
        printf("\n");
        int choice;
        do {
            printf("Introduceti optiunea dorita: ");
            scanf("%d", &choice);
        } while (choice < 1 || choice > 2);
        system("cls");
        lista:
        if (choice == 1) {
            citire();
            printf("  -= MESE BILIARD =-\n");
            printf("\n");
            printf("Mese de biliard disponibile:\n");
            printf("- Masa numarul 1\n");
            printf("- Masa numarul 2\n");
            printf("- Masa numarul 3\n");
            printf("- Masa numarul 4\n");
            printf("- Masa numarul 5\n");
            printf("- Masa numarul 6\n");
            printf(" Pentru a reveni la meniul principal apasati numarul 7!\n");
            printf("\n");
            int masa;
            do {
                printf("Introduceti numarul mesei pe care doriti sa o rezervati: ");
                scanf("%d", &masa);
            } while (masa < 1 || masa > 7);
            system("cls");
            ///MASA 1
            if(masa==1){
                masa1:
                printf("Ati selectat masa cu nr 1.\n");
                printf("\n");
                printf("1. Vezi detalii\n");
                printf("2. Rezerva masa\n");
                printf("- Pentru a accesa din nou lista de mese apasati tasta 3!\n");
                printf("- Pentru a reveni la meniul principal apasati tasta 4!\n");
                printf("\n");
                int opt;
                do {
                    printf(" Introduceti optiunea dorita: ");
                    scanf("%d", &opt);
                } while (opt < 1 || opt > 4);
                system("cls");
                if(opt==1){
                    afisare_detalii(masa-1);
                    ///system("pause");
                    printf("-Pentru a merge inapoi apasati tasta 1!\n");
                    printf("-Pentru a rezerva masa apasati tasta 2!\n");
                    printf("\n");
                    int alegere;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &alegere);
                    } while (alegere < 1 || alegere > 2);
                    system("cls");
                    if(alegere==1){
                        goto masa1;
                        system("cls");
                    }
                    else if(alegere==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                    }
                }
                else if(opt==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    printf("\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                }
                else if(opt==3){
                    goto lista;
                    system("cls");
                }

                else if(opt==4)
                    back();
            }
            ///MASA 2
            else if(masa==2){
                masa2:
                printf("Ati selectat masa cu nr 2.\n");
                printf("\n");
                printf("1. Vezi detalii\n");
                printf("2. Rezerva masa\n");
                printf("- Pentru a accesa din nou lista de mese apasati tasta 3!\n");
                printf("- Pentru a reveni la meniul principal apasati 4!\n");
                printf("\n");
                int opt;
                do {
                    printf(" Introduceti optiunea dorita: ");
                    scanf("%d", &opt);
                } while (opt < 1 || opt > 4);
                system("cls");
                if(opt==1){
                    afisare_detalii(masa-1);
                    ///system("pause");
                    printf("Pentru a merge inapoi apasati tasta 1!\n");
                    printf("Pentru a rezerva masa apasati tasta 2!\n");
                    int alegere;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &alegere);
                    } while (alegere < 1 || alegere > 2);
                    system("cls");
                    if(alegere==1){
                        goto masa2;
                        system("cls");
                    }
                    else if(alegere==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    printf("\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                    }
                }
                else if(opt==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    printf("\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                }
                else if(opt==3){
                    goto lista;
                    system("cls");
                }

                else if(opt==4)
                    back();
            }
            ///MASA 3
            else if(masa==3){
                masa3:
                printf("Ati selectat masa cu nr 3.\n");
                printf("\n");
                printf("1. Vezi detalii\n");
                printf("2. Rezerva masa\n");
                printf("- Pentru a accesa din nou lista de mese apasati tasta 3!\n");
                printf("- Pentru a reveni la meniul principal apasati 4!\n");
                printf("\n");
                int opt;
                do {
                    printf(" Introduceti optiunea dorita: ");
                    scanf("%d", &opt);
                } while (opt < 1 || opt > 4);
                system("cls");
                if(opt==1){
                    afisare_detalii(masa-1);
                    ///system("pause");
                    printf("Pentru a merge inapoi apasati tasta 1!\n");
                    printf("Pentru a rezerva masa apasati tasta 2!\n");
                    printf("\n");
                    int alegere;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &alegere);
                    } while (alegere < 1 || alegere > 2);
                    system("cls");
                    if(alegere==1){
                        goto masa3;
                        system("cls");
                    }
                    else if(alegere==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    printf("\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                    }
                }
                else if(opt==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    printf("\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                }
                else if(opt==3){
                    goto lista;
                    system("cls");
                }

                else if(opt==4)
                    back();
            }
            ///MASA 4
            else if(masa==4){
                masa4:
                printf("Ati selectat masa cu nr 4.\n");
                printf("\n");
                printf("1. Vezi detalii\n");
                printf("2. Rezerva masa\n");
                printf("- Pentru a accesa din nou lista de mese apasati tasta 3!\n");
                printf("- Pentru a reveni la meniul principal apasati 4!\n");
                printf("\n");
                int opt;
                do {
                    printf(" Introduceti optiunea dorita: ");
                    scanf("%d", &opt);
                } while (opt < 1 || opt > 4);
                system("cls");
                if(opt==1){
                    afisare_detalii(masa-1);
                    ///system("pause");
                    printf("Pentru a merge inapoi apasati tasta 1!\n");
                    printf("Pentru a rezerva masa apasati tasta 2!\n");
                    printf("\n");
                    int alegere;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &alegere);
                    } while (alegere < 1 || alegere > 2);
                    system("cls");
                    if(alegere==1){
                        goto masa4;
                        system("cls");
                    }
                    else if(alegere==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    printf("\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                    }
                }
                else if(opt==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    printf("\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                }
                else if(opt==3){
                    goto lista;
                    system("cls");
                }

                else if(opt==4)
                    back();
            }
            ///MASA 5
            else if(masa==5){
                masa5:
                printf("Ati selectat masa cu nr 5.\n");
                printf("\n");
                printf("1. Vezi detalii\n");
                printf("2. Rezerva masa\n");
                printf("- Pentru a accesa din nou lista de mese apasati tasta 3!\n");
                printf("- Pentru a reveni la meniul principal apasati 4!\n");
                printf("\n");
                int opt;
                do {
                    printf(" Introduceti optiunea dorita: ");
                    scanf("%d", &opt);
                } while (opt < 1 || opt > 4);
                system("cls");
                if(opt==1){
                    afisare_detalii(masa-1);
                    ///system("pause");
                    printf("Pentru a merge inapoi apasati tasta 1!\n");
                    printf("Pentru a rezerva masa apasati tasta 2!\n");
                    printf("\n");
                    int alegere;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &alegere);
                    } while (alegere < 1 || alegere > 2);
                    system("cls");
                    if(alegere==1){
                        goto masa5;
                        system("cls");
                    }
                    else if(alegere==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    printf("\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                    }
                }
                else if(opt==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    printf("\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                }
                else if(opt==3){
                    goto lista;
                    system("cls");
                }

                else if(opt==4)
                    back();
            }
            ///MASA 6
            else if(masa==6){
                masa6:
                printf("Ati selectat masa cu nr 6.\n");
                printf("\n");
                printf("1. Vezi detalii\n");
                printf("2. Rezerva masa\n");
                printf("- Pentru a accesa din nou lista de mese apasati tasta 3!\n");
                printf("- Pentru a reveni la meniul principal apasati 4!\n");
                printf("\n");
                int opt;
                do {
                    printf(" Introduceti optiunea dorita: ");
                    scanf("%d", &opt);
                } while (opt < 1 || opt > 4);
                system("cls");
                if(opt==1){
                    afisare_detalii(masa-1);
                    ///system("pause");
                    printf("Pentru a merge inapoi apasati tasta 1!\n");
                    printf("Pentru a rezerva masa apasati tasta 2!\n");
                    printf("\n");
                    int alegere;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &alegere);
                    } while (alegere < 1 || alegere > 2);
                    system("cls");
                    if(alegere==1){
                        goto masa6;
                        system("cls");
                    }
                    else if(alegere==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                    }
                }
                else if(opt==2){
                    rezervare(masa);
                   /// _getch();
                    printf("- Daca doriti sa rezervati alta masa apasati tasta 1!\n");
                    printf("- Daca ati terminat apasati tasta 2!\n");
                    int op;
                    do {
                       printf(" Introduceti optiunea dorita: ");
                       scanf("%d", &op);
                    } while (op < 1 || op > 2);
                    system("cls");
                    if(op==1) goto lista;
                    else if(op=2) iesire();
                }
                else if(opt==3){
                    goto lista;
                    system("cls");
                }

                else if(opt==4)
                    back();
            }
            ///PENTRU BACK
            else if(masa==7)
                back();

        }
        else if(choice==2)
            iesire();
    }
    return 0;
}
