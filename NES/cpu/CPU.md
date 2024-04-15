# ref

- <https://www.nesdev.org/wiki/CPU>
- <https://www.pagetable.com/c64ref/6502/>
- <https://www.masswerk.at/6502/6502_instruction_set.html>

# Architecture

The 6502 microprocessor is a relatively simple 8 bit CPU with only a few internal registers capable of addressing at most 64Kb of memory via its 16 bit address bus. The processor is little endian and expects addresses to be stored in memory least significant byte first.

The first 256 byte page of memory ($0000-$00FF) is referred to as 'Zero Page' and is the focus of a number of special addressing modes that result in shorter (and quicker) instructions or allow indirect access to the memory. The second page of memory ($0100-$01FF) is reserved for the system stack and which cannot be relocated.

The only other reserved locations in the memory map are the very last 6 bytes of memory $FFFA to $FFFF which must be programmed with the addresses of the non-maskable interrupt handler ($FFFA/B), the power on reset location ($FFFC/D) and the BRK/interrupt request handler ($FFFE/F) respectively.

The 6502 does not have any special support of hardware devices so they must be mapped to regions of memory in order to exchange data with the hardware latches.

# Registers

## PC Program Counter

  This register points the address from which the next instruction
  byte (opcode or parameter) will be fetched. Unlike other
  registers, this one is 16 bits in length. The low and high 8-bit
  halves of the register are called PCL and PCH, respectively. The
  Program Counter may be read by pushing its value on the stack.
  This can be done either by jumping to a subroutine or by causing
  an interrupt.

##  S Stack pointer

  The NMOS 65xx processors have 256 bytes of stack memory, ranging
  from $0100 to $01FF. The S register is a 8-bit offset to the stack
  page. In other words, whenever anything is being pushed on the
  stack, it will be stored to the address $0100+S.
  The Stack pointer can be read and written by transfering its value
  to or from the index register X (see below) with the TSX and TXS
  instructions.

## P Processor status

  This 8-bit register stores the state of the processor. The bits in
  this register are called flags. Most of the flags have something
  to do with arithmetic operations.

  The P register can be read by pushing it on the stack (with PHP or
  by causing an interrupt). If you only need to read one flag, you
  can use the branch instructions. Setting the flags is possible by
  pulling the P register from stack or by using the flag set or
  clear instructions.

### Following is a list of the flags, starting from the 8th bit of the P register (bit 7, value $80)

- N Negative flag
        This flag will be set after any arithmetic operations
        (when any of the registers A, X or Y is being loaded
        with a value). Generally, the N flag will be copied from
        the topmost bit of the register being loaded.

        Note that TXS (Transfer X to S) is not an arithmetic
        operation. Also note that the BIT instruction affects
        the Negative flag just like arithmetic operations.
        Finally, the Negative flag behaves differently in
        Decimal operations (see description below).
- V oVerflow flag
        Like the Negative flag, this flag is intended to be used
        with 8-bit signed integer numbers. The flag will be
        affected by addition and subtraction, the instructions
        PLP, CLV and BIT, and the hardware signal -SO. Note that
        there is no SEV instruction, even though the MOS
        engineers loved to use East European abbreviations, like
        DDR (Deutsche Demokratische Republik vs. Data Direction
        Register). (The Russian abbreviation for their former
        trade association COMECON is SEV.) The -SO (Set
        Overflow) signal is available on some processors, at
        least the 6502, to set the V flag. This enables response
        to an I/O activity in equal or less than three clock
        cycles when using a BVC instruction branching to itself
        ($50 $FE).

        The CLV instruction clears the V flag, and the PLP and
        BIT instructions copy the flag value from the bit 6 of
        the topmost stack entry or from memory.

        After a binary addition or subtraction, the V flag will
        be set on a sign overflow, cleared otherwise. What is a
        sign overflow? For instance, if you are trying to add
        123 and 45 together, the result (168) does not fit in a
        8-bit signed integer (upper limit 127 and lower limit
        -128). Similarly, adding -123 to -45 causes the
        overflow, just like subtracting -45 from 123 or 123 from
        -45 would do.

        Like the N flag, the V flag will not be set as expected
        in the Decimal mode. Later in this document is a precise
        operation description.

        A common misbelief is that the V flag could only be set
        by arithmetic operations, not cleared.
    1 unused flag
        To the current knowledge, this flag is always 1.
- B Break flag
        This flag is used to distinguish software (BRK)
        interrupts from hardware interrupts (IRQ or NMI). The B
        flag is always set except when the P register is being
        pushed on stack when jumping to an interrupt routine to
        process only a hardware interrupt.

        The official NMOS 65xx documentation claims that the BRK
        instruction could only cause a jump to the IRQ vector
        ($FFFE). However, if an NMI interrupt occurs while
        executing a BRK instruction, the processor will jump to
        the NMI vector ($FFFA), and the P register will be
        pushed on the stack with the B flag set.
- D Decimal mode flag
        This flag is used to select the (Binary Coded) Decimal
        mode for addition and subtraction. In most applications,
        the flag is zero.

        The Decimal mode has many oddities, and it operates
        differently on CMOS processors. See the description of
        the ADC, SBC and ARR instructions below.
- I Interrupt disable flag
        This flag can be used to prevent the processor from
        jumping to the IRQ handler vector ($FFFE) whenever the
        hardware line -IRQ is active. The flag will be
        automatically set after taking an interrupt, so that the
        processor would not keep jumping to the interrupt
        routine if the -IRQ signal remains low for several clock
        cycles.
- Z Zero flag
        The Zero flag will be affected in the same cases than
        the Negative flag. Generally, it will be set if an
        arithmetic register is being loaded with the value zero,
        and cleared otherwise. The flag will behave differently
        in Decimal operations.
- C Carry flag
        This flag is used in additions, subtractions,
        comparisons and bit rotations. In additions and
        subtractions, it acts as a 9th bit and lets you to chain
        operations to calculate with bigger than 8-bit numbers.
        When subtracting, the Carry flag is the negative of
        Borrow: if an overflow occurs, the flag will be clear,
        otherwise set. Comparisons are a special case of
        subtraction: they assume Carry flag set and Decimal flag
        clear, and do not store the result of the subtraction
        anywhere.

        There are four kinds of bit rotations. All of them store
        the bit that is being rotated off to the Carry flag. The
        left shifting instructions are ROL and ASL. ROL copies
        the initial Carry flag to the lowmost bit of the byte;
        ASL always clears it. Similarly, the ROR and LSR
        instructions shift to the right.

## A Accumulator

  The accumulator is the main register for arithmetic and logic
  operations. Unlike the index registers X and Y, it has a direct
  connection to the Arithmetic and Logic Unit (ALU). This is why
  many operations are only available for the accumulator, not the
  index registers.

## X Index register X

  This is the main register for addressing data with indices. It has
  a special addressing mode, indexed indirect, which lets you to
  have a vector table on the zero page.

## Y Index register Y

  The Y register has the least operations available. On the other
  hand, only it has the indirect indexed addressing mode that
  enables access to any memory place without having to use
  self-modifying code.

************

# Instruction

  The 6502 has a relatively basic set of instructions, many having similar functions (e.g. memory access, arithmetic, etc.). The following sections list the complete set of 56 instructions in functional groups.

  OBS:
    * add 1 to cycles if page boundary is crossed

    ** add 1 to cycles if branch occurs on same page
       add 2 to cycles if branch occurs to different page

## Transfer Instructions

  Load, store, interregister transfer
  
  1. Load/Store Operations<br>
    These instructions transfer a single byte between memory and one of the registers. Load operations set the negative (N) and zero (Z) flags depending on the value of transferred. Store operations do not affect the flag settings.

- LDA

    load accumulator

    When instruction LDA is executed by the microprocessor, data is transferred from memory to the accumulator and stored in the accumulator.

    LDA affects the contents of the accumulator, does not affect the carry or overflow flags; sets the zero flag if the accumulator is zero as a result of the LDA, otherwise resets the zero flag; sets the negative flag if bit 7 of the accumulator is a 1, other­ wise resets the negative flag.

        M -> A                                   N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   |  LDA #oper   | A9  |   2   |   2    |
    | zeropage    |  LDA oper    | A5  |   2   |   3    |
    | zeropage,X  |  LDA oper,X  | B5  |   2   |   4    |
    | absolute    |  LDA oper    | AD  |   3   |   4    |
    | absolute,X  |  LDA oper,X  | BD  |   3   |   4*   |
    | absolute,Y  |  LDA oper,Y  | B9  |   3   |   4*   |
    | (indirect,X)|  LDA (oper,X)| A1  |   2   |   6    |
    | (indirect),Y|  LDA (oper),Y| B1  |   2   |   5*   |

