.syntax unified
.text
.align 2
.global asm_average

/* C Declaration                                             */
/* extern uint32_t asm_average(uint32_t Q, uint32_t* input); */

asm_average:

    /*
    C CODE
    uint32_t c_average(uint32_t Q, uint32_t* values) {
    int n = 2^Q;
    int sum_x = 0;
    for (int i = 0; i < n; i++)
    {
        sum_x += values[i];
    }
    uint32_t avg_x = sum_x/n;
    return avg_x;
    }
    */

    /*
        On entry:
        Q       r0 (1-16) // ln2(count)
        input   r1        // address of array containing count 32-bit unsigned integers
    */
    push    {r4-r7, lr}

    movs r2, #1
    lsls r2, r0 // store to r2, 1<<Q

    movs r3, #0 // sum begins at 0
    movs r4, #0 // overflow register also begins at 0
    movs r5, r2 //index counter begins at 1<<Q, and decrements from there

    sum_loop:
        subs r5, #1 //decrement the index
        ldr r6, [r1, r5] // load to r6 number[i] (where i is index at reg r5 and array address is r1)
        adds r3, r6 // at to the sum, the new number
        
        bcs carry

        
        cmp r5, #0  //check if index is 0
        bne sum_loop // branch if !=
    carry:
        adds r4, #1 // add one to the overflow register, 
        movs r3, #0 // sum re-begins at 0

        // address duplicated code 
        subs r5, #1 //decrement the index
        cmp r5, #0  //check if index is 0
        bne sum_loop // branch if !=

    subs r5, 32, r0 // store to r5, (32-Q)
    lsls r4, r5 // store to r4, r4<<(32-Q), (multiplied by 2^Q)
    lsrs r3, r0 // store to r3, r3>>(Q), (divided by 2^Q)
    orrs r0, r4, r3 // add these registers together to create final result
    
    
    
    
    /* TODO - put the result into r0 */

    pop {r4-r7, pc}

