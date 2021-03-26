#include "gates.h"

#include <stdio.h>
#include <assert.h>

/* Task 1 - Bit by Bit */

uint8_t get_bit(uint64_t nr, uint8_t i)			
{
    assert(i <= 8 * sizeof nr);

    uint8_t res = -1;

    /*  TODO
    *
    * "res" should be 1 if the bit is active, else 0
    */

    if(nr & (1L << i))
        res = 1;
    else
        res = 0;

    return res;
}


uint64_t flip_bit(uint64_t nr, uint8_t i)       
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    /* TODO
     *
     * Return the "nr" with the ith bit flipped
     */

    res = nr ^ (1L << i);

    return res;
}


uint64_t activate_bit(uint64_t nr, uint8_t i) 
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = 0xFF;

    /* TODO
     *
     * Return the "nr" with the ith bit "1"
     */
    
    res = (nr | (1L << i));
    
    return res;
}


uint64_t clear_bit(uint64_t nr, uint8_t i)     
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    /* TODO
     *
     * Return the "nr" with the ith bit "0"
     */
   
   //masca care are toti bitii 1, mai putin bitul i
    uint64_t mask = ~(1L << i);

    res = nr & mask;
   
    return res;
}


/* Task 2 - One Gate to Rule Them All */

uint8_t nand_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    return !(a & b);
}


uint8_t and_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    /* TODO - Use the nand gate to implement the and gate */

    res = !(nand_gate(a, b));

    return res;
}


uint8_t not_gate(uint8_t a)
{
    assert (a == 0 || a == 1);

    uint8_t res = -1;

    /* TODO - Use the nand gate to implement the not gate */

    res = nand_gate(a, a);

    return res;
}


uint8_t or_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    /* TODO - Use the previously defined gates to implement the or gate */

    res = nand_gate(nand_gate(a, a), nand_gate(b, b));

    return res;
}


uint8_t xor_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    /* TODO - Use the previously defined gates to implement the xor gate */

    res = nand_gate(nand_gate(nand_gate(a, b), a), nand_gate(b, nand_gate(a, b)));
    return res;
}


/* Task 3 - Just Carry the Bit */

uint8_t full_adder(uint8_t a, uint8_t b, uint8_t c)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    assert (c == 0 || c == 1);

    uint8_t res = 0;

    /* TODO - implement the full_adder using the previous gates
     * Since the full_adder needs to provide 2 results, you should
     * encode the sum bit and the carry bit in one byte - you can encode
     * it in whatever way you like
     */

    //suma este cel mai nesemnificativ bit
    //carry este urmatorul

    //toti bitii din suma sunt 0
    if(a == 0 && b == 0 && c == 0)  
    	return 0;

    //toti bitii sunt 1
    else if(a == 1 && b == 1 && c == 1){ 
    	res = activate_bit(res, 0);	//suma
    	res = activate_bit(res, 1);	//carry

    }

    //2 biti sunt 1 si unul 0
    else if((a == 1 && b == 1 && c == 0) ||
    	(a == 1 && b == 0 && c == 1) || (a == 0 && b == 1 && c == 1))  
    	res = activate_bit(res, 1); //suma =0 carry =1
    
    //un bit este 1 si restul 0
    else
    	res = activate_bit(res, 0); //suma = 1; carry = 0;
  
    return res;
}


uint64_t ripple_carry_adder(uint64_t a, uint64_t b)
{
    uint64_t res = -1;

    /* TODO
     * Use the full_adder to implement the ripple carry adder
     * If there is ANY overflow while adding "a" and "b" then the
     * result should be 0
     */
    // uint64_t res = 0;

    //intial carry  = 0
    uint8_t carry = 0;

    //parcurg bitii din numere
    for(int i = 0; i < 8 * sizeof(a); i++){

        //prelucrez bitii separat
        uint8_t bit_a = get_bit(a, i);
        uint8_t bit_b = get_bit(b, i);

        //rezultatul de la full_adder
        uint8_t rezultat = full_adder(bit_a, bit_b, carry);

        //extrag valoarea carry-ului
        carry = get_bit(rezultat, 1);

        //daca suma bitilor este 0
        if(get_bit(rezultat, 0) == 0)

            //resetez bitul corespunzator din res
            res = clear_bit(res, i);
    }
    
    //daca am overflow
    if(carry == 1)
        return 0;

    return res;
}