- LDX

    load X

    Load the index register X from memory.

    LDX does not affect the C or V flags; sets Z if the value loaded was zero, otherwise resets it; sets N if the value loaded in bit 7 is a 1; otherwise N is reset, and affects only the X register.

        M -> X                                   N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   |  LDX #oper   | A2  |   2   |   2    |
    | zeropage    |  LDX oper    | A6  |   2   |   3    |
    | zeropage,Y  |  LDX oper,Y  | B6  |   2   |   4    |
    | absolute    |  LDX oper    | AE  |   3   |   4    |
    | absolute,Y  |  LDX oper,Y  | BE  |   3   |   4*   |

- LDY

    load Y

    Load the index register Y from memory.

    LDY does not affect the C or V flags, sets the N flag if the value loaded in bit 7 is a 1, otherwise resets N, sets Z flag if the loaded value is zero otherwise resets Z and only affects the Y register.

        M -> Y                                   N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   |  LDY #oper   | A0  |   2   |   2    |
    | zeropage    |  LDY oper    | A4  |   2   |   3    |
    | zeropage,X  |  LDY oper,X  | B4  |   2   |   4    |
    | absolute    |  LDY oper    | AC  |   3   |   4    |
    | absolute,X  |  LDY oper,X  | BC  |   3   |   4*   |

- STA

    store accumulator

    This instruction transfers the contents of the accumulator to memory.

    This instruction affects none of the flags in the processor status register and does not affect the accumulator.

        A -> M                                   N Z C I D V
                                                 - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | zeropage    |  STA oper    | 85  |   2   |   3    |
    | zeropage,X  |  STA oper,X  | 95  |   2   |   4    |
    | absolute    |  STA oper    | 8D  |   3   |   4    |
    | absolute,X  |  STA oper,X  | 9D  |   3   |   5    |
    | absolute,Y  |  STA oper,Y  | 99  |   3   |   5    |
    | (indirect,X)|  STA (oper,X)| 81  |   2   |   6    |
    | (indirect,Y)|  STA (oper),X| 91  |   2   |   6    |

- STX

    store X

    Transfers value of X register to addressed memory location.

    No flags or registers in the microprocessor are affected by the store operation.

        X -> M                                   N Z C I D V
                                                 - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | zeropage    |  STX oper    | 86  |   2   |   3    |
    | zeropage,Y  |  STX oper,Y  | 96  |   2   |   4    |
    | absolute    |  STX oper    | 8E  |   3   |   4    |

- STY

    store Y

    Transfer the value of the Y register to the addressed memory location.

    STY does not affect any flags or registers in the microprocessor.

        Y -> M                                   N Z C I D V
                                                 - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | zeropage    |  STY oper    | 84  |   2   |   3    |
    | zeropage,X  |  STY oper,X  | 94  |   2   |   4    |
    | absolute    |  STY oper    | 8C  |   3   |   4    |

  2. Register Transfers<br>
  The contents of the X and Y registers can be moved to or from the accumulator, setting the negative (N) and zero (Z) flags as appropriate.

- TAX

    transfer accumulator to X

    This instruction takes the value from accumulator A and trans­ fers or loads it into the index register X without disturbing the content of the accumulator A.

    TAX only affects the index register X, does not affect the carry or overflow flags. The N flag is set if the resultant value in the index register X has bit 7 on, otherwise N is reset. The Z bit is set if the content of the register X is 0 as aresult of theopera­ tion, otherwise it is reset.

        A -> M                                   N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     TAX      | AA  |   1   |   2    |

- TAY

    transfer accumulator to Y

    This instruction moves the value of the accumulator into index register Y without affecting the accumulator.

    TAY instruction only affects the Y register and does not affect either the carry or overflow flags. If the index register Y has bit 7 on, then N is set, otherwise it is reset. If the content of the index register Y equals 0 as a result of the operation, Z is set on, otherwise it is reset.

        A -> Y                                   N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     TAY      | A8  |   1   |   2    |

- TXA

    transfer X to accumulator

    This instruction moves the value that is in the index register X to the accumulator A without disturbing the content of the index register X.

    TXA does not affect any register other than the accumula­tor and does not affect the carry or overflow flag. If the result in A has bit 7 on, then the N flag is set, otherwise it is reset. If the resultant value in the accumulator is 0, then the Z flag is set, other­ wise it is reset.

- TYA

    transfer Y to accumulator

    This instruction moves the value that is in the index register Y to accumulator A without disturbing the content of the register Y.

    TYA does not affect any other register other than the accumula­ tor and does not affect the carry or overflow flag. If the result in the accumulator A has bit 7 on, the N flag is set, otherwise it is reset. If the resultant value in the accumulator A is 0, then the Z flag is set, otherwise it is reset.

        Y -> A                                   N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     TYA      | 98  |   1   |   2    |

## Stack Instructions

  These instructions transfer the accumulator or status register (flags) to and from the stack. The processor stack is a last-in-first-out (LIFO) stack of 256 bytes length, implemented at addresses $0100 - $01FF. The stack grows down as new values are pushed onto it with the current insertion point maintained in the stack pointer register.
  (When a byte is pushed onto the stack, it will be stored in the address indicated by the value currently in the stack pointer, which will be then decremented by 1. Conversely, when a value is pulled from the stack, the stack pointer is incremented. The stack pointer is accessible by the TSX and TXS instructions.)

- TXS

    transfer X to stack pointer

    This instruction transfers the value in the index register X to the stack pointer.

    TXS changes only the stack pointer, making it equal to the content of the index register X. It does not affect any of the flags.

        X -> SP                                  N Z C I D V
                                                 - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     TXS      | 9A  |   1   |   2    |

- TSX

    transfer stack pointer to X

    This instruction transfers the value in the stack pointer to the index register X.

    TSX does not affect the carry or overflow flags. It sets N if bit 7 is on in index X as a result of the instruction, otherwise it is reset. If index X is zero as a result of the TSX, the Z flag is set, other­ wise it is reset. TSX changes the value of index X, making it equal to the content of the stack pointer.

        SP -> X                                  N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     TSX      | BA  |   1   |   2    |

- PHA

    push accumulator

    This instruction transfers the current value of the accumulator to the next location on the stack, automatically decrementing the stack to point to the next empty location.

    The Push A instruction only affects the stack pointer register which is decremented by 1 as a result of the operation. It affects no flags.

- PHP

    push processor status register (with break flag set)

    This instruction transfers the contents of the processor status reg­ ister unchanged to the stack, as governed by the stack pointer.

    The PHP instruction affects no registers or flags in the micropro­cessor.

        push SR                                  N Z C I D V
                                                 - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     PHP      | 08  |   1   |   3    |

- PLA

    pull accumulator

    This instruction adds 1 to the current value of the stack pointer and uses it to address the stack and loads the contents of the stack into the A register.

    The PLA instruction does not affect the carry or overflow flags. It sets N if the bit 7 is on in accumulator A as a result of instructions, otherwise it is reset. If accumulator A is zero as a result of the PLA, then the Z flag is set, otherwise it is reset. The PLA instruction changes content of the accumulator A to the contents of the memory location at stack register plus 1 and also increments the stack register.

        pull A                                   N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     PLA      | 68  |   1   |   4    |

- PLP

    pull processor status register

    This instruction transfers the next value on the stack to the Proces­ sor Status register, thereby changing all of the flags and setting the mode switches to the values from the stack.

    The PLP instruction affects no registers in the processor other than the status register. **This instruction could affect all flags in the status register**.

        pull SR                                  N Z C I D V
                                                 from the stack

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     PLP      | 28  |   1   |   4    |

## Decrements & Increments

