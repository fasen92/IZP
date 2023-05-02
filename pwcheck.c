/*
* Ivan Mahút
* xmahut01
*/ 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int getInt(char *str, bool lvl);
int getStrLen(char str[]);
bool checkStats(char *str);
bool firstLvl(char str[]);
bool secondLvl(char str[], int param);
bool thirdLvl(char str[], int param);
bool fourthLvl(char str[], int param);

int main(int argc, char *argv[]) {

  int lvl = 0;
  int param = 0;
  bool stats = false;
  bool avg = true;
  bool diffCharBool;
  bool firstChar = true;
  bool longPass = true;
  char passw[102] = {}; // 100 znakove heslo + \n +\0
  int minLen = 0;
  int avgSum = 0;
  int avgCount = 0;
  char diffChars[94] = {}; // max. pocet moznych podporovanych znakov

  if (argc == 3 ){ // overenie poctu argumentov

  }else if (argc == 4) {
    stats = checkStats(argv[3]); // kontrola validneho vstupu stats
    if(!stats){
      fprintf(stderr, "Chyba vstupnych parametrov, neplatny parameter --stats\n");
    return EXIT_FAILURE;
    }
  } else {
    fprintf(stderr, "Chyba vstupnych parametrov, minimalni pocet parametrov je "
                    "2 a maximalny 3\n");
    return EXIT_FAILURE;
  }

  // zistenie int hodnot, overenie neocakavaneho vstupu
  lvl = getInt(argv[1], true);
  if (lvl == -1) {
    return EXIT_FAILURE;
  }
  param = getInt(argv[2], false);
  if (param == -1) {
    return EXIT_FAILURE;
  }

  while (fgets(passw, 102, stdin) != NULL) { // nacitanie jednotlivych hesiel
    int i = 0;
    while (passw[i] != '\0') {
      if (passw[i] == '\n') { // kontrola dlzky hesla
        if (i < 101){
          longPass = false;
          break;
        }
      }
      i++;
    }

    if (longPass)
    {
      fprintf(stderr, "Chyba vstupnych parametrov, maximalna dlzka hesla je 100 znakov\n");
      return EXIT_FAILURE;
    }
    
    // zistovanie statistik
    if (stats) {
      int i = 0;
      while (passw[i] != '\n') {
        diffCharBool = true;
        if(firstChar){ // pole je prazdne, zapise sa prvy znak
          diffChars[0] = passw[0];
          firstChar = false;
        }
        int j = 0;
        // zapisovanie vsetkych znakov do pomocneho pola
        while(diffChars[j] != '\0'){ 
          if (passw[i] == diffChars[j]) {
            diffCharBool = false;
            break;
          }
          j++;
        }
        if (diffCharBool) {
          diffChars[j++] = passw[i];
        }
        i++;
      }

      if (avg) { // dlzka prveho hesla sa zapise do premennej min
        minLen = getStrLen(passw);
        avg = false;

      } else if (minLen > getStrLen(passw)){ // dalej sa porovnava s dlzkou kazdeho hesla
        minLen = getStrLen(passw);
      }
      avgSum += getStrLen(passw);
      avgCount++;
    }

    switch (lvl) // testovanie hesiel podla lvls
    {
    case 1:
      if (firstLvl(passw)) {
        printf("%s", passw);
      }
      break;

    case 2:
      if (firstLvl(passw)) { // ak neplati prvy lvl, druhy nebude zavolany
        // ak je 1 lvl splneny, su splenen 2 pravidla
        if (secondLvl(passw, param))
          printf("%s", passw);
      }
      break;

    case 3:
      if (firstLvl(passw)) {
        if (secondLvl(passw, param)) {
          if (thirdLvl(passw, param))
            printf("%s", passw);
        }
      }
      break;

    case 4:
      if (firstLvl(passw)) {
        if (secondLvl(passw, param)) {
          if (thirdLvl(passw, param)) {
            if (fourthLvl(passw, param))
              printf("%s", passw);
          }
        }
      }
      break;

    default: // overujeme vstup, default nemoze nastat
      break;
    }
    
  }
  if(stats){ //vypis stats
    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", getStrLen(diffChars));
    printf("Minimalni delka: %d\n", minLen-1);
    printf("Prumerna delka: %3.1f\n", ((double)(avgSum-avgCount)/avgCount));
  }

  return EXIT_SUCCESS;
}

