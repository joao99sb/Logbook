# ref

- https://www.nesdev.org/wiki/CPU
- https://www.pagetable.com/c64ref/6502/
- https://www.masswerk.at/6502/6502_instruction_set.html

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

  ### Following is a list of the flags, starting from the 8th bit of the P register (bit 7, value $80):
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
    I Interrupt disable flag
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

  ## Transfer Instructions
  Load, store, interregister transfer
  
  1. Load/Store Operations<br>
    These instructions transfer a single byte between memory and one of the registers. Load operations set the negative (N) and zero (Z) flags depending on the value of transferred. Store operations do not affect the flag settings.

  - LDA

    load accumulator

    When instruction LDA is executed by the microprocessor, data is transferred from memory to the accumulator and stored in the accumulator.

    LDA affects the contents of the accumulator, does not affect the carry or overflow flags; sets the zero flag if the accumulator is zero as a result of the LDA, otherwise resets the zero flag; sets the negative flag if bit 7 of the accumulator is a 1, other­ wise resets the negative flag.

  - LDX

    load X

    Load the index register X from memory.

    LDX does not affect the C or V flags; sets Z if the value loaded was zero, otherwise resets it; sets N if the value loaded in bit 7 is a 1; otherwise N is reset, and affects only the X register.

  - LDY

    load Y

    Load the index register Y from memory.

    LDY does not affect the C or V flags, sets the N flag if the value loaded in bit 7 is a 1, otherwise resets N, sets Z flag if the loaded value is zero otherwise resets Z and only affects the Y register.

  - STA

    store accumulator

    This instruction transfers the contents of the accumulator to memory.

    This instruction affects none of the flags in the processor status register and does not affect the accumulator.

  - STX

    store X

    Transfers value of X register to addressed memory location.

    No flags or registers in the microprocessor are affected by the store operation.

  - STY

    store Y
    
    Transfer the value of the Y register to the addressed memory location.

    STY does not affect any flags or registers in the microprocessor.

  
  
  2. Register Transfers<br>
  The contents of the X and Y registers can be moved to or from the accumulator, setting the negative (N) and zero (Z) flags as appropriate.
    
  - TAX

    transfer accumulator to X

    This instruction takes the value from accumulator A and trans­ fers or loads it into the index register X without disturbing the content of the accumulator A.

    TAX only affects the index register X, does not affect the carry or overflow flags. The N flag is set if the resultant value in the index register X has bit 7 on, otherwise N is reset. The Z bit is set if the content of the register X is 0 as aresult of theopera­ tion, otherwise it is reset.


  - TAY

    transfer accumulator to Y

    This instruction moves the value of the accumulator into index register Y without affecting the accumulator.

    TAY instruction only affects the Y register and does not affect either the carry or overflow flags. If the index register Y has bit 7 on, then N is set, otherwise it is reset. If the content of the index register Y equals 0 as a result of the operation, Z is set on, otherwise it is reset.

  - TXA

    transfer X to accumulator

    This instruction moves the value that is in the index register X to the accumulator A without disturbing the content of the index register X.

    TXA does not affect any register other than the accumula­tor and does not affect the carry or overflow flag. If the result in A has bit 7 on, then the N flag is set, otherwise it is reset. If the resultant value in the accumulator is 0, then the Z flag is set, other­ wise it is reset.
    
  - TYA

    transfer Y to accumulator
    
    This instruction moves the value that is in the index register Y to accumulator A without disturbing the content of the register Y.

    TYA does not affect any other register other than the accumula­ tor and does not affect the carry or overflow flag. If the result in the accumulator A has bit 7 on, the N flag is set, otherwise it is reset. If the resultant value in the accumulator A is 0, then the Z flag is set, otherwise it is reset.
    
    
  ## Stack Instructions
  These instructions transfer the accumulator or status register (flags) to and from the stack. The processor stack is a last-in-first-out (LIFO) stack of 256 bytes length, implemented at addresses $0100 - $01FF. The stack grows down as new values are pushed onto it with the current insertion point maintained in the stack pointer register.
  (When a byte is pushed onto the stack, it will be stored in the address indicated by the value currently in the stack pointer, which will be then decremented by 1. Conversely, when a value is pulled from the stack, the stack pointer is incremented. The stack pointer is accessible by the TSX and TXS instructions.)

  - TXS

    transfer X to stack pointer

    This instruction transfers the value in the index register X to the stack pointer.

    TXS changes only the stack pointer, making it equal to the content of the index register X. It does not affect any of the flags.



  - TSX

    transfer stack pointer to X

    This instruction transfers the value in the stack pointer to the index register X.

    TSX does not affect the carry or overflow flags. It sets N if bit 7 is on in index X as a result of the instruction, otherwise it is reset. If index X is zero as a result of the TSX, the Z flag is set, other­ wise it is reset. TSX changes the value of index X, making it equal to the content of the stack pointer.

  - PHA

    push accumulator

    This instruction transfers the current value of the accumulator to the next location on the stack, automatically decrementing the stack to point to the next empty location.

    The Push A instruction only affects the stack pointer register which is decremented by 1 as a result of the operation. It affects no flags.

  - PHP

    push processor status register (with break flag set)

    This instruction transfers the contents of the processor status reg­ ister unchanged to the stack, as governed by the stack pointer.

    The PHP instruction affects no registers or flags in the micropro­cessor.

  - PLA

    pull accumulator

    This instruction adds 1 to the current value of the stack pointer and uses it to address the stack and loads the contents of the stack into the A register.

    The PLA instruction does not affect the carry or overflow flags. It sets N if the bit 7 is on in accumulator A as a result of instructions, otherwise it is reset. If accumulator A is zero as a result of the PLA, then the Z flag is set, otherwise it is reset. The PLA instruction changes content of the accumulator A to the contents of the memory location at stack register plus 1 and also increments the stack register.

  - PLP

    pull processor status register 
    
    This instruction transfers the next value on the stack to the Proces­ sor Status register, thereby changing all of the flags and setting the mode switches to the values from the stack.

    The PLP instruction affects no registers in the processor other than the status register. **This instruction could affect all flags in the status register**.



  ## Logical Operations
  The following instructions perform logical operations on the contents of the accumulator and another value held in memory. The BIT instruction performs a logical AND to test the presence of bits in the memory value to set the flags but does not keep the result.

  - AND
  
    and (with accumulator)

    The AND instruction transfer the accumulator and memory to the adder which performs a bit-by-bit AND operation and stores the result back in the accumulator.

    This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise resets the zero flag; sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the

  - EOR

    exclusive or (with accumulator)

    The EOR instruction transfers the memory and the accumulator to the adder which performs a binary "EXCLUSIVE OR" on a bit-by-bit basis and stores the result in the accumulator.

    This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise resets the zero flag sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the negative flag.

  - ORA
  
    (inclusive) or with accumulator
    The ORA instruction transfers the memory and the accumulator to the adder which performs a binary "OR" on a bit-by-bit basis and stores the result in the accumulator.

    This instruction affects the accumulator; sets the zero flag if the result in the accumulator is 0, otherwise resets the zero flag; sets the negative flag if the result in the accumulator has bit 7 on, otherwise resets the negative flag.
  

  ## Arithmetic Operations
  The arithmetic operations perform addition and subtraction on the contents of the accumulator. The compare operations allow the comparison of the accumulator and X or Y with memory values.

  - ADC 

    Add with Carry

    This instruction adds the value of memory and carry from the previous operation to the value of the accumulator and stores the result in the accumulator.

    This instruction affects the accumulator; sets the carry flag when the sum of a binary add exceeds 255 or when the sum of a decimal add exceeds 99, otherwise carry is reset. The overflow flag is set when the sign or bit 7 is changed due to the result exceeding +127 or -128, otherwise overflow is reset. The negative flag is set if the accumulator result contains bit 7 on, otherwise the negative flag is reset. The zero flag is set if the accumulator result is 0, otherwise the zero flag is reset.
    
    Note on the MOS 6502:
    In decimal mode, the N, V and Z flags are not consistent with the decimal result.

  - SBC

    Subtract with Carry

    This instruction subtracts the value of memory and borrow from the value of the accumulator, using two's complement arithmetic, and stores the result in the accumulator. Borrow is defined as the carry flag complemented; therefore, a resultant carry flag indicates that a borrow has not occurred.

    This instruction affects the accumulator. The carry flag is set if the result is greater than or equal to 0. The carry flag is reset when the result is less than 0, indicating a borrow. The over­flow flag is set when the result exceeds +127 or -127, otherwise it is reset. The negative flag is set if the result in the accumulator has bit 7 on, otherwise it is reset. The Z flag is set if the result in the accumulator is 0, otherwise it is reset.

    Note on the MOS 6502:
    In decimal mode, the N, V and Z flags are not consistent with the decimal result.

  
  ## Shift & Rotate Instructions
  All shift and rotate instructions preserve the bit shifted out in the carry flag.

  - ASL
    arithmetic shift left (shifts in a zero bit on the right)
    
    The shift left instruction shifts either the accumulator or the address memory location 1 bit to the left, with the bit 0 always being set to 0 and the the input bit 7 being stored in the carry flag. ASL either shifts the accumulator left 1 bit or is a read/modify/write instruction that affects only memory.

    The instruction does not affect the overflow bit, sets N equal to the result bit 7 (bit 6 in the input), sets Z flag if the result is equal to 0, otherwise resets Z and stores the input bit 7 in the carry flag.

  - LSR

    logical shift right (shifts in a zero bit on the left)

    This instruction shifts either the accumulator or a specified memory location 1 bit to the right, with the higher bit of the result always being set to 0, and the low bit which is shifted out of the field being stored in the carry flag.

    The shift right instruction either affects the accumulator by shift­ing it right 1 or is a read/modify/write instruction which changes a speci­fied memory location but does not affect any internal registers. The shift right does not affect the overflow flag. The N flag is always reset. The Z flag is set if the result of the shift is 0 and reset otherwise. The carry is set equal to bit 0 of the input.

  - ROL

    rotate left (shifts in carry bit on the right)

    The rotate left instruction shifts either the accumulator or addressed memory left 1 bit, with the input carry being stored in bit 0 and with the input bit 7 being stored in the carry flags.

    The ROL instruction either shifts the accumulator left 1 bit and stores the carry in accumulator bit 0 or does not affect the internal reg­isters at all. The ROL instruction sets carry equal to the input bit 7, sets N equal to the input bit 6 , sets the Z flag if the result of the ro­ tate is 0, otherwise it resets Z and does not affect the

  - ROR

    rotate right (shifts in zero bit on the left)

    The rotate right instruction shifts either the accumulator or addressed memory right 1 bit with bit 0 shifted into the carry and carry shifted into bit 7.

    The ROR instruction either shifts the accumulator right 1 bit and stores the carry in accumulator bit 7 or does not affect the internal regis­ ters at all. The ROR instruction sets carry equal to input bit 0, sets N equal to the input carry and sets the Z flag if the result of the rotate is 0; otherwise it resets Z and does not affect the overflow flag at all.

  ## Flag Instructions
  The following instructions change the values of specific status flags.

  - CLC

    clear carry

    This instruction initializes the carry flag to a 0. This op­ eration should normally precede an ADC loop. It is also useful when used with a R0L instruction to clear a bit in memory.

    This instruction affects no registers in the microprocessor and no flags other than the carry flag which is reset.

  - CLD

    clear decimal (BCD arithmetics disabled)
    
    This instruction sets the decimal mode flag to a 0. This all subsequent ADC and SBC instructions to operate as simple operations.

    CLD affects no registers in the microprocessor and no flags other than the decimal mode flag which is set to a 0.



  - CLI

    This instruction initializes the interrupt disable to a 0. This allows the microprocessor to receive interrupts.

    It affects no registers in the microprocessor and no flags other than the interrupt disable which is cleared.

    clear interrupt disable

    
  
  - CLV

    clear overflow

    This instruction clears the overflow flag to a 0. This com­ mand is used in conjunction with the set overflow pin which can change the state of the overflow flag with an external signal.

    CLV affects no registers in the microprocessor and no flags other than the overflow flag which is set to a 0.
  
  - SEC

    set carry

    This instruction initializes the carry flag to a 1. This op eration should normally precede a SBC loop. It is also useful when used with a ROL instruction to initialize a bit in memory to a 1.

    This instruction affects no registers in the microprocessor and no flags other than the carry flag which is set.
  
  - SED

    set decimal (BCD arithmetics enabled)

    This instruction sets the decimal mode flag D to a 1. This makes all subsequent ADC and SBC instructions operate as a decimal arithmetic operation.

    SED affects no registers in the microprocessor and no flags other than the decimal mode which is set to a 1.
  
  - SEI

    set interrupt disable

    This instruction initializes the interrupt disable to a 1. It is used to mask interrupt requests during system reset operations and during interrupt commands.

    It affects no registers in the microprocessor and no flags other than the interrupt disable which is set.

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
  
  - CPX
  
    compare with X

    This instruction subtracts the value of the addressed memory location from the content of index register X using the adder but does not store the result; therefore, its only use is to set the N, Z and C flags to allow for comparison between the index register X and the value in memory.

    The CPX instruction does not affect any register in the machine; it also does not affect the overflow flag. It causes the carry to be set on if the absolute value of the index register X is equal to or greater than the data from memory. If the value of the memory is greater than the content of the index register X, carry is reset. If the results of the subtraction contain a bit 7, then the N flag is set, if not, it is reset. If the value in memory is equal to the value in index register X, the Z flag is set, otherwise it is reset.
  
  - CPY
  
    compare with Y

    This instruction performs a two's complement subtraction between the index register Y and the specified memory location. The results of the subtraction are not stored anywhere. The instruction is strict­ly used to set the flags.

    CPY affects no registers in the microprocessor and also does not affect the overflow flag. If the value in the index register Y is equal to or greater than the value in the memory, the carry flag will be set, otherwise it will be cleared. If the results of the subtract- tion contain bit 7 on the N bit will be set, otherwise it will be cleared. If the value in the index register Y and the value in the memory are equal, the zero flag will be set, otherwise it will be cleared.


  ## Comparisons
  Branch targets are relative, signed 8-bit address offsets. (An offset of #0 corresponds to the immedately following address — or a rather odd and expensive NOP.)

  
  - BCC
  
    branch on carry clear

    This instruction tests the state of the carry bit and takes a conditional branch if the carry bit is reset.

    It affects no flags or registers other than the program counter and then only if the C flag is not on.
  
  - BCS
  
    branch on carry set

    This instruction takes the conditional branch if the carry flag is on.

    BCS does not affect any of the flags or registers except for the program counter and only then if the carry flag is on.
  
  - BEQ
  
    branch on equal (zero set)

    This instruction could also be called "Branch on Equal."

    It takes a conditional branch whenever the Z flag is on or the previ­ ous result is equal to 0.

    BEQ does not affect any of the flags or registers other than the program counter and only then when the Z flag is set.

  - BMI
  
    branch on minus (negative set)

    This instruction takes the conditional branch if the N bit is set.

    BMI does not affect any of the flags or any other part of the machine other than the program counter and then only if the N bit is on.
  
  - BNE
  
    branch on not equal (zero clear)

    This instruction could also be called "Branch on Not Equal." It tests the Z flag and takes the conditional branch if the Z flag is not on, indicating that the previous result was not zero.

    BNE does not affect any of the flags or registers other than the program counter and only then if the Z flag is reset.
  
  - BPL
  
    branch on plus (negative clear)

    This instruction is the complementary branch to branch on result minus. It is a conditional branch which takes the branch when the N bit is reset (0). BPL is used to test if the previous result bit 7 was off (0) and branch on result minus is used to determine if the previous result was minus or bit 7 was on (1).

    The instruction affects no flags or other registers other than the P counter and only affects the P counter when the N bit is reset.
  
  - BVC
  
    branch on overflow clear

    This instruction tests the status of the V flag and takes the conditional branch if the flag is not set.

    BVC does not affect any of the flags and registers other than the program counter and only when the overflow flag is reset.
  
  - BVS
  
    branch on overflow set

    This instruction tests the V flag and takes the conditional branch if V is on.

    BVS does not affect any flags or registers other than the program, counter and only when the overflow flag is set.


  ## Jumps & Subroutines
  JSR and RTS affect the stack as the return address is pushed onto or pulled from the stack, respectively.
  (JSR will first push the high-byte of the return address [PC+2] onto the stack, then the low-byte. The stack will then contain, seen from the bottom or from the most recently added byte, [PC+2]-L [PC+2]-H.)

  
  - JMP
  
    jump

    This instruction establishes a new valne for the program counter.

    It affects only the program counter in the microprocessor and affects no flags in the status register.
  
  - JSR
  
    jump subroutine

    This instruction transfers control of the program counter to a subroutine location but leaves a return pointer on the stack to allow the user to return to perform the next instruction in the main program after the subroutine is complete. To accomplish this, JSR instruction stores the program counter address which points to the last byte of the jump instruc­ tion onto the stack using the stack pointer. The stack byte contains the program count high first, followed by program count low. The JSR then transfers the addresses following the jump instruction to the program counter low and the program counter high, thereby directing the program to begin at that new address.

    The JSR instruction affects no flags, causes the stack pointer to be decremented by 2 and substitutes new values into the program counter low and the program counter high.
  
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


  
  - RTI

    return from interrupt

    This instruction transfers from the stack into the microprocessor the processor status and the program counter location for the instruction which was interrupted. By virtue of the interrupt having stored this data before executing the instruction and thei fact that the RTI reinitializes the microprocessor to the same state as when it was interrupted, the combination of interrupt plus RTI allows truly reentrant coding.

    The RTI instruction reinitializes all flags to the position to the point they were at the time the interrupt was taken and sets the program counter back to its pre-interrupt state. It affects no other registers in the microprocessor.

  ## Other
  
  - BIT   

    Bit Test     

    This instruction performs an AND between a memory location and the accumulator but does not store the result of the AND into the accumulator.   

    The bit instruction affects the N flag with N being set to the value of bit 7 of the memory being tested, the V flag with V being set equal to bit 6 of the memory being tested and Z being set by the result of the AND operation between the accumulator and the memory if the result is Zero, Z is reset otherwise. It does not affect the accumulator.
    
  - NOP

    No operation

  
  



# Addressing Mode
  The 6502 processor provides several ways in which memory locations can be addressed. Some instructions support several different modes while others may only support one. In addition the two index registers can not always be used interchangeably. This lack of orthogonality in the instruction set is one of the features that makes the 6502 trickier to program well.


  ## Implied Addressing
  These instructions act directly on one or more registers or flags internal to the CPU. Therefor, these instructions are principally single-byte instructions, lacking an explicit operand. The operand is implied, as it is already provided by the very instruction.

  Instructions targeting exclusively the contents of the accumulator may or may not be denoted by using an explicit "A" as the operand, depending on the flavor of syntax. (This may be regarded as a special address mode of its own, but it is really a special case of an implied instruction. It is still a single-byte instruction and no operand is provided in machine language.)

  
  - CLC
  
    clear the carry flag
  
    This instruction initializes the carry flag to a 0. This op­ eration should normally precede an ADC loop. It is also useful when used with a R0L instruction to clear a bit in memory.

    This instruction affects no registers in the microprocessor and no f    lags other than the carry flag which is reset.

  - ROL
  
    rotate contents of accumulator left by one position

    The rotate left instruction shifts either the accumulator or addressed memory left 1 bit, with the input carry being stored in bit 0 and with the input bit 7 being stored in the carry flags.

    The ROL instruction either shifts the accumulator left 1 bit and stores the carry in accumulator bit 0 or does not affect the internal reg­isters at all. The ROL instruction sets carry equal to the input bit 7, sets N equal to the input bit 6 , sets the Z flag if the result of the ro­ tate is 0, otherwise it resets Z and does not affect the overflow flag at all.
  
  - TXA
  
    transfer contents of X-register to the accumulator

    This instruction moves the value that is in the index register X to the accumulator A without disturbing the content of the index register X.

    TXA does not affect any register other than the accumula­tor and does not affect the carry or overflow flag. If the result in A has bit 7 on, then the N flag is set, otherwise it is reset. If the resultant value in the accumulator is 0, then the Z flag is set, other­ wise it is reset.
  
  - PHA<br>  
    push the contents of the accumulator to the stack

    This instruction transfers the current value of the accumulator to the next location on the stack, automatically decrementing the stack to point to the next empty location.

    The Push A instruction only affects the stack pointer register which is decremented by 1 as a result of the operation. It affects no flags.
  
  - RTS
  
    return from subroutine (by pulling PC from stack)

    This instruction loads the program count low and program count high from the stack into the program counter and increments the program counter so that it points to the instruction following the JSR. The stack pointer is adjusted by incrementing it twice.

    The RTS instruction does not affect any flags and affects only PCL and PCH.

  Mind that some of these instructions, while simple in appearance, may be quite complex operations, like "PHA", which involves the accumulator, the stack pointer and memory access.




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
                            \             __
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