- DEC
    Decrement Memory By One

    This instruction subtracts 1, in two's complement, from the contents of the addressed memory location.

    The decrement instruction does not affect any internal register in the microprocessor. It does not affect the carry or overflow flags. If bit 7 is on as a result of the decrement, then the N flag is set, otherwise it is reset. If the result of the decrement is 0, the Z flag is set, other­wise it is reset.

        M - 1 -> M                               N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | zeropage    | DEC oper     | C6  |   2   |   5    |
    | zeropage,X | DEC oper,X   | D6  |   2   |   6    |
    | absolute   | DEC oper    | CE  |   3   |   6    |  
    | absolute,X | DEC oper,X  | DE  |   3   |   7    |  

- DEX
    Decrement Index Register X By One

    This instruction subtracts one from the current value of the index register X and stores the result in the index register X.

    DEX does not affect the carry or overflow flag, it sets the N flag if it has bit 7 on as a result of the decrement, otherwise it resets the N flag; sets the Z flag if X is a 0 as a result of the decrement, otherwise it resets the Z flag.

        x - 1 -> x                               N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | implied     |     DEX      | CA  |   1   |   2    |

- DEY
    Decrement Index Register Y By One

    This instruction subtracts one from the current value in the in­ dex register Y and stores the result into the index register Y. The result does not affect or consider carry so that the value in the index register Y is decremented to 0 and then through 0 to FF.

    Decrement Y does not affect the carry or overflow flags; if the Y register contains bit 7 on as a result of the decrement the N flag is set, otherwise the N flag is reset. If the Y register is 0 as a result of the decrement, the Z flag is set otherwise the Z flag is reset. This instruction only affects the index register Y.

        x - 1 -> x                               N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | implied     |     DEY      |  88 |   1   |   2    |

- INC
    Increment Memory By One

    This instruction adds 1 to the contents of the addressed memory loca­tion.

    The increment memory instruction does not affect any internal registers and does not affect the carry or overflow flags. If bit 7 is on as the result of the increment,N is set, otherwise it is reset; if the increment causes the result to become 0, the Z flag is set on, otherwise it is reset.

        M + 1 -> M                               N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | zeropage   | INC oper     | E5  | 2     |   5    |
    | zeropage,X | INC oper,X  | F6  | 2     |   6    |  
    | absolute   | INC oper    | EE  | 3    |   6    |
    | absolute,X | INC oper,X  | FE  | 3    |   7    |
  
- INX
    Increment Index Register X By One

    Increment X adds 1 to the current value of the X register. This is an 8-bit increment which does not affect the carry operation, therefore, if the value of X before the increment was FF, the resulting value is 00.

    INX does not affect the carry or overflow flags; it sets the N flag if the result of the increment has a one in bit 7, otherwise resets N; sets the Z flag if the result of the increment is 0, otherwise it resets the Z flag.

    INX does not affect any other register other than the X register.

        X + 1 -> X                               N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | implied     |     INX      | E8  | 1     |   2    |

- INY
    Increment Index Register Y By One

    Increment Y increments or adds one to the current value in the Y register, storing the result in the Y register. As in the case of INX the primary application is to step thru a set of values using the Y register.

    The INY does not affect the carry or overflow flags, sets the N flag if the result of the increment has a one in bit 7, otherwise resets N, sets Z if as a result of the increment the Y register is zero otherwise resets the Z flag.

        Y + 1 -> Y                               N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | implied     |     INY      | C8  | 1     |   2    |

## Logical Operations

  The following instructions perform logical operations on the contents of the accumulator and another value held in memory. The BIT instruction performs a logical AND to test the presence of bits in the memory value to set the flags but does not keep the result.

- AND
  
    and (with accumulator)

    The AND instruction transfer the accumulator and memory to the adder which performs a bit-by-bit AND operation and stores the result back in the accumulator.

    This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise resets the zero flag; sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the

        A AND M -> A                             N Z C I D V
                                                 + + + - - +

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   | AND #oper    | 29  | 2     | 2      |
    | zeropage   | AND oper     | 25  | 2     | 3      |
    | zeropage,X | AND oper,X  | 35  | 2     | 4      |  
    | absolute   | AND oper    | 2D  | 3    | 4      |
    | absolute,X | AND oper,X  | 3D  | 3    | 4*     |
    | absolute,Y | AND oper,Y  | 39  | 3    | 4*     |
    | (indirect,X)| AND (oper,X) | 21  | 2     | 6      |
    | (indirect),Y| AND (oper),Y | 31  | 2     | 5*     |

- EOR

    exclusive or (with accumulator)

    The EOR instruction transfers the memory and the accumulator to the adder which performs a binary "EXCLUSIVE OR" on a bit-by-bit basis and stores the result in the accumulator.

    This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise resets the zero flag sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the negative flag.

        A EOR M -> A                             N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   | EOR #oper    | 49  | 2     | 2      |
    | zeropage   | EOR oper     | 45  | 2     | 3      |
    | zeropage,X | EOR oper,X  | 55  | 2     | 4      |  
    | absolute   | EOR oper    | 4D  | 3    | 4      |
    | absolute,X | EOR oper,X  | 5D  | 3    | 4*     |
    | absolute,Y | EOR oper,Y  | 59  | 3    | 4*     |
    | (indirect,X)| EOR (oper,X) | 41  | 2     | 6      |
    | (indirect),Y| EOR (oper),Y | 51  | 2     | 5*     |

- ORA
  
    (inclusive) or with accumulator
    The ORA instruction transfers the memory and the accumulator to the adder which performs a binary "OR" on a bit-by-bit basis and stores the result in the accumulator.

    This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise resets the zero flag; sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the negative flag.

        A OR M -> A                              N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   | ORA #oper    | 09  | 2     | 2      |
    | zeropage   | ORA oper     | 05  | 2     | 3      |
    | zeropage,X | ORA oper,X  | 15  | 2     | 4      |  
    | absolute   | ORA oper    | 0D  | 3    | 4      |
    | absolute,X | ORA oper,X  | 1D  | 3    | 4*     |
    | absolute,Y | ORA oper,Y  | 19  | 3    | 4*     |
    | (indirect,X)| ORA (oper,X) | 01  | 2     | 6      |
    | (indirect),Y| ORA (oper),Y | 11  | 2     | 5*     |
  
## Arithmetic Operations

  The arithmetic operations perform addition and subtraction on the contents of the accumulator. The compare operations allow the comparison of the accumulator and X or Y with memory values.

- ADC

    Add with Carry

    This instruction adds the value of memory and carry from the previous operation to the value of the accumulator and stores the result in the accumulator.

    This instruction affects the accumulator; sets the carry flag when the sum of a binary add exceeds 255 or when the sum of a decimal add exceeds 99, otherwise carry is reset. The overflow flag is set when the sign or bit 7 is changed due to the result exceeding +127 or -128, otherwise overflow is reset. The negative flag is set if the accumulator result contains bit 7 on, otherwise the negative flag is reset. The zero flag is set if the accumulator result is 0, otherwise the zero flag is reset.

    Note on the MOS 6502:
    In decimal mode, the N, V and Z flags are not consistent with the decimal result.

        A + M + C -> A, C                        N Z C I D V
                                                 + + + - - +

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   | ADC #oper    | 69  | 2     | 2      |
    | zeropage   | ADC oper     | 65  | 2     | 3      |
    | zeropage,X | ADC oper,X  | 75  | 2     | 4      |  
    | absolute   | ADC oper    | 6D  | 3    | 4      |
    | absolute,X | ADC oper,X  | 7D  | 3    | 4*     |
    | absolute,Y | ADC oper,Y  | 79  | 3    | 4*     |
    | (indirect,X)| ADC (oper,X) | 61  | 2     | 6      |
    | (indirect),Y| ADC (oper),Y | 71  | 2     | 5*     |

