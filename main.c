#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 30

//définition du plateau dans lequel on va jouer
char monde[BOARD_SIZE*BOARD_SIZE] = {0};

void changer_couleur(int x, int y, char couleur, char* plateau){
    //remplace la case (x, y) du plateau par la couleur
    plateau[y*BOARD_SIZE + x] = couleur;
}

char element(int x, int y, char* plateau){
    //renvoie la couleur de l'élément (x, y) du plateau
    if ((y < BOARD_SIZE) && (x < BOARD_SIZE)){
        return plateau[y*BOARD_SIZE + x];
    }
    else{
        return 0;
    }
}

void afficher_monde(){
    //permet d'afficher le plateau "monde"
    for (int i = 0; i<BOARD_SIZE; i = i+1){
        for(int j = 0; j<BOARD_SIZE; j = j+1){
            printf("%c", monde[i*BOARD_SIZE + j]);
        }
        printf("\n");
    }
}

void initialiser(){
    //initialise le plateau "monde" en le remplissant de couleurs aléatoires
    for (int i = 0; i<BOARD_SIZE; i = i+1){
        for(int j = 0; j<BOARD_SIZE; j = j+1){
            int couleur = rand()*7/RAND_MAX;
            while (couleur == 7){
                couleur = rand()*7/RAND_MAX;
            }
            couleur = couleur + 65;
            changer_couleur(j, i, couleur, monde);
        }
    }
    //initialise la position des deux joueurs
    monde[BOARD_SIZE-1] = 94;
    monde[(BOARD_SIZE-1)*BOARD_SIZE] = 118;
}

void rafraichir_monde(char* plateau, char joueur, char couleur){
    //change la valeur des cases de la bonne couleur en celle du joueur si une case adjacente est de la couleur du joueur
    int change = 1; //variable qui indique si il y a des changements du plateau dans la boucle
    while (change == 1){
        change = 0; //on part du principe qu'il n'y aura pas de changement
        for (int i = 0; i<BOARD_SIZE; i = i+1){
            for(int j = 0; j<BOARD_SIZE; j = j+1){
                if (element(j, i, plateau) == couleur){
                    if (((i+1<BOARD_SIZE)&&(element(j, i+1, plateau) == joueur)) || ((i-1 >= 0)&&(element(j, i-1, plateau)== joueur)) || ((j+1<BOARD_SIZE)&&(element(j+1, i, plateau) == joueur)) || ((j-1 >= 0)&&(element(j-1, i, plateau) == joueur))){
                        changer_couleur(j, i, joueur, plateau);
                        change = 1; //si il y a changement, on recommence la boucle
                    }
                }
            }
        }
    }
}

char acquisition_couleur(){
    //renvoie la couleur choisie par le joueur
    char couleur = 0;
    printf("Entrez la couleur que vous voulez prendre : ");
    while ((couleur != 65) && (couleur != 66) && (couleur != 67) && (couleur != 68) && (couleur != 69) && (couleur != 70) && (couleur != 71)){
        scanf("%c", &couleur);
    }
    printf("\n");
    return couleur;
}

int compte_territoire(char joueur, char* plateau){
    //renvoie la taille du territoire du joueur sur le plateau
    int compte = 0;
    for (int i = 0; i<BOARD_SIZE; i = i+1){
        for(int j = 0; j<BOARD_SIZE; j = j+1){
            if (element(j, i, plateau) == joueur){
                compte = compte + 1;
            }
        }
    }
    return compte;
}

int victoire(char joueur){
    //indique que le joueur a gagné si son territoire est plus grand que la moiité du plateau
    int compte = compte_territoire(joueur, monde);
    if (compte > BOARD_SIZE*BOARD_SIZE/2){
        return 1;
    }
    else{
        return 0;
    }
}

char ia(char joueur){
    //c'est le débile
    //il choisit la couleur que l'ordinateur va jouer aléatoirement parmi celles qui sont ajacente au territoire du joueur
    char choix[7] = {0};
    char couleur = 0;
    //crée la liste des couleursjouables
    for (int i = 0; i<BOARD_SIZE; i = i+1){
        for(int j = 0; j<BOARD_SIZE; j = j+1){
            if (((i+1<BOARD_SIZE)&&(element(j, i+1, monde) == joueur)) || ((i-1 >= 0)&&(element(j, i-1, monde)== joueur)) || ((j+1<BOARD_SIZE)&&(element(j+1, i, monde) == joueur)) || ((j-1 >= 0)&&(element(j-1, i, monde) == joueur))){
                switch (element(j, i, monde)){
                case 65:
                    choix[0] = 65;
                    break;
                case 66:
                    choix[1] = 66;
                    break;
                case 67:
                    choix[2] = 67;
                    break;
                case 68:
                    choix[3] = 68;
                    break;
                case 69:
                    choix[4] = 69;
                    break;
                case 70:
                    choix[5] = 70;
                    break;
                case 71:
                    choix[6] = 71;
                    break;
                }
            }
        }
    }
    //choisit parmi les couleurs jouables
    while (couleur == 0){
        int indice = rand()*7/RAND_MAX;
        while (indice == 7){
            indice = rand()*7/RAND_MAX;
        }
        couleur = choix[indice];
    }
    printf("%c\n", couleur);
    return couleur;
}

