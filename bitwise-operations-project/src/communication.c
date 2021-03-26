#include "communication.h"
#include "util_comm.h"

#include <stdio.h>
#include <string.h>

/* Task 1 - The Beginning */

void send_byte_message(void)
{
    /* TODO
     * Send the encoding of the characters: R, I, C, K
     */

    send_squanch((int)'R' - 64);
    send_squanch((int)'I' - 64);
    send_squanch((int)'C' - 64);
    send_squanch((int)'K' - 64);

}


void recv_byte_message(void)
{
    /* TODO
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     *
     * ATTENTION!: Use fprintf(stdout, ...)
     */

    //contorizez caracterele primite
    for(int i = 0; i < 5; i++)
        //le afisez decodificate
        fprintf(stdout, "%c", recv_squanch() + 64);

}


void comm_byte(void)
{
    /* TODO
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */

    //contorizez caracterle
    for(int i = 0; i < 10; i++){

        //salvez caracterul primit
        uint8_t received = recv_squanch();

        //il trimit de 2 ori
        send_squanch(received);
        send_squanch(received);

    }
}


/* Task 2 - Waiting for the Message */

void send_message(void)
{
    /* TODO
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */

    //salvez mesajul intr-un sir
    char str[] = "HELLOTHERE";

    //patrez lungimea acestuia
    uint8_t length = strlen(str);
    uint8_t size = length;

    //codific lungimea ca sa fie pe cei
    //4 biti din mijloc 
    length <<= 2;
    
    //trimit lungimea codificata
    send_squanch(length);

    //iterez prin caracterele sirului
    for(int i = 0; i < size; i++)
        //trimit fiecare caracter codificat
        send_squanch(str[i] - 64);
    
}


void recv_message(void)
{
    /* TODO
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     * 
     * ATTENTION!: Use fprintf(stdout, ...)
     */

    //primesc lungimea
    uint8_t length = recv_squanch();

    //decodific lungimea
    length >>= 2;

    //masca pentru a decodifica lungimea
    uint8_t mask = 0;

    //parcurg cei 4 biti asociati lungimii
    for(int i = 0; i < 4; i++ ){

        //setez valoarea corespunzatoare in mask
        if((length & (1 << i)) != 0)
            mask |= (1 << i);
    }

    //atribui lungimii valoarea decodificata
    length = mask;

    //pastrez lungimea
    uint8_t size = length;


    //afisez lungimea
    fprintf(stdout, "%u", length);


    //afisez fiecare caracter decodificat
    for(int i = 0; i < size; i++){
        fprintf(stdout, "%c", recv_squanch() + 64);
    }
}


void comm_message(void)
{
    /* TODO
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */

    uint8_t length = recv_squanch();

    //decodific lungimea
    length >>= 2;

    uint8_t mask = 0;

    for(int i = 0; i < 4; i++ ){
        if((length & (1 << i)) != 0)
            mask |= (1 << i);
    }

    length = mask;
    

    uint8_t character;
    char str[100];

    //primesc toate caracterele
    for(int i = 0; i < length; i++)
       character = recv_squanch() + 64;
    
    //verific daca ultimul este 'P'
    if(character == 80)
        strcpy(str, "PICKLERICK");
    
    else
        strcpy(str, "VINDICATORS");

    //trimit lungimea codificata
    send_squanch((strlen(str) << 2));

    //trimit caracterele codificate
    for(int i = 0; i < strlen(str); i++)
        send_squanch((int)str[i] - 64);

}


/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    /* TODO
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */

    //noul byte creat
    uint8_t new = 0;

    //masca pentru a compara primul bit
    uint8_t mask = (1 << 0);

    //iterez prin byte
    for(int i = 0; i < 8; i ++){

        //setez in noul byte valoarea corespunzatoare lui c2
        if((c2 & mask) != 0)
            new |= (1 << i);

        //il shifter pe c2 pentru a continua prelucrarea primului bit
        c2 >>= 1;

        //creste pozitia pe care adaug elemente in new
        i++;

        //setez in new bitul asociat lui  c1
        if((c1 & mask) != 0)
            new |= (1 << i);

        //il shiftez
        c1 >>= 1;
    }

    //trimit noul byte
    send_squanch(new);
}


uint8_t decode_squanch2(uint8_t c)
{
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */

    uint8_t res = -1;

    /* TODO */

    //iterator pentru c
    int i = 0;

    //index pentru bitii din prima jumatate
    int par = 0;

    //index pentru bitii din a doua jumatate
    int impar = 4;

    //il parcurg pe c
    while(i < 8){

        //daca este un bit de pe pozitie para
        if(i % 2 == 0){

            //setez valoarea lui in locul corespunzator din prima jumtate
            if((c & (1 << i)) == 0){
                res &= ~(1 << par);
               
            }
            //incrementez indexul
            par++;
        }
        else{
             //setez valoarea lui in locul corespunzator din a doua jumatate
            if((c & (1 << i)) == 0){
                res &= ~(1 << impar);
                
            }
            //incrementez indexul
            impar++;
        }
        //iterez prin c
        i++;
    }
    
    return res;
}