- SBC

    Subtract with Carry

    This instruction subtracts the value of memory and borrow from the value of the accumulator, using two's complement arithmetic, and stores the result in the accumulator. Borrow is defined as the carry flag complemented; therefore, a resultant carry flag indicates that a borrow has not occurred.

    This instruction affects the accumulator. The carry flag is set if the result is greater than or equal to 0. The carry flag is reset when the result is less than 0, indicating a borrow. The over­flow flag is set when the result exceeds +127 or -127, otherwise it is reset. The negative flag is set if the result in the accumulator has bit 7 on, otherwise it is reset. The Z flag is set if the result in the accumulator is 0, otherwise it is reset.

    Note on the MOS 6502:
    In decimal mode, the N, V and Z flags are not consistent with the decimal result.

        A - M - C̅ -> A                             N Z C I D V
                                                   + + + - - +

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   |  SBC #oper   | E9  |   2   |    2   |
    | zeropage    |  SBC oper    | E5  |   2   |    3   |
    | zeropage,X  |  SBC oper,X  | F5  |   2   |    4   |
    | absolute    |  SBC oper    | ED  |   3   |    4   |
    | absolute,X  |  SBC oper,X  | FD  |   3   |    4*  |
    | absolute,Y  |  SBC oper,Y  | F9  |   3   |    4*  |
    | (indirect,X)|  SBC (oper,X)| E1  |   2   |    6   |
    | (indirect),Y|  SBC (oper),Y| F1  |   2   |    5*  |

## Shift & Rotate Instructions

  All shift and rotate instructions preserve the bit shifted out in the carry flag.

- ASL
    arithmetic shift left (shifts in a zero bit on the right)

    The shift left instruction shifts either the accumulator or the address memory location 1 bit to the left, with the bit 0 always being set to 0 and the the input bit 7 being stored in the carry flag. ASL either shifts the accumulator left 1 bit or is a read/modify/write instruction that affects only memory.

    The instruction does not affect the overflow bit, sets N equal to the result bit 7 (bit 6 in the input), sets Z flag if the result is equal to 0, otherwise resets Z and stores the input bit 7 in the carry flag.

        C <- [76543210] <- 0                     N Z C I D V
                                                 + + + - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | accumulator | ASL A        | 0A  | 1     | 2      |
    | zeropage   | ASL oper     | 06  | 2     | 5      |
    | zeropage,X | ASL oper,X  | 16  | 2     | 6      |  
    | absolute   | ASL oper    | 0E  | 3    | 6      |
    | absolute,X | ASL oper,X  | 1E  | 3    | 7      |

- LSR

    logical shift right (shifts in a zero bit on the left)

    This instruction shifts either the accumulator or a specified memory location 1 bit to the right, with the higher bit of the result always being set to 0, and the low bit which is shifted out of the field being stored in the carry flag.

    The shift right instruction either affects the accumulator by shift­ing it right 1 or is a read/modify/write instruction which changes a speci­fied memory location but does not affect any internal registers. The shift right does not affect the overflow flag. The N flag is always reset. The Z flag is set if the result of the shift is 0 and reset otherwise. The carry is set equal to bit 0 of the input.

        0 -> [76543210] -> C                     N Z C I D V
                                                 0 + + - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | accumulator |  LSR A       | 4A  |   1   |   2    |
    | zeropage    |  LSR oper    | 46  |   2   |   5    |
    | zeropage,X  |  LSR oper,X  | 56  |   2   |   6    |
    | absolute    |  LSR oper    | 4E  |   3   |   6    |
    | absolute,X  |  LSR oper,X  | 5E  |   3   |   7    |

- ROL

    rotate left (shifts in carry bit on the right)

    The rotate left instruction shifts either the accumulator or addressed memory left 1 bit, with the input carry being stored in bit 0 and with the input bit 7 being stored in the carry flags.

    The ROL instruction either shifts the accumulator left 1 bit and stores the carry in accumulator bit 0 or does not affect the internal reg­isters at all. The ROL instruction sets carry equal to the input bit 7, sets N equal to the input bit 6 , sets the Z flag if the result of the ro­ tate is 0, otherwise it resets Z and does not affect the

        C <- [76543210] <- C                     N Z C I D V
                                                 + + + - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | accumulator |  ROL A       | 2A  |   1   |   2    |
    | zeropage    |  ROL oper    | 26  |   2   |   5    |
    | zeropage,X  |  ROL oper,X  | 36  |   2   |   6    |
    | absolute    |  ROL oper    | 2E  |   3   |   6    |
    | absolute,X  |  ROL oper,X  | 3E  |   3   |   7    |

- ROR

    rotate right (shifts in zero bit on the left)

    The rotate right instruction shifts either the accumulator or addressed memory right 1 bit with bit 0 shifted into the carry and carry shifted into bit 7.

    The ROR instruction either shifts the accumulator right 1 bit and stores the carry in accumulator bit 7 or does not affect the internal regis­ ters at all. The ROR instruction sets carry equal to input bit 0, sets N equal to the input carry and sets the Z flag if the result of the rotate is 0; otherwise it resets Z and does not affect the overflow flag at all.

        C <- [76543210] <- C                     N Z C I D V
                                                 + + + - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | accumulator |  ROR A       | 6A  |   1   |   2    |
    | zeropage    |  ROR oper    | 66  |   2   |   5    |
    | zeropage,X  |  ROR oper,X  | 76  |   2   |   6    |
    | absolute    |  ROR oper    | 6E  |   3   |   6    |
    | absolute,X  |  ROR oper,X  | 7E  |   3   |   7    |

## Flag Instructions

  The following instructions change the values of specific status flags.

- CLC

    clear carry

    This instruction initializes the carry flag to a 0. This op­ eration should normally precede an ADC loop. It is also useful when used with a R0L instruction to clear a bit in memory.

    This instruction affects no registers in the microprocessor and no flags other than the carry flag which is reset.

        0 -> C                                   N Z C I D V
                                                 - - 0 - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |      CLC     | 18  |   1   |   2    |

- CLD

    clear decimal (BCD arithmetics disabled)

    This instruction sets the decimal mode flag to a 0. This all subsequent ADC and SBC instructions to operate as simple operations.

    CLD affects no registers in the microprocessor and no flags other than the decimal mode flag which is set to a 0.

        0 -> D                                   N Z C I D V
                                                 - - - - 0 -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     CLD      | D8  |   1   |   2    |

- CLI

    This instruction initializes the interrupt disable to a 0. This allows the microprocessor to receive interrupts.

    It affects no registers in the microprocessor and no flags other than the interrupt disable which is cleared.

    clear interrupt disable

        0 -> I                                   N Z C I D V
                                                 - - - 0 - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     CLI      | 58  |   1   |   2    |

- CLV

    clear overflow

    This instruction clears the overflow flag to a 0. This com­ mand is used in conjunction with the set overflow pin which can change the state of the overflow flag with an external signal.

    CLV affects no registers in the microprocessor and no flags other than the overflow flag which is set to a 0.

        clear interrupt disable

        0 -> V                                   N Z C I D V
                                                 - - - - - 0

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     CLV      | B8  |   1   |   2    |

- SEC

    set carry

    This instruction initializes the carry flag to a 1. This op eration should normally precede a SBC loop. It is also useful when used with a ROL instruction to initialize a bit in memory to a 1.

    This instruction affects no registers in the microprocessor and no flags other than the carry flag which is set.

        1 -> C                                   N Z C I D V
                                                 - - 1 - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     SEC      | 38  |   1   |   2    |

- SED

    set decimal (BCD arithmetics enabled)

    This instruction sets the decimal mode flag D to a 1. This makes all subsequent ADC and SBC instructions operate as a decimal arithmetic operation.

    SED affects no registers in the microprocessor and no flags other than the decimal mode which is set to a 1.

        1 -> D                                   N Z C I D V
                                                 - - - - 1 -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     SED      | F8  |   1   |   2    |
  
- SEI

    set interrupt disable

    This instruction initializes the interrupt disable to a 1. It is used to mask interrupt requests during system reset operations and during interrupt commands.

    It affects no registers in the microprocessor and no flags other than the interrupt disable which is set.

        1 -> I                                   N Z C I D V
                                                 - - - 1 - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     SEI      | 78  |   1   |   2    |

## Comparisons

  Generally, comparison instructions subtract the operand from the given register without affecting this register. Flags are still set as with a normal subtraction and thus the relation of the two values becomes accessible by the Zero, Carry and Negative flags.
  (See the branch instructions below for how to evaluate flags.)

  |Relation R − Op   |  Z  |   C   |   N                  |
  |:----------------:|:---:|-------|:--------------------:|
  |Register < Operand|  0  |   0   |   sign bit of result |
  |Register = Operand|  1  |   1   |   0                  |
  |Register > Operand|  0  |   1   |   sign bit of result |

