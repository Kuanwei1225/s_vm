// this file is used for tutorial to build the compiler step by step

#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define int long long // work with 64bit target

enum { MV_CONST,
    LD_C,
    LD_I,
    PUSH,
    POP,
    JMP,
    JZ,
    JNZ,
    CALL,
    RET,
    LD_ARG,
    OR,
    AND,
    EQU,
    NEQ,
    GT,
    GE,
    LT,
    LE,
    SHR,
    SHL,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    NOP,
    EXIT };
/**
 * Test program
 *
 * int sub(a, b) {
 	if(a > b)
		return a - b;
	else
		return b - a;
 * }
 * void main(void){
 	sub(30, 10);
 * }
 */

static int test[] = {
	MV_CONST, 20, 				//0
	PUSH, 						//2
	MV_CONST, 33, 				//3
	PUSH, 						//5
	CALL, (int)&test[11], 2,	//6
	EXIT,						//9
    NOP,						//10
    LD_ARG, 0,					//11 	get b 
    PUSH,						//13
    LD_ARG, 1,					//14 	get a
    GT,							//16 	if(a > b)
    JZ, (int)&test[26],			//17	a > b
    LD_ARG, 0,					//19	get b
    PUSH,						//21	
    LD_ARG, 1,					//22	get a
    SUB,						//24	a - b
    RET,						//25
    LD_ARG, 1,					//26	get a
    PUSH,						//28
    LD_ARG, 0,					//29	get b
    SUB,						//30	b - a
    RET							//31
};
#define NCODE() (*pc++)
#define VPUSH(val) (*(--sp) = (int)(val))
#define VPOP() (*sp++)
int eval(int* pgm)
{
	int *pc, *bp, *sp, ax; 		// virtual machine registers
    int op, tmp, *addr = 0;		// auxiliary variable

    bp = sp = (int*)((char*)pgm + pgm[2] + pgm[1] - pgm[0]); // offset[1] - offset[0] = poolsize
    pc = (int*)((char*)pgm + pgm[0]);

    while (1) {
        op = NCODE(); // get next operation code
        switch (op) {
        case MV_CONST:
            ax = NCODE();
            break;
        case LD_C:
            tmp = VPOP();
            ax = *(char*)tmp;
            break;
        case LD_I:
            tmp = VPOP();
            ax = *(int*)tmp;
            break;
        case PUSH:
            VPUSH(ax);
            break;
        case POP:
            ax = VPOP();
            break;
        case JMP:
            pc = (int*)NCODE();
            break;
        case JZ:
            addr = (int*)NCODE();
            pc = (ax) ? (pc) : (addr);
            break;
        case JNZ:
            addr = (int*)NCODE();
            pc = (ax) ? (addr) : (pc);
            break;
        case CALL:
            addr = (int*)NCODE(); // get function address
            tmp = NCODE(); // get number of argument to send to function
            VPUSH(tmp); // save how many argument to send
            VPUSH(bp); // base frame, to send argument
            VPUSH(pc); // save return address
            bp = sp;
            sp -= tmp; // make new stack frame
            pc = addr;
            break;
        case RET:
            sp = bp;
            pc = (int*)VPOP(); // return address
            bp = (int*)VPOP();
            tmp = VPOP(); // how many argument be sended
            sp += tmp; // remove frame
            break;
        case LD_ARG:
            tmp = NCODE(); // get offset
            ax = *(bp + 3 + tmp);
            break;
        case OR:
            ax |= VPOP();
            break;
        case AND:
            ax &= VPOP();
            break;
        case EQU:
            tmp = VPOP();
            ax = (ax == tmp);
            break;
        case NEQ:
            tmp = VPOP();
            ax = (ax != tmp);
            break;
        case GT:
            tmp = VPOP();
            ax = (ax > tmp);
            break;
        case GE:
            tmp = VPOP();
            ax = (ax >= tmp);
            break;
        case LT:
            tmp = VPOP();
            ax = (ax < tmp);
            break;
        case LE:
            tmp = VPOP();
            ax = (ax >= tmp);
            break;
        case SHR:
            ax >>= VPOP();
            break;
        case SHL:
            ax <<= VPOP();
            break;
        case ADD:
            ax += VPOP();
            break;
        case SUB:
            ax -= VPOP();
            break;
        case MUL:
            ax *= VPOP();
            break;
        case DIV:
            ax /= VPOP();
            break;
        case MOD:
            ax %= VPOP();
            break;
        case NOP:
            break;
        case EXIT:
            printf("EXIT: %lld\n", ax);
            return 0;
        default:
            printf("unknown instruction:%lld\n", op);
            return -1;
        }
    }
    return 0;
}
//
//	initial memory for virtual machine
//	@param size of each section
//	@ret an execuable memory
//
int* init_vm(int poolsize)
{
    int* exefile;
    int exe_size;
    int* offset;

    exe_size = 3 * poolsize + 3 * sizeof(offset);
    // allocate memory for virtual machine
    if (!(exefile = (int*)malloc(exe_size))) {
        printf("could not malloc(%lld) for exe file\n", exe_size);
        return NULL;
    }
    memset(exefile, 0, exe_size);
    offset = exefile;
    offset[0] = 3 * sizeof(offset); // text offset
    offset[1] = offset[0] + poolsize; // data offset
    offset[2] = offset[1] + poolsize; // stack offset

	// teat program
	memcpy((char*)exefile + offset[0], test, sizeof(test));
	//eval(test);
    return exefile;
}

