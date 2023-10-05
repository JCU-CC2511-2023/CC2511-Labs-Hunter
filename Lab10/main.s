/** 
 * CC2511 Lab 10
 */

/* Assembler directives */

.syntax unified /* Use modern syntax                                                    */
.text           /* The "text" section of the executable is where code is placed.        */
.thumb_func     /* Specifies that main is a thumb mode function and will be offset by 1 */ 
.global main    /* Declare "main" as a global symbol (callable from other files).       */
.align 4        /* Make sure the starting address is a multiple of 4.                   */

/* Definitions */
/* These are equivalent to C macros. However, note that they use a comma. */

.EQU LED_PIN,                 25
.EQU WAIT_LOOPS_ON,           10000000
.EQU WAIT_LOOPS_OFF,          10000000

               

/* TODO - Add definitions of register addresses used in the program */  

// PAD CONTROL REGISTER DEFINITION
.EQU PADS_BANK0_GPIO25, 0xd0000068 // SIO_BASE + 0x68
// FUNCTION ALLOCATION REGISTER DEFINITION
.EQU GPIO25_CTRL, 0x400140CC // IO_BANK0_BASE + 0x0cc
// DATA DIRECTION REGISTER DEFINITION
.EQU REG_GPIO_OE_SET, 0xd0000024 // SIO_BASE + 0x24
// LED TOGGLE REGISTER DEFINITION
.EQU GPIO_OUT_SET, 0xd0000014 // SIO_BASE + 0x14
.EQU GPIO_OUT_CLR, 0xd0000018 // SIO_BASE + 0x18


/* Main function */
main:
  push {lr}

  /* TODO - Calculate any values that will be used throughout the program */

  /* TODO - Initialise 3 registers, as in Lab 3 */ 

  //| orrs
  //& ands
  //~ mvn

  /* 
    ENABLE PADS
  */ 
  ldr r0, = PADS_BANK0_GPIO25
  // CONTENT_OF(PADS_BANK0_GPIO25) &= ~(0x1 << 7);
  ldr r1, = #0x80 // turn r1 into 0x1 << 7
  mvns r1, r1 // turn r1 into ~(0x1 << 7)
  ands r1, r1, r0 // turn r0 into (PADS_BANK0_GPIO25) &= ~(0x1 << 7);
  // CONTENT_OF(PADS_BANK0_GPIO25) |= (0x1 << 6);
  ldr r1, = #0x40 // turn r1 into 0x1 << 6
  orrs r1, r1, r0 // turn r1 into (PADS_BANK0_GPIO25) |= (0x1 << 6);
  str r1, [r0] // source, destination
  /* 
    SELECT GPIO FUNCTION
  */ 
  ldr r2, = GPIO25_CTRL
  // CONTENT_OF(GPIO25_CTRL) &= ~(0x1f) | (0x5);
  ldr r3, = #0x1f // turn r3 into 0x1f
  mvns r3, r3 // turn r3 into ~(0x1f)
  ldr r4, = #0x5 // turn r4 into 0x5
  orrs r3, r3, r4 // turn r3 into (GPIO25_CTRL) &= ~(0x1f) | (0x5);
  str r3, [r2] // source, destination
  /* 
    SET DATA DIRECTION
  */ 
  ldr r4, = REG_GPIO_OE_SET
  //CONTENT_OF(REG_GPIO_OE_SET) |= (0x1 << LED_PIN), where LED_PIN = 25;
  ldr r5, = #0x2000000 // turn r5 into 0x1 << 25
  orrs r5, r5, r4 // turn r5 into (REG_GPIO_OE_SET) |= (0x1 << LED_PIN)
  str r5, [r4] // source, destination
  
  // prepare variable decleration for while loop
  ldr r0, = GPIO_OUT_SET
  ldr r2, = GPIO_OUT_CLR
  whileloop:

    //CONTENT_OF(GPIO_OUT_SET) |= (0x1 << LED_PIN);
    ldr r1, = #0x2000000 // turn r1 into 0x1 << 25
    orrs r1, r1, r0 // turn r1 into (GPIO_OUT_SET) |= (0x1 << LED_PIN)
    str r1, [r0] // source, destination

    ldr r4, = WAIT_LOOPS_ON
    loop1: 
      subs r4, #1 //r6 -= 1
      cmp r4, #0 //?r6==0?
      bne loop1 // branch if !=
    
    //CONTENT_OF(GPIO_OUT_CLR) |= (0x1 << LED_PIN);
    ldr r3, = #0x2000000 // turn r3 into 0x1 << 25
    orrs r3, r3, r2 // turn r1 into (GPIO_OUT_CLR) |= (0x1 << LED_PIN)
    str r3, [r2] // source, destination

    ldr r4, = WAIT_LOOPS_OFF
    loop2: 
      subs r4, #1 //r4 -= 1
      cmp r4, #0 //?r4==0?
      bne loop2 // branch if !=

  b whileloop
  /** TODO 
   *  Run in an endless loop that:
   *    (i)    Turns on the LED
   *    (ii)   Waits, by using an inner loop
   *    (iii)  Turns off the LED
   *    (iv)   Waits, by using an inner loop
   *    (v)    Returns to the top of the loop 
   */

	pop {pc}  /* Return from this function, by popping the return address into the program counter (pc) */