- CMP
  
    compare (with accumulator)

    This instruction subtracts the contents of memory from the contents of the accumulator.

    The use of the CMP affects the following flags: Z flag is set on an equal comparison, reset otherwise; the N flag is set or reset by the result bit 7, the carry flag is set when the value in memory is less than or equal to the accumulator, reset when it is greater than the accumulator. The accumulator is not affected.

        A - M                                      N Z C I D V
                                                   + + + - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   |  CMP #oper   | C9  |   2   |    2   |
    | zeropage    |  CMP oper    | C5  |   2   |    3   |
    | zeropage,X  |  CMP oper,X  | D5  |   2   |    4   |
    | absolute    |  CMP oper    | CD  |   3   |    4   |
    | absolute,X  |  CMP oper,X  | DD  |   3   |    4*  |
    | absolute,Y  |  CMP oper,Y  | D9  |   3   |    4*  |
    | (indirect,X)|  CMP (oper,X)| C1  |   2   |    6   |
    | (indirect),Y|  CMP (oper),Y| D1  |   2   |    5*  |
  
- CPX
  
    compare with X

    This instruction subtracts the value of the addressed memory location from the content of index register X using the adder but does not store the result; therefore, its only use is to set the N, Z and C flags to allow for comparison between the index register X and the value in memory.

    The CPX instruction does not affect any register in the machine; it also does not affect the overflow flag. It causes the carry to be set on if the absolute value of the index register X is equal to or greater than the data from memory. If the value of the memory is greater than the content of the index register X, carry is reset. If the results of the subtraction contain a bit 7, then the N flag is set, if not, it is reset. If the value in memory is equal to the value in index register X, the Z flag is set, otherwise it is reset.

        X - M                                      N Z C I D V
                                                   + + + - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   |  CMX #oper   | E0  |   2   |    2   |
    | zeropage    |  CMX oper    | E4  |   2   |    3   |
    | absolute    |  CMX oper    | EC  |   3   |    4   |
  
- CPY
  
    compare with Y

    This instruction performs a two's complement subtraction between the index register Y and the specified memory location. The results of the subtraction are not stored anywhere. The instruction is strict­ly used to set the flags.

    CPY affects no registers in the microprocessor and also does not affect the overflow flag. If the value in the index register Y is equal to or greater than the value in the memory, the carry flag will be set, otherwise it will be cleared. If the results of the subtract- tion contain bit 7 on the N bit will be set, otherwise it will be cleared. If the value in the index register Y and the value in the memory are equal, the zero flag will be set, otherwise it will be cleared.

        Y - M                                      N Z C I D V
                                                   + + + - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | immediate   |  CMY #oper   | C0  |   2   |    2   |
    | zeropage    |  CMY oper    | C4  |   2   |    3   |
    | absolute    |  CMY oper    | CC  |   3   |    4   |
  