int getInt(char *str, bool lvl) {
  char *endptr;

  int i = strtol(str, &endptr,10); // konvertovanie char na int, zapis pripadnych nadbytocnych znakov
  if (i == 0) { // overenie ze parameter je nenulovy
    fprintf(stderr,
            "Chyba vstupnych parametrov, parameter sa nesmie rovnat 0\n");
    return -1;
  } else if (i < 0) {
    fprintf(
        stderr,
        "Chyba vstupnych parametrov, parameter musi byt cele kladne cislo\n");
    return -1;
  } else if (endptr[0] !=
             '\0') { // overenie ze za cislom sa nenachadzaju neziaduce znaky
    fprintf(stderr,
            "Chyba vstupnych parametrov, parameter moze obsahovat len cisla\n");
    return -1;
  }else if (lvl) { // podmienka je splnená len pri lvl
    if (i != 1 && i != 2 && i != 3 &&
        i != 4) { // overovanie ci je cislo z intervalu
      fprintf(stderr, "Chyba vstupnych parametrov, parameter lvl je definovany "
                      "len v intervale [1, 4]\n");
      return -1;
    }
  }

  return i;
}

/*
* level 4, overovanie dvoch rovankych substringov dlzky param
*/
bool fourthLvl(char *str, int param) {
  int i = 0;
  while(str[i] != '\0')
  {
    int j = i+1;
    while (str[j] != '\0')
    {
      if (str[i] == str[j])
      {
        for (int k = 1; k <= param; k++)
        {
          if(k == param) return false;
          if(str[i+k] != str[j+k]) break;
        }
      }
      j++;
    }
    i++;
  }
  return true;
}

/*
* level 3, overovanie sekvencie znakov dlzky param
*/ 
bool thirdLvl(char *str, int param) {
  char controlChar = str[0];
  int controlCount = 1;
  if (param == 1) return false;
  

  for (int i = 1; str[i] != '\0'; i++) {
    if (controlChar == str[i]) {
      controlCount++;
      if (controlCount >= param){
        return false;
      }
    } else {
      controlCount = 1;
    }
    controlChar = str[i];
  }
  return true;
}

bool secondLvl(char *str, int param) {
  int i = 0;
  int gPassed = 2; // ak je 1 lvl splneny, su splnene 2 pravidla
  bool cisla = false, special = false;
  if(param > 4) param = 4;
  while (str[i] != '\0') {

    if (str[i] >= '0' && str[i] <= '9') {
      // overovanie cislic
      cisla = true;
    } else if ((str[i] >= ' ' && str[i] <= '/' ) ||
               (str[i] >= ':' && str[i] <= '@') ||
               (str[i] >= '[' && str[i] <= '`') ||
               (str[i] >= '{' && str[i] <= '~')){
      // overovanie specialnych znakov
      special = true;
    }
    i++;
  }

  gPassed += ((int)cisla + (int)special);
  if (gPassed >= param) { //
    return true;
  } else {
    return false;
  }
}

bool firstLvl(char *str) {
  int i = 0;
  bool male = false, velke = false;
  while (str[i] != '\0') {
    if (str[i] >= 'a' && str[i] <= 'z') { // overenie lowercase
      male = true;
    } else if (str[i] >= 'A' && str[i] <= 'Z') { // overenie uppercase
      velke = true;
    }
    i++;
  }

  if (male && velke) {
    return true;
  } else {
    return false;
  }
}


int getStrLen(char str[]){
  int i = 0;
  while (str[i] != '\0')
  {
    i++;
  }
  return i;
}

/*
* overovanie --stats
*/
bool checkStats(char *str) {
  char sCompare[] = "--stats";
  int i = 0;
  
  // kontrola znakov
  while (sCompare[i] != '\0' && str[i] != '\0') {

    if (sCompare[i] != str[i]) {
      return false;
    }
    i++;
  }

  // ak sa rovnaju znaky a zaroven sa za poslednym znakom nic nenachadza, vstup je validny
  if (sCompare[i] == '\0' && str[i] == '\0') {
    return true;
  } 
  return false;
}