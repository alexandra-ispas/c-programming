#include "hunt.h"
#include <stdio.h>

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{
    /*
     * The spell is 16 bits and they are in the memory exactly after 5
     * consecutive bits of 1:
     *
     *            high                                             low
     *                [][][x]...[x][x][x][1][1][1][1][1][]...[][][]
     *                    |----16bits---|
     *                          Spell
     *
     * There is only 1 group of 5 consecutive bits of 1 and the other groups
     * have less than consecutive bits of 1
     * Steps:
     * - find 5 consecutive bits of 1
     * - the NEXT 16 bits == spell
     */

    /* TODO */

    uint16_t res = -1;

    //pastreaza numarul de valori de 1 consecutive
    uint8_t counter = 0;

    //iterator
    int i; 

    //parcurg sirul de biti
    for(i = 0; i < 8 * sizeof(memory) ; i++){

        //daca bitul este setat, creste counter
        if((memory & (1L << i)))
            counter++;

        //daca bitul nu este setat, verific daca s-a atins numarul necesar
        //de valori de 1
        else{
            //am gasit indexul
            if(counter == 5){
                break;
            }
            //caut o alta grupare de valori de 1
            else counter = 0;
        }
    }

    //contorizez cei 16 biti care imi trebuie
    for(int j = 0; j < 16; j++)

        //ii adaug in res
        if((memory & (1L << (j + i))) == 0)
            res &= ~(1L << j);

    return res;
}


uint16_t find_key(uint64_t memory)
{
    /*
     * The key is 16 bits and they are in the memory exactly before 3
     * consecutive bits of 1:
     *
     *                high                                           low
     *                     [][][][1][1][1][x][x]...[x][x][]...[][][]
     *                                    |----16bits---|
     *                                           Key
     * Steps:
     * - find 3 consecutive bits of 1
     * - the PREVIOUS 16 bits == key
     */

    uint16_t res = -1;

    /* TODO */

    uint8_t counter = 0;

    int i; 

    //parcurg sirul de biti pornind de la cel mai semnificativ
    for(i = 8 * sizeof(memory) - 1; i >= 0 ; i--){

        //daca este setat, creste numarul de biti de 1
        if((memory & (1L << i)))
            counter++;
        else{
            //daca am numarat 3 biti, am gasit indexul
            if(counter == 3) break;

            //caut alta grupare
            else counter = 0;
        }
    }

    //contorizez cei 16 biti
    for(int j = 0; j < 16; j++)

        //ii setez corespunzator in res
        if((memory & (1L << (i - j))) == 0)
            res &= ~(1L << (15 - j));

    return res;
}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */

    uint16_t res = -1;

    /* TODO */

    //setez res
    res &= (spell ^ key);

    return res;
}


/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{
    /*
     * Help Geralt construct a sword that will give him an advantage in battle.
     * The steps we will follow:
     * - choose the corect sword
     * - brand the sword with the correct runes such that the equation
     * for that specific enemy type is true
     *
     * How does the sword look (Sword template):
     *  [][][][] [0][0][0]...[0][0][0] [][][][][][][][][][][][][][][][]
     *  -------- --------------------- --------------------------------
     *     Type    Does not matter      The "Value" such that the equation
     *                               from 2 holds depending on the enemy type
     *    4 bits      12 bits                     16 bits
     *
     * 1. Type of the sword:
     * - 1001 - Silver Sword - for monsters
     * - 0110 - Steel Sword - for humans
     *
     *  If the number of active bits from variable "enemy" is even == Monster
     *  else == Human
     *
     * 2.
     * Monster Equation:
     *    Value ^ (Enemy & (1 - Enemy)) = 0
     *
     * Human Equation:
     *    Value + Enemy = 0
     */

    uint32_t res = -1;

    /* TODO */

    //get number of active bits from enemy

    //calculez numarul de valori de 1
    //pentru a stabili type-ul
    int counter = 0;

    for(int i = 0; i < 8 * sizeof(enemy); i++){
        if((enemy & (1U << i)))
            counter++;
    }

    uint32_t value;

    //daca este pentru monstrii
    if(counter % 2 == 0){

        //setez type
        res &= ~(1 << 30);
        res &= ~(1 << 29);

        // calculez value din formula
        //Value ^ (Enemy & (1 - Enemy)) = 0
        uint32_t minus_enemy = 1U - enemy;
    	uint32_t logical_and = enemy & minus_enemy;

    	value = 0 ^ logical_and;
    }
    //este pentru oameni
    else{

        //setez type
        res &= ~(1 << 31);
        res &= ~(1 << 28);

        //calculez value din formula
        //Value + Enemy = 0
        value = -enemy;  
    }

    //pun bitii din value la finalul lui res
    for(int i = 0; i < 16; i++){
    	if((value & (1U << i)) == 0)
    		res &= ~(1U << i);
    }

    //setez la 0 cei 12 biti 
    for(int i = 16; i < 28; i++)
    	res &= ~(1U << i);

    return res;
}


/* Task 3 - The Witcher Trials */