## Comparisons

  Branch targets are relative, signed 8-bit address offsets. (An offset of #0 corresponds to the immedately following address — or a rather odd and expensive NOP.)

- BCC
  
    branch on carry clear

    This instruction tests the state of the carry bit and takes a conditional branch if the carry bit is reset.

    It affects no flags or registers other than the program counter and then only if the C flag is not on.

        branch on C = 0                            N Z C I D V
                                                   - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | relative    | BCC oper     | 90  | 2     | 2**    |

- BCS
  
    branch on carry set

    This instruction takes the conditional branch if the carry flag is on.

    BCS does not affect any of the flags or registers except for the program counter and only then if the carry flag is on.

        branch on C = 1                            N Z C I D V
                                                   - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | relative    | BCS oper     | B0  | 2     | 2**    |

- BEQ
  
    branch on equal (zero set)

    This instruction could also be called "Branch on Equal."

    It takes a conditional branch whenever the Z flag is on or the previ­ ous result is equal to 0.

    BEQ does not affect any of the flags or registers other than the program counter and only then when the Z flag is set.

        branch on Z = 1                            N Z C I D V
                                                   - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | relative    | BCS oper     | B0  | 2     | 2**    |  

- BMI
  
    branch on minus (negative set)

    This instruction takes the conditional branch if the N bit is set.

    BMI does not affect any of the flags or any other part of the machine other than the program counter and then only if the N bit is on.

        branch on N = 1                            N Z C I D V
                                                   - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | relative    | BMI oper     | 30  | 2     | 2**    |  
  
- BNE
  
    branch on not equal (zero clear)

    This instruction could also be called "Branch on Not Equal." It tests the Z flag and takes the conditional branch if the Z flag is not on, indicating that the previous result was not zero.

    BNE does not affect any of the flags or registers other than the program counter and only then if the Z flag is reset.
  
        branch on Z = 0                            N Z C I D V
                                                   - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | relative    | BNE oper     | D0  | 2     | 2**    |  

- BPL
  
    branch on plus (negative clear)

    This instruction is the complementary branch to branch on result minus. It is a conditional branch which takes the branch when the N bit is reset (0). BPL is used to test if the previous result bit 7 was off (0) and branch on result minus is used to determine if the previous result was minus or bit 7 was on (1).

    The instruction affects no flags or other registers other than the P counter and only affects the P counter when the N bit is reset.

        branch on N = 1 0                          N Z C I D V
                                                   - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | relative    | BPL oper     | 10  | 2     | 2**    |  
  
- BVC
  
    branch on overflow clear

    This instruction tests the status of the V flag and takes the conditional branch if the flag is not set.

    BVC does not affect any of the flags and registers other than the program counter and only when the overflow flag is reset.

        branch on V = 0                            N Z C I D V
                                                   - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | relative    | BVC oper     | 50  |   2   | 2**    |
  
- BVS
  
    branch on overflow set

    This instruction tests the V flag and takes the conditional branch if V is on.

    BVS does not affect any flags or registers other than the program, counter and only when the overflow flag is set.

        branch on V = 1                            N Z C I D V
                                                   - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | relative    | BVS oper     | 70  |   2   | 2**    |

## Jumps & Subroutines

  JSR and RTS affect the stack as the return address is pushed onto or pulled from the stack, respectively.
  (JSR will first push the high-byte of the return address [PC+2] onto the stack, then the low-byte. The stack will then contain, seen from the bottom or from the most recently added byte, [PC+2]-L [PC+2]-H.)

- JMP
  
    jump

    This instruction establishes a new valne for the program counter.

    It affects only the program counter in the microprocessor and affects no flags in the status register.
  
        (PC+1) -> PCL                              N Z C I D V
        (PC+2) -> PCH                              - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | absolute    | JMP oper     | 4C  |   3   |   3    |
    | relative    | JMP (oper)   | 6C  |   3   |   5    |

- JSR
  
    jump subroutine

    This instruction transfers control of the program counter to a subroutine location but leaves a return pointer on the stack to allow the user to return to perform the next instruction in the main program after the subroutine is complete. To accomplish this, JSR instruction stores the program counter address which points to the last byte of the jump instruc­ tion onto the stack using the stack pointer. The stack byte contains the program count high first, followed by program count low. The JSR then transfers the addresses following the jump instruction to the program counter low and the program counter high, thereby directing the program to begin at that new address.

    The JSR instruction affects no flags, causes the stack pointer to be decremented by 2 and substitutes new values into the program counter low and the program counter high.

        push (PC+2)                                N Z C I D V
        (PC+1) -> PCL                              - - - - - -
        (PC+2) -> PCH

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | absolute    | JSR oper     | 20  |   3   |   6    |
  
- RTS
  
    return from subroutine

    This instruction loads the program count low and program count high from the stack into the program counter and increments the program counter so that it points to the instruction following the JSR. The stack pointer is adjusted by incrementing it twice.

    The RTS instruction does not affect any flags and affects only PCL and PCH.

## Interrupts

  A hardware interrupt (maskable IRQ and non-maskable NMI), will cause the processor to put first the address currently in the program counter onto the stack (in HB-LB order), followed by the value of the status register. (The stack will now contain, seen from the bottom or from the most recently added byte, SR PC-L PC-H with the stack pointer pointing to the address below the stored contents of status register.) Then, the processor will divert its control flow to the address provided in the two word-size interrupt vectors at $FFFA (IRQ) and $FFFE (NMI).
  A set interrupt disable flag will inhibit the execution of an IRQ, but not of a NMI, which will be executed anyways.
  The break instruction (BRK) behaves like a NMI, but will push the value of PC+2 onto the stack to be used as the return address. Also, as with any software initiated transfer of the status register to the stack, the break flag will be found set on the respective value pushed onto the stack. Then, control is transferred to the address in the NMI-vector at $FFFE.
  In any way, the interrupt disable flag is set to inhibit any further IRQ as control is transferred to the interrupt handler specified by the respective interrupt vector.

  The RTI instruction restores the status register from the stack and behaves otherwise like the JSR instruction. (The break flag is always ignored as the status is read from the stack, as it isn't a real processor flag anyway.)

- BRK
  
    break / software interrupt

    The break command causes the microprocessor to go through an inter­ rupt sequence under program control. This means that the program counter of the second byte after the BRK. is automatically stored on the stack along with the processor status at the beginning of the break instruction. The microprocessor then transfers control to the interrupt vector.

    Other than changing the program counter, the break instruction changes no values in either the registers or the flags.

        interrupt,
        push PC+2, push SR                         N Z C I D V
                                                   - - - 1 - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | implied     | BRK          | 00  | 1     | 7      |
  
- RTI

    return from interrupt

    This instruction transfers from the stack into the microprocessor the processor status and the program counter location for the instruction which was interrupted. By virtue of the interrupt having stored this data before executing the instruction and thei fact that the RTI reinitializes the microprocessor to the same state as when it was interrupted, the combination of interrupt plus RTI allows truly reentrant coding.

    The RTI instruction reinitializes all flags to the position to the point they were at the time the interrupt was taken and sets the program counter back to its pre-interrupt state. It affects no other registers in the microprocessor.

        pull SR, pull PC                         N Z C I D V
                                                 from stack

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |   implied   |     RTI      | 40  |   1   |   6    |

## Other
  
- BIT

    Bit Test

    This instruction performs an AND between a memory location and the accumulator but does not store the result of the AND into the accumulator.

    The bit instruction affects the N flag with N being set to the value of bit 7 of the memory being tested, the V flag with V being set equal to bit 6 of the memory being tested and Z being set by the result of the AND operation between the accumulator and the memory if the result is Zero, Z is reset otherwise. It does not affect the accumulator.

        A AND M, M7 -> N, M6 -> V                  N Z C I D V
                                                  M7 + - - - M6

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | zeropage    | BIT oper     | 24  | 2     | 3      |  
    | absolute    | BIT oper     | 2C  | 3     | 4      |

- NOP

    No operation

        ---                                        N Z C I D V
                                                   - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    | implied     |    NOP       | EA  |   1   |   2    |  

# Addressing Mode

  The 6502 processor provides several ways in which memory locations can be addressed. Some instructions support several different modes while others may only support one. In addition the two index registers can not always be used interchangeably. This lack of orthogonality in the instruction set is one of the features that makes the 6502 trickier to program well.

## Implied Addressing

  These instructions act directly on one or more registers or flags internal to the CPU. Therefor, these instructions are principally single-byte instructions, lacking an explicit operand. The operand is implied, as it is already provided by the very instruction.

  Instructions targeting exclusively the contents of the accumulator may or may not be denoted by using an explicit "A" as the operand, depending on the flavor of syntax. (This may be regarded as a special address mode of its own, but it is really a special case of an implied instruction. It is still a single-byte instruction and no operand is provided in machine language.)

- CLC
  
    clear the carry flag
  
    This instruction initializes the carry flag to a 0. This op­ eration should normally precede an ADC loop. It is also useful when used with a R0L instruction to clear a bit in memory.

    This instruction affects no registers in the microprocessor and no f    lags other than the carry flag which is reset.

        0 -> C                                   N Z C I D V
                                                 - - 0 - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     CLC      | 18  |   1   | 2**    |

- ROL
  
    rotate contents of accumulator left by one position

    The rotate left instruction shifts either the accumulator or addressed memory left 1 bit, with the input carry being stored in bit 0 and with the input bit 7 being stored in the carry flags.

    The ROL instruction either shifts the accumulator left 1 bit and stores the carry in accumulator bit 0 or does not affect the internal reg­isters at all. The ROL instruction sets carry equal to the input bit 7, sets N equal to the input bit 6 , sets the Z flag if the result of the ro­ tate is 0, otherwise it resets Z and does not affect the overflow flag at all.
  
- TXA
  
    transfer contents of X-register to the accumulator

    This instruction moves the value that is in the index register X to the accumulator A without disturbing the content of the index register X.

    TXA does not affect any register other than the accumula­tor and does not affect the carry or overflow flag. If the result in A has bit 7 on, then the N flag is set, otherwise it is reset. If the resultant value in the accumulator is 0, then the Z flag is set, other­ wise it is reset.

        X -> A                                   N Z C I D V
                                                 + + - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     TXA      | 8A  |   1   |   2    |

- PHA
    push the contents of the accumulator to the stack

    This instruction transfers the current value of the accumulator to the next location on the stack, automatically decrementing the stack to point to the next empty location.

    The Push A instruction only affects the stack pointer register which is decremented by 1 as a result of the operation. It affects no flags.

        push A                                   N Z C I D V
                                                 - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |  implied    |     PHA      | 48  |   1   |   3    |

- RTS
  
    return from subroutine (by pulling PC from stack)

    This instruction loads the program count low and program count high from the stack into the program counter and increments the program counter so that it points to the instruction following the JSR. The stack pointer is adjusted by incrementing it twice.

    The RTS instruction does not affect any flags and affects only PCL and PCH.

    Mind that some of these instructions, while simple in appearance, may be quite complex operations, like "PHA", which involves the accumulator, the stack pointer and memory access.

        pull PC, PC + 1 -> PC                    N Z C I D V
                                                 - - - - - -

    |   Address   |  assembler   | opc | bytes | cycles |
    |:-----------:|:------------:|:---:|:-----:|:------:|
    |   implied   |     RTS      | 60  |   1   |   6    |

## Immediate Addressing

  Here, a literal operand is given immediately after the instruction. The operand is always an 8-bit value and the total instruction length is always 2 bytes. In memory, the operand is a single byte following immediately after the instruction code. In assembler, the mode is usually indicated by a "#" prefix adjacent to the operand.

    Mnemonic       Instruction 
                    __ __
    LDA #7         |A9|07|
                       ||
                       \/
                       __
                    A:|07|

  Mnemonic Examples:

  LDA #$07 =>  load the literal hexidecimal value "$7" into the accumulator<br>
  ADC #$A0 =>  add the literal hexidecimal value "$A0" to the accumulator<br>
  CPX #$32 =>  compare the X-register to the literal hexidecimal value "$32"

## Absolute Addressing

  Absolute addressing modes provides the 16-bit address of a memory location, the contents of which used as the operand to the instruction. In machine language, the address is provided in two bytes immediately after the instruction (making these 3-byte instructions) in low-byte, high-byte order (LLHH) or little-endian. In assembler, conventional numbers (HHLL order or big-endian words) are used to provide the address.

  Absolute addresses are also used for the jump instructions JMP and JSR to provide the address for the next instruction to continue with in the control flow.

    Mnemonic          Instruction     Data
                       __ __ __               __
    LDA #3010         |AD|10|30|      $3010: |34|
                                              ||
                                              \/
                                              __
                                          A: |34|   
  Mnemonic Examples:

  LDA $3010 => load the contents of address "$3010" into the accumulator<br>
  ROL $08A0 => rotate the contents of address "$08A0" left by one position<br>
  JMP $4000 => jump to (continue with) location "$4000"<br>

## Zero-Page Addressing

  The 16-bit address space available to the 6502 is thought to consist of 256 "pages" of 256 memory locations each ($00…$FF). In this model the high-byte of an address gives the page number and the low-byte a location inside this page. The very first of these pages, where the high-byte is zero (addresses $0000…$00FF), is somewhat special.

  The zero-page address mode is similar to absolute address mode, but these instructions use only a single byte for the operand, the low-byte, while the high-byte is assumed to be zero by definition. Therefore, these instructions have a total length of just two bytes (one less than absolute mode) and take one CPU cycle less to execute, as there is one byte less to fetch.

    Mnemonic        Instruction     Data
                     __ __                  __
    LDA #80         |A5|80|         $0080: |34|
                                            ||
                                            \/
                                            __
                                        A: |34|   

  Mnemonic Examples:

  LDA $80 => load the contents of address "$0080" into the accumulator<br>
  BIT $A2 => perform bit-test with the contents of address "$00A2"<br>
  ASL $9A => arithmetic shift left of the contents of location "$009A"

  (One way to think of the zero-page is as a page of 256 additional registers, somewhat slower than the internal registers, but with zero-page instructions also faster executing than "normal" instructions. The zero-page has a few more tricks up its sleeve, making these addresses perform more like real registers, see below.)

## Indexed Addressing: Absolute,X and Absolute,Y

  Indexed addressing adds the contents of either the X-register or the Y-register to the provided address to give the effective address, which provides the operand.

  These instructions are usefull to e.g., load values from tables or to write to a continuous segment of memory in a loop. The most basic forms are "absolute,X" and "absolute,X", where either the X- or the Y-register, respectively, is added to a given base address. As the base address is a 16-bit value, these are generally 3-byte instructions. Since there is an additional operation to perform to determine the effective address, these instructions are one cycle slower than those using absolute addressing mode.*

    Mnemonic        Instruction             Data
                     __ __ __        
    LDA #3120       |BD|20|31|       
                              \             
                               \             __
                                 + = $3132: |78|
                           __  /             ||
                       X: |12|/              \/
                                         A: |78|

  Mnemonic Examples:

  LDA $3120,X => load the contents of address "$3120 + X" into A<br>
  LDX $8240,Y => load the contents of address "$8240 + Y" into X<br>
  INC $1400,X => increment the contents of address "$1400 + X"

  *) If the addition of the contents of the index register effects in a change of the high-byte given by the base address so that the effective address is on the next memory page, the additional operation to increment the high-byte takes another CPU cycle. This is also known as a crossing of page boundaries.

## Indexed Addressing: Zero-Page,X (and Zero-Page,Y)

  As with absolute addressing, there is also a zero-page mode for indexed addressing. However, this is generally only available with the X-register. (The only exception to this is LDX, which has an indexed zero-page mode utilizing the Y-register.)<br>
  As we have already seen with normal zero-page mode, these instructions are one byte less in total length (two bytes) and take one CPU cycle less than instructions in absolute indexed mode.

  Unlike absolute indexed instructions with 16-bit base addresses, zero-page indexed instructions never affect the high-byte of the effective address, which will simply wrap around in the zero-page, and there is no penalty for crossing any page boundaries.

    Mnemonic        Instruction          Data
                     __ __         
    LDA #80,X       |B6|80|     
                           \             
                            \            __
                             + = $0082: |64|
                        __  /             ||
                    X: |02|/              \/
                                          __
                                      A: |64|

## Indirect Addressing

  This mode looks up a given address and uses the contents of this address and the next one (in LLHH little-endian order) as the effective address. In its basic form, this mode is available for the JMP instruction only. (Its generally use is jump vectors and jump tables.)<br>
  Like the absolute JMP instruction it uses a 16-bit address (3 bytes in total), but takes two additional CPU cycles to execute, since there are two additional bytes to fetch for the lookup of the effective jump target.

  Generally, indirect addressing is denoted by putting the lookup address in parenthesis.

    Mnemonic        Instruction     Lookup 
                     __ __ __               __ __
    JMP ($FF82)     |6C|82|FF|      $FF82: |C4|80|
                                             ||
                                             \/
                                       PC: $80C4 (Effective Taret)

  Mnemonic Exemple:

  JMP ($FF82) => jump to address given in addresses "$FF82" and "$FF83"

## Pre-Indexed Indirect, "(Zero-Page,X)"

  Indexed indirect address modes are generally available only for instructions supplying an operand to the accumulator (LDA, STA, ADC, SBC, AND, ORA, EOR, etc). The placement of the index register inside or outside of the parenthesis indicating the address lookup will give you clue what these instructions are doing.

  Pre-indexed indirect address mode is only available in combination with the X-register. It works much like the "zero-page,X" mode, but, after the X-register has been added to the base address, instead of directly accessing this, an additional lookup is performed, reading the contents of resulting address and the next one (in LLHH little-endian order), in order to determine the effective address.

  Like with "zero-page,X" mode, the total instruction length is 2 bytes, but there are two additional CPU cycles in order to fetch the effective 16-bit address. As "zero-page,X" mode, a lookup address will never overflow into the next page, but will simply wrap around in the zero-page.

  These instructions are useful, whenever we want to loop over a table of pointers to disperse addresses, or where we want to apply the same operation to various addresses, which we have stored as a table in the zero-page.

    Mnemonic        Instruction         Lookup          Data
                     __ __                            
    LDA ($70,X)     |A1|70|                           
                           \                          
                            \            __ __            __
                             + = $0075: |23|30|   $3032: |A5|        
                            /                             ||
                        __ /                              \/
                    X: |05|                               __
                                                      A: |A5|
  Mnemonic Examples:

  LDA ($70,X) => load the contents of the location given in addresses "$0070+X" and "$0070+1+X" into A<br/>
  STA ($A2,X) => store the contents of A in the location given in addresses "$00A2+X" and "$00A3+X"<br/>
  EOR ($BA,X) => perform an exlusive OR of the contents of A and the contents

## Post-Indexed Indirect, "(Zero-Page),Y"

  Post-indexed indirect addressing is only available in combination with the Y-register. As indicated by the indexing term ",Y" being appended to the outside of the parenthesis indicating the indirect lookup, here, a pointer is first read (from the given zero-page address) and resolved and only then the contents of the Y-register is added to this to give the effective address.

  Like with "zero-page,Y" mode, the total instruction length is 2 bytes, but there it takes an additional CPU cycles to resolve and index the 16-bit pointer. As with "absolute,X" mode, the effective address may overflow into the next page, in the case of which the execution uses an extra CPU cycle.

  These instructions are useful, wherever we want to perform lookups on varying bases addresses or whenever we want to loop over tables, the base address of which we have stored in the zero-page.

    Mnemonic        Instruction         Lookup              Data
                     __ __               __ __             
    LDA ($70,X)     |B1|70|      $0070: |43|35|                 
                                               \                          
                                                \            __ 
                                                 + = $3553: |23|
                                                /            ||
                                            __ /             \/
                                        Y: |10|              __
                                                         A: |23|

  LDA ($70,Y) => add the contents of the Y-register to the pointer provided in "$0070" and "$0071" and load the contents of this ddress into A<br/>
  STA ($A2,Y) => store the contents of A in the location given by the pointer in "$00A2" and "$00A3" plus the contents of the Y-register<br/>
  EOR ($BA,Y) => perform an exlusive OR of the contents of A and the address given by the addition of Y to the pointer in "$00BA" and "$00BB"

## Relative Addressing (Conditional Branching)

  This final address mode is exlusive to conditional branch instructions, which branch in the execution path depending on the state of a given CPU flag. Here, the instruction provides only a relative offset, which is added to the contents of the program counter (PC) as it points to the immediate next instruction. The relative offset is a signed single byte value in two's complement encoding (giving a range of −128…+127), which allows for branching up to half a page forwards and backwards.
  On the one hand, this makes these instructions compact, fast and relocatable at the same time. On the other hand, we have to mind that our branch target is no farther away than half a memory page.

  Generally, an assembler will take care of this and we only have to provide the target address, not having to worry about relative addressing.

  These instructions are always of 2 bytes length and perform in 2 CPU cycles, if the branch is not taken (the condition resolving to 'false'), and 3 cycles, if the branch is taken (when the condition is true). If a branch is taken and the target is on a different page, this adds another CPU cycle (4 in total).

    PC      Mnemonic     Instruction            Target       
                          __ __      
    $1000   BEQ $1005    |F0|03|
                                \ (offset)
    $1002----------------------- +     =>   PC: $1005
          PC pointing to next instruction

    BEQ $1005 => branch to location "$1005", if the zero flag is set. if the current address is $1000, this will give an offset of $03.<br/>
    BCS $08C4 => branch to location "$08C4", if the carry flag is set. if the current address is $08D4, this will give an offset of $EE (−$12).<br/>
    BCC $084A => branch to location "$084A", if the carry flag is clear.

# A Primer of 6502 Arithmetic Operations

  The 6502 processor features two basic arithmetic instructions, ADC, ADd with Carry,
  and SBC, SuBtract with Carry. As the names suggest, these provide addition and
  subtraction for single byte operands and results. However, operations are not
  limited to a single byte range, which is where the carry flag comes in, providing
  the means for a single-bit carry (or borrow), to combine operations over several
  bytes.

  In order to accomplish this, the carry is included in each of these operations:
  for additions, it is added (much like another operand); for subtractions, which are
  just an addition using the inverse of the operand (complement value of the operand),
  the role of the carry is inverted, as well.
  Therefore, it is crucial to set up the carry appropriatly: fo additions, the carry
  has to be initially cleared (using CLC), while for subtractions, it must be initally
  set (using SEC — more on SBC below).

               ;ADC: A = A + M + C
  CLC          ;clear carry in preparation
  LDA #2       ;load 2 into the accumulator
  ADD #3       ;add 3 -> now 5 in accumulator

               ;SBC: A = A - M - C̅   ("C̅": "not carry")
  SEC          ;set carry in preparation
  LDA #15      ;load 15 into the accumulator
  SBC #8       ;subtract 8 -> now 7 in accumulator

  Note: Here, we used direct mode, indicated by the prefix "#" before the operand, to
  directly load a literal value. If there is no such "#" prefix, we generally mean
  to use the value stored at the address, which is given by the operand. As we
  will see in the next example.)

  To combine this for 16-bit values (2 bytes each), we simply chain the instructions
  for the next bytes to operate on, but this time without setting or clearing the carry.

  Supposing two 16-bit values in

  $1000 ... first argument, low byte
  $1001 ... first argument, high byte

  $1002 ... second argument, low byte
  $1003 ... second argument, high byte

  and

  $1004 ... result, low byte
  $1005 ... result, high byte

  we perform a 16-bit addition by:

  CLC          ;prepare carry for addition
  LDA $1000    ;load value at address $1000 into A (low byte of first argument)
  ADC $1002    ;add low byte of second argument at $1002
  STA $1004    ;store low byte of result at $1004
  LDA $1001    ;load high byte of first argument
  ADC $1003    ;add high byte of second argument
  STA $1005    ;store high byte of result (result in $1004 and $1005)

  and, conversely, for a 16-bit subtraction:

  SEC          ;prepare carry for subtraction
  LDA $1000    ;load value at address $1000 into A (low byte of first argument)
  SBC $1002    ;subtract low byte of second argument at $1002
  STA $1004    ;store low byte of result at $1004
  LDA $1001    ;load high byte of first argument
  SBC $1003    ;subtract high byte of second argument
  STA $1005    ;store high byte of result (result in $1004 and $1005)

  Note: Another, important preparatory step is to set the processor into binary
  mode by use of the CLD (CLear Decimal flag) instruction. (Compare the section
  on decimal mode below.) This has to be done only once.

