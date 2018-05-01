#include <stdio.h>

/* 
 * Simple Assembler using C Macro
 */ 

/* Field definitions */
#define OP(op) ((op) << 26)
#define RS(rs) ((rs) << 21)
#define RT(rt) ((rt) << 16)
#define RD(rd) ((rd) << 11)

/* Field definitions for decode */
#define GETOP(inst) (((inst) >> 26) & 0x3F)
#define GETRS(inst) (((inst) >> 21) & 0x1F)
#define GETRT(inst) (((inst) >> 16) & 0x1F)
#define GETRD(inst) (((inst) >> 11) & 0x1F)
#define GETFUNCT(inst) ((inst) & 0x3F)
#define GETIM(inst) ((inst) & 0xFFFF)

/* Operation type definitions */
#define R(op, rs, rt, rd, funct) (OP(op) | RS(rs) | RT(rt) | RD(rd) | (funct))
#define I(op, rs, rt, im) (OP(op) | RS(rs) | RT(rt) | (im))

/* Operation definitions */
#define OPALO 0x0
#define FADD 0x20
#define ADD(rd, rs, rt) R(OPALO, rs, rt, rd, FADD)
#define FSUB 0x22
#define SUB(rd, rs, rt) R(OPALO, rs, rt, rd, FSUB)

#define OPADDI 0x08
#define ADDI(rt, rs, im) I(OPADDI, rs, rt, im)

#define OPBEQ 0x04
#define BNE(rs, rt, im) I(OPBEQ, rs, rt, im)

/* 
 * Memory and Registers
 */
typedef unsigned int uint32;
uint32 memory[] = {
    ADDI(1, 0, 5), // constant 5
    ADDI(2, 0, 0), // counter
    ADDI(2, 2, 1), // counter++
    BNE(1, 2, -2)    
};

int regs[32] = {0};

/* 
 * Body
 */
int main () {
    int pc = 0;

    for (;;) {
        /* fetch */
        if (pc >= sizeof(memory)/sizeof(memory[0])) {
            break;
        }
        uint32 ir = memory[pc];

        /* decode */
        uint32 rs = GETRS(ir);
        uint32 rt = GETRT(ir);
        uint32 rd = GETRD(ir);
        uint32 funct = GETFUNCT(ir);
        int im = GETIM(ir);
        pc++;

        /* execute */
        switch (GETOP(ir)) {
        case OPALO:
            switch (funct) {
            case FADD:
                regs[rd] = regs[rs] + regs[rt];
                break;
            case FSUB:
                regs[rd] = regs[rs] - regs[rt];
                break;
            }
            break;

        case OPADDI:
            regs[rt] = regs[rs] + im; 
            break;

        case OPBEQ:
            if (regs[rs] != regs[rd]) {
                pc += im;
            }
            break;

        default: /* illigal operation */
            printf ("illigal operation\n");
            break;
            goto eoe;
        }

        {
            int i;
            for (i=0 ; i<32 ; i++) {
                if (i % 8 == 0) {
                    printf ("\n");
                }
                printf ("r%02d: %d, ", i, regs[i]);
            }
            printf ("\n");
        }
    }

eoe:
    return 0;
}