uint32_t trial_of_the_grasses(uint16_t cocktail)
{
    /*
     * To become a witcher one must survive a cocktail of different
     * herbs. The body should generate the correct antibodies to
     * neutralize the given cocktail.
     *
     *
     * The antibodies need to respect the following properties:
     *   (antibodies_high & cocktail) ^ (antibodies_low | cocktail) = 0
     *   antibodies_low & antibodies_high = 0
     *   antibodies_low | antibodies_high = cocktail
     *
     * Where:
     *  [][][]...[][][] | [][][]...[][][]
     *  ---------------   ---------------
     *  antibodies_high    antibodies_low
     *      16 bits           16 bits
     *      -------------------------
     *              antibodies
     */

    uint32_t res = -1;

    /* TODO */

    uint16_t antibodies_low = 0, antibodies_high = cocktail;

    //setez in res bitii corespunzatori pentru antibodies_low
    for(int i = 0; i < 16; i++){
        if((antibodies_low & (1U << i)) == 0)
            res &= ~(1U << i);
    }

    //setez in res bitii corespunzatori pentru antibodies_high
    for (int i = 0; i < 16; ++i)
    {
        if((antibodies_high & (1U << i)) == 0)
            res &= ~(1U << (i + 16));
    }

    return res;
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{
    /*
     * For the next trail, the candidate is tied up and blindfolded.
     * They are also taken to a forest and they should return until
     * the next morning.
     *
     * The candidate knows that there are 4 possibilities for the forest:
     * Brokilon, Hindar, Wolven Glade and Caed Dhu.
     *
     * The candidate also knows those forests by heart, but first you
     * need to identify the forest.
     *
     * The characteristics for 3 of the forests are as follows:
     * - Brokilon - has ONLY groups of 4 trees (or a forest without any tree)
     *   Ex: a group of 4 trees: "...00111100..."
     * - Hindar - has ONLY 2 trees in the MIDDLE
     * - Wolven Glade - FULL of trees
     *
     * The "map" variable encodes the following information:
     * - 1 tree
     * - 0 patch of grass
     *
     * You should put in the "res" variable the index for the identified
     * forrest.
     * - Brokilon - index 0
     * - Hindar - index 1
     * - Wolven Glade - index 2
     * - Caed Dhu - index 3
     */

    // uint8_t res = -1;

    /* TODO */
  
    uint64_t check = -1;

    //cazul pentru Brokilon daca padurea nu are niciun copac
    if(map == 0){
    	return 0;
    }

    //chech Wolven Glade
    if((check & map) == check){
    	return 2;
    }

    //check Hindar
    //am calculat valoarea pe care ar avea-o numarul cu 2 biti activi in mijloc
    if(map == 6442450944){
    	return 1;
    }

    //pentru cazul Brokilon cu grupuri de 4 copaci

    //il folosesc pentru cazurile in care:
    //-sunt mai putin de 4 copaci consecutivi
    //-sunt mai mult de 4 copaci consecutivi
    int notok = 0;

    //parcurg toti bitii de pe harta
    for(int i = 0; i < 8 * sizeof(map) && notok == 0; i++){

        //daca este un bit setat
    	if(map & (1L << i)){
    		
            //iterez prin urmatorii 3 biti
    		int k;
    		for(k = 1; k < 4 && notok == 0; k++)

    			//se intrerupe sirul de valori de 1
    			if((map & (1L << (i + k))) == 0)
    				notok = 1;

    		//verific ca urmatoare valoare sa fie 0, daca exista una urmatoare
    		if(((i + k + 1) < 8 * sizeof(map)) && (map & (1L << (i + k + 1))))
    			notok = 1;

    		//continui de unde am ramas
    		i += k + 2;
    	}
    }

    //daca s-a respectat numarul de copaci
    if(notok == 0)
    	return 0;

   //daca nu se incadreaza in niciun tipar, este Caed Dhu
    return 3;
}


uint8_t trial_of_the_dreams(uint32_t map)
{
    /*
     * For this final trial, the candidates go into a trance and when
     * they woke up they would be a fully fledged witcher.
     *
     * For this task, you need to help the witcher find the number
     * of steps that would take the candidate from the dream realm to
     * the real world.
     *
     * The "map" variable would have only 2 bits of 1:
     * - candidate location
     * - portal location
     *
     * You should put in the "res" variable the distance from the
     * candidate position to the portal.
     *
     *  Ex:
     *  [0][0][0][0][0][0]....[1][0][0][0][0][1][0]....[0][0][0]
     *
     *  res = 5
     */

    uint8_t res = -1;

    /* TODO */

    //bitul din stanga cu valoarea 1
    int left = -1;

    //bitul din dreapta cu valoarea 1
    int right = -1;

    //parcurg toti bitii din map
    for(int i = 0; i < 8 * sizeof(map); i++){

        //daca bitul este 1
        if((map & (1 << i)) != 0){

            //setez valorile corespunzatoare
            if(right == -1)
                right = i;

            else if(left == -1){
                left = i;
                //daca am gasit ambele valori
                //nu mai iterez
                break;
            }
        }
    }

    //calculez dinstanta
    res = left - right;
    return res;
}