## Signed Values

  Operations for unsigned and signed values are principally the same, the only
  difference being in how we interpret the values. Generally, the 6502 uses what
  is known as two's complement to represent negative values.

  (In earlier computers,something known as ones' complement was used, where we
  simply flip all bits to their opposite state to represent a negative value.
  While simple, this came with a few drawbacks, like an additional value of
  negative zero, which are overcome by two's complement.)

  In two's complement representation, we simply flip all the bits in a byte to
  their opposite (the same as an XOR by $FF) and then add 1 to this.

  E.g., to represent -4:,
  (We here use "$" to indicate a hexadecimal number and "%" for binary notation.
  A dot is used to separate the high- and low-nibble, i.e. group of 4 bits.)

      %0000.0100     4

XOR %1111.1111   255
  -------------

      %1111.1011   complement (all bits flipped)

-           1

  -------------
      %1111.1100    -4, two's complement

  Thus, in a single byte, we may represent values in the range

  from  -128 (%1000.0000 or $80)
  to    +127 (%0111.1111 or $7F)

  A notable feature is that the highest value bit (first bit from the left) will
  always be 1 for a negative value and always be 0 for a positive one, for which
  it is also known as the sign bit. Whenever we interpret a value as a signed
  number, a set sign bit indicates a negative value.

  This works just the same for larger values, e.g., for a signed 16-bit value:

  512 = %1111.1110.0000.0000 = $FE $00
  -516 = %1111.1101.1111.1100 = $FD $FC (mind how the +1 step carries over)

  Notably, the binary operations are still the same as with unsigned values and
  provide the expected results:

    100  %0110.0100  $64

- -24  %1110.1000  $E8

  ---------------------
    76  %0100.1100  $4C  (+ carry)

  Note: We may now see how SBC actually works, by adding ones' complement of
  the operand to the accumulator. If we add 1 from the carry to the result,
  this effectively results in a subtraction in two's complement (the inverse
  of the operand + 1). If the carry happens to be zero, the result falls
  short by 1 in terms of two's complement, which is equivalent to adding 1
  to the operand before the subtraction. Thus, the carry either provides
  the correction required for a valid two's complement representation or,
  if missing, results in a subtraction including a binary borrow.