char ia2(char joueur){
    //c'est le glouton
    //il choisit la couleur qui remplira le maximum de territoire
    char couleur = 0;
    int* comptes[7] = {0};

    for (int let = 65; let < 72; let ++){
        //on crée un plateau "digital", copie du "monde" sur lequel on va tester le changement de couleur pour chaque couleur
        char digital[BOARD_SIZE*BOARD_SIZE] = {0};
        for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++){
            digital[i] = monde[i];
        }
        rafraichir_monde(digital, joueur, let);
        comptes[let - 65] = compte_territoire(joueur, digital);
    }

    int max = 0;
    int* possibles[7] = {0};
    for (int i = 0; i < 7 ; i++){
        if (comptes[i] > max){
            max = comptes[i];
        }
    }
    for (int i = 0; i < 7 ; i++){
        if (comptes[i] == max){
            possibles[i] = 1;
        }
    }

    //choisit parmi les couleurs possibles
    int indice;
    while (couleur == 0){

        indice = rand()*7/RAND_MAX;
        while (indice == 7){
            indice = rand()*7/RAND_MAX;
        }
        couleur = possibles[indice];
    }
    couleur = indice + 65;
    printf("%c\n", couleur);
    return couleur;
}

int nombre_joueurs(){
    //renvoie le nombre de joueurs humains
    int nb_joueurs;
    printf("Nombre de joueurs, 0, 1 ou 2 ?\n");
    scanf("%i\n",&nb_joueurs);
    return nb_joueurs;
}

int difficulte(){
    //renvoie la difficulté du jeu (utilile uniquement s'il n'y a qu'un joueur)
    int diff;
    printf("Choisissez votre difficulté, 1 ou 2 ?\n");
    scanf("%i\n",&diff);
    return diff;
}

int nombre_parties(){
    // renvoie le nombre de partie de suite à jouer avec ces paramètres
    int nb_parties;
    printf("Nombre de parties ?\n");
    scanf("%i\n",&nb_parties);
    return nb_parties;
}

int tour1(int nb_joueurs){
    //effectue le tour du joueur 1 et renvoie 1 s'il gagne ce tour et 0 sinon
    char joueur = 118;
    char couleur;
    // si pas de joueur, le glouton joue
    if (nb_joueurs == 0){
        couleur = ia2(joueur);
    }
    //sinon, au moins un joueur, donc l'humain joue
    else{
        couleur = acquisition_couleur();
    }
    rafraichir_monde(monde, joueur, couleur);
    afficher_monde();
    printf("\n");

    int fin = victoire(joueur);//calcule 1 si le joueur 1 gagne ce tour et 0 sinon
    return fin;
}

int tour2(int nb_joueurs, int diff){
    //effectue le tour du joueur 2 et renvoie 1 s'il gagne ce tour et 0 sinon
    char joueur = 94;
    char couleur;
    //si 2 joueurs, alors il y a 2 humains et le joueur 2 en est un
    if (nb_joueurs == 2){
        couleur = acquisition_couleur();
    }
    //sinon c'est une IA
    else{
        //si le jeu est en facile (difficulté 1), le débile joue
        if (diff == 1){
            couleur = ia(joueur);
        }
        //sinon, le glouton joue
        else {
            couleur = ia2(joueur);
        }
    }
    rafraichir_monde(monde, joueur, couleur);
    afficher_monde();
    printf("\n");

    int fin = victoire(joueur);//calcule 1 si le joueur 1 gagne ce tour et 0 sinon
    return fin;
}

int partie(int nb_joueurs, int diff){
    //gère une partie et renvoie le gagnant (1 ou 2)
    int tour = 1;
    int fin = 0;
    int victoire = 0;

    initialiser();
    afficher_monde();
    printf("\n");

    while (fin == 0){
        if (tour == 1){
            fin = tour1(nb_joueurs);
            victoire = 1;
        }
        else{
            fin = tour2(nb_joueurs, diff);
            victoire = 2;
        }
        if (tour == 1){
            tour = 2;
        }
        else{
            tour = 1;
        }
    }
    return victoire;
}

int main(int argc, char*argv[]){
    //on initialise le nombre de joueurs, la difficulté et le nombre de parties
    int nb_joueurs = nombre_joueurs();
    int diff = 0;
    if (nb_joueurs == 1){
        diff = difficulte();
    }
    int nb_parties = nombre_parties();
    printf("%i, %i, %i\n", nb_joueurs, diff, nb_parties);

    //initialise les scores
    int score1 = 0;
    int score2 = 0;

    for (int i = 0; i<nb_parties; i++){
        //on joue toutes les parties
        int victoire = 0; //numéro du joueur victorieux de la partie
        victoire = partie(nb_joueurs, diff);
        if (victoire == 1){
            score1++;
        }
        else{
            score2++;
        }
        printf("%i : %i\n", score1, score2);//affichage des scores finaux
    }

    //on annonce le vainqueur
    if (score1>score2){
        printf("Le joueur 1 gagne !\n");
    }
    else{
        printf("Le joueur 2 gagne !\n");
    }

    return 0;
}