## FLags with ADC and SBC

  Besides the carry flag (C), which allows us to chain multi-byte operations, the
  CPU sets the following flags on the result of an arithmetic operation:

  zero flag (Z) ........ set if the result is zero, else unset
  negative flag (N)  ... the N flag always reflects the sign bit of the result
  overflow flag (V)  ... indicates overflow in signed operations

  The latter may require explanation: how is signed overflow different from
  the carry flag? The overflow flag is about a certain ambiguity of the sign bit
  and the negative flag in signed context: if operands are of the same sign, the
  case may occure, where the sign bit flips (as indicated by a change of the
  negative flag), while the result is still of the same sign. This condition is
  indicated by the overflow flag. Notably, such an overflow can never occur, when
  the operands are of opposite signs.

  E.g., adding positive $40 to positive $40:

            Acc  Acc binary  NVDIZC flags
  LDA #$40   $40  %0100.0000  000000
  ADC #$40   $80  %1000.0000  110000

  Here, the change of the sign bit is unrelated to the actual value in the
  accumulator, it is merely a consequence of carry propagation from bit 6 to
  bit 7, the sign bit. Since both operands are positive, the result must be
  positive, as well.
  The overflow flag (V) is of interest in signed context only and has no meaning
  in unsigned context.

## Decimal Mode (BCD)

  Besides binary arithmetic, the 6502 processor supports a second mode, binary
  coded decimal (BCD), where each byte, rather than representing a range of 0…255,
  represents two decimal digits packed into a single byte. For this, a byte is
  thought divided into two sections of 4 bits, the high- and the low-nibble. Only
  values from 0…9 are used for each nibble and a byte can represent a range of a
  2-digit decimal value only, as in 0…99.
  E.g.,

    14  %0001.0100  $14
    98  %1001.1000  $98

  Mind how this intuitively translates to hexadecimal notation, where figures
  A…F are never used.

  Whether or not the processor is in decimal mode is determined by the decimal
  flag (D). If it is set (using SED) the processor will use BCD arithmetic.
  If it is cleared (using CLD), the processor is in binary mode.
  Decimal mode only affects instructions ADC and SBC (but not INC or DEC.)

  Examples:

    SED
    CLC
    LDA #$12
    ADC #$44  ;accumulator now holds $56

    SED
    CLC
    LDA #$28
    ADC #$14  ;accumulator now holds $42

    Mind that BCD mode is always unsigned:

            Acc  NVDIZC flags
    SED
    SEC
    LDA #0   $00  001011
    SBC #1   $99  101000

  The carry flag and the zero flag work in decimal mode as expected.
  The negative flag is set similar to binary mode (and of questionable value.)
  The overflow flag has no meaning in decimal mode.

  Multi-byte operations are just as in decimal mode: We first prepare the carry
  and then chain operations of the individual bytes in increasing value order,
  starting with the lowest value pair.

  (It may be important to note that Western Design Center (WDC) version of
  the processor, the 65C02, always clears the decimal flag when it enters an
  interrupt, while the original NMOS version of the 6502 does not.)
