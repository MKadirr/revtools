#include "parser.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>

#define SPECIAL_MASK

static const char* strregis(uint n);

static
int nitoggle = 0;

static
void print0i(const char* ins,
             uint imm,
             const char *end)
{
    printf("%-10s 0x%x %s", ins, 
                        imm, 
                        end);

}

static
void print1(const char* ins,
             uint rs,
             const char *end)
{
    printf("%-10s %s %s", ins, 
                        strregis(rs), 
                        end);

}

static
void print1i(const char* ins,
            uint rs,
            uint imm,
            const char *end) 
{

    printf("%-10s %s,0x%x %s", ins, 
                           strregis(rs),
                           imm,
                           end);
}
static
void printoffb(const char* ins,
        uint rt,
        uint offset,
        uint base,
        const char *end)
{
    printf("%-10s %s, 0x%x(%s) %s",
                        ins,
                        strregis(rt),
                        offset,
                        strregis(base),
                        end);
}

static
void print2(const char* ins,
            uint rd, 
            uint rs, 
            const char *end) 
{

    printf("%-10s %s,%s %s", ins, 
                           strregis(rd), 
                           strregis(rs), 
                           end);
}

static
void print2cc(const char* ins,
            uint rd, 
            uint rs, 
            uint cc,
            const char *end) 
{

    printf("%-10s %s,%s,%u %s", ins, 
                           strregis(rd), 
                           strregis(rs),
                           cc,
                           end);
}


static
void print2i(const char* ins,
            uint rd, 
            uint rs,
            uint imm,
            const char *end) 
{

    printf("%-10s %s,%s,0x%x %s", ins, 
                           strregis(rd), 
                           strregis(rs),
                           imm,
                           end);
}


static
void print3(const char *ins,
            uint rd,
            uint rs,
            uint rt,
            const char *end)
{
    printf("%-10s %s,%s,%s %s", ins,
                              strregis(rd), 
                              strregis(rs), 
                              strregis(rt), 
                              end);
}

static
void setup(struct assembler *file)
{
    if(file == NULL || file->data == NULL)
        errx(1, "setup: file or data NULL");


    unsigned char t[] = {0x41, 0x0, 0x2, 0x3c};
    printf("test:");
    for(int i = 0; i < 4; i++)
        printf(" %hhx", file->data[i]);
    printf(" => %x \n", ((unsigned int *)file->data)[0]);
    printf("t2: %x\n", ((unsigned int *)file->data)[0] & 0xff);
    printf("test:");
    for(int i = 0; i < 4; i++)
        printf(" %hhx", t[i]);
    printf(" => %x \n", ((unsigned int *)t)[0]);
    printf("t2: %x\n", ((unsigned int *)t)[0] & 0xff);
    if(file->endian == BENDIAN)
    {
        unsigned char *data = file->data;

        unsigned char t;

        for(int i = 0; i < file->len; i += 4)
        {
            t = data[i];
            data[i] = data[i + 3];
            data[i + 3] = t;

            t = data[i + 1];
            data[i + 1] = data[i + 2];
            data[i + 2] = t;
        }
    }
}

static
uint getop(uint op)
{
    return op >> 26;
}

static
uint getnbit(uint op, uint index, uint n)
{
    if(n == 32)
        return op >> index;
    return (((uint)1 << n) - 1) & (op >> index);
}
static
uint getlow(uint op)
{
    return op & (uint)0b111111;
}

static 
uint get5(uint op, int index)
{
    return (uint)((op >> index) & 0b11111); 
}

static
uint get3(uint op, int index)
{
    return (uint)((op >> index) & 0b111);
}

static const char* regis[] = {
    "zero", "at", "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "k0", "k1", "gp", "sp",
    "fp", "ra"
};

static
const char* strregis(uint n)
{
    if(0 <= n && n <= 31)
        return regis[n];
    else
        return "idk";
}

static
uint getimm(uint op)
{
    return (uint)((uint)0xFFFF & op);
}

static
uint getlimm(uint op)
{
    return (uint)((uint)0x3FFFFFFF & op);
}



static
void cop0(uint op)
{
    switch(getlow(op))
    {
        case 0b011111:
            printf("deret");
            break;

        case 0b011000:
            printf("deret");
            break;

        default:
            printf("		ni(cop0)");
            break;
    }
}

static
void cop1(uint op)
{
    if(get5(op, 21) == 0b01000)
    {
        uint offset = getimm(op) << 2;
        uint ndtf = (uint)((uint)0b11 & (op >> 16));

        printf("bc1%s%s 0x%x", (ndtf & 0b01 ? "T" : "F"),
                               (ndtf & 0b10 ? "L" : ""),
                               offset);

        //printf("BC not implemented");
    }

    uint fs, fd, ft;
    switch(getlow(op))
    {
        case 0b000101: // ABS
            fd = get5(op, 6);
            fs = get5(op, 11);
            print2("abs", fd, fs, "");
            return;

        case 0b000111: // NEG
            fd = get5(op, 6);
            fs = get5(op, 11);
            print2("neg", fd, fs, "");
            return;

        case 0b001100: // NEG
            fd = get5(op, 6);
            fs = get5(op, 11);
            print2("round.fmt", fd, fs, "");
            return;

        case 0b001101: // NEG
            fd = get5(op, 6);
            fs = get5(op, 11);
            print2("trunc.fmt", fd, fs, "");
            return;

            
        case 0b000000: // ADD
            fs = get5(op, 21);
            ft = get5(op, 16);
            fd = get5(op, 11);
            print3("add.fmt", fd, fs, ft, "");
            return;

        case 0b000001: // ADD
            fs = get5(op, 21);
            ft = get5(op, 16);
            fd = get5(op, 11);
            print3("sub.fmt", fd, fs, ft, "");
            return;

        case 0b010001: // DIV
            fs = get5(op, 21);
            ft = get5(op, 16);
            fd = get5(op, 11);
            print3("div.fmt", fd, fs, ft, "");
            return;

        case 0b000010: // MUL.fmt
            fs = get5(op, 11);
            ft = get5(op, 16);
            fd = get5(op, 6);
            print3("mul.fmt", fd, fs, ft, "");
            return;


        case 0b000110: // MOV
            fs = get5(op, 11);
            fd = get5(op, 6);
            print2("mov.fmt", fd, fs, "");
            return;
        
        case 0b000100: // MOV
            fs = get5(op, 11);
            fd = get5(op, 6);
            print2("sqrt.fmt", fd, fs, "");
            return;


        default:
            printf("		ni(cop1)");
            break;
    }
}

static
void cop2(uint op)
{
    if(get5(op, 21) == 0b01000)
    {
        uint offset = getimm(op) << 2;
        uint ndtf = (uint)((uint)0b11 & (op >> 16));

        printf("bc2%s%s 0x%x", (ndtf & 0b01 ? "T" : "F"),
                               (ndtf & 0b10 ? "L" : ""),
                               offset);

        //printf("BC not implemented");
    }
    else
        printf("		ni(cop2)");


}

static
void special(uint op)
{
    uint rs, rt, rd, cc;
    switch(getlow(op))
    {
        case 0b001101:
            printf("break");
            return;

        case 0b001001:
            rs = get5(op, 21);
            rd = get5(op, 11);
            if(rd == 31)
                print1("jalr", rs, "");
            else
                print2("jalr", rd, rs, "");
            return;

        case 0b110100: // TEQ
            rs = get5(op, 21);
            rd = get5(op, 16);
            print2("teq", rs, rd, "");
            return;
        
        case 0b110110: // TNE
            rs = get5(op, 21);
            rd = get5(op, 16);
            print2("tne", rs, rd, "");
            return;

        case 0b110000: //TGE
            rs = get5(op, 21);
            rd = get5(op, 16);
            print2("tge", rs, rd, "");
            return;
        
        case 0b110001: //TGEU
            rs = get5(op, 21);
            rd = get5(op, 16);
            print2("tgeu", rs, rd, "");
            return;

        case 0b110010: //TLT
            rs = get5(op, 21);
            rd = get5(op, 16);
            print2("tlt", rs, rd, "");
            return;
        
        case 0b110011: //TLTU
            rs = get5(op, 21);
            rd = get5(op, 16);
            print2("tltu", rs, rd, "");
            return;


        case 0b010000:
            rd = get5(op, 11);
            print1("mfhi", rs, "");
            return;

        case 0b001111:
            rd = get5(op, 6);
            print1("sync", rs, "");
            return;

         case 0b001100:
            rd = get5(op, 6);
            print1("syscall", rs, "(syscall exception)");
            return;


        case 0b010010:
            rd = get5(op, 11);
            print1("mflo", rs, "");
            return;

        
        case 0b001000:
            rs = get5(op, 21);
            print1("jr", rs, "");
            return;


        case 0b100000: //add signed 32
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("add", rd, rs, rt, "");
            return;

        case 0b100001: //add unsigned 32
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("addu", rd, rs, rt, "");
            return;

        case 0b011010: //div signed 32
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("div", rd, rs, rt, "");
            return;

        case 0b011000: //mul unsigned 32
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("mul", rd, rs, rt, "");
            return;

        case 0b011001: //mulu signed 32
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("mulu", rd, rs, rt, "");
            return;

        case 0b100010: //SUB unsigned 32
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("sub", rd, rs, rt, "");
            return;

        case 0b100011: //SUBU signed 32
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("subu", rd, rs, rt, "");
            return;



        case 0b011011: //div unsigned 32
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("divu", rd, rs, rt, "");
            return;


        case 0b100100: // and
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("and", rd, rs, rt, "");
            return;

        case 0b100111: // NOR
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("nor", rd, rs, rt, "");
            return;
    
        case 0b100101: // NOR
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("or", rd, rs, rt, "");
            return;

        case 0b100110: // NOR
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("xor", rd, rs, rt, "");
            return;


        case 0b000000: // NOP & SLL
            rs = get5(op, 6);
            rt = get5(op, 16);
            rd = get5(op, 11);
            if(rt == 0 && rd == 0)
            {
                if(rs == 0)
                    printf("NOP");
                else if(rs == 1)
                    printf("SSNOP");
            }
            else
                print3("sll", rd, rs, rt, "");
            return;

        case 0b000100: // SLLV
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("sllv", rd, rt, rs, "rd = rt << rs");
            return;


        case 0b000011: // SRA
            rs = get5(op, 6);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("sra", rd, rs, rt, "");
            return;

        case 0b000111: // SRAV
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("srav", rd, rt, rs, "rd = rt << rs");
            return;

        case 0b000010: // SRL
            rs = get5(op, 6);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("srl", rd, rs, rt, "");
            return;

        case 0b000110: // SRAV
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("srav", rd, rt, rs, "rd = rt << rs");
            return;



        case 0b101011: // SLTU
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("sltu", rd, rs, rt, "");
            return;


            // move
        case 0b001011: // MOVN
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("movn", rd, rs, rt, "");
            return;

        case 0b001010: // MOVZ
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("movz", rd, rs, rt, "");
            return;

        case 0b101010: // SLT
            rs = get5(op, 21);
            rt = get5(op, 16);
            rd = get5(op, 11);
            print3("slt", rd, rs, rt, "");
            return;


        case 0b000001: // MOVCI
            rs = get5(op, 21);
            cc = get3(op, 18);
            rd = get5(op, 11);
            print2cc("movci", rd, rs, cc, "(moveT)");
            return;

        case 0b010011: // MTHI
            rs = get5(op, 21);
            print1("mthi", rs, "");
            return;

        case 0b010001: // MTLO
            rs = get5(op, 21);
            print1("mtlo", rs, "");
            return;


        default:
            printf("		ni(special)");
            return;
    }
}

void special2(uint op)
{
    uint rs, rt, rd;
    switch(getlow(op))
    {
        case 0b000000: // MADD
            rs = get5(op, 21);
            rt = get5(op, 16);
            print2("madd", rs, rt, "");
            return;
        
        case 0b000001: // MADDU
            rs = get5(op, 21);
            rt = get5(op, 16);
            print2("maddu", rs, rt, "");
            return;

        case 0b000100: // MSUB
            rs = get5(op, 21);
            rt = get5(op, 16);
            print2("msub", rs, rt, "");
            return;

        case 0b000101: // MSUBU
            rs = get5(op, 21);
            rt = get5(op, 16);
            print2("msubu", rs, rt, "");
            return;

        case 0b000010: // MUL
            rs = get5(op, 21);
            rt = get5(op, 16);
            print2("mul", rs, rt, "");
            return;

       case 0b111111: // MUL
            rs = get5(op, 21);
            rt = get5(op, 16);
            print2("SDBBP", rs, rt, "");
            return;

        default:
            printf("		ni(special2)");

    }
}

static
void regimm(uint op)
{
    uint imm, rs;

    uint id = get5(op, 16);
    imm = getimm(op);
    if(id == 0b01100) //TGEI
    {
        rs = get5(op, 21);
        print1i("teqi", rs, imm, "");
    }
    else if(id == 0b01110) //TGEI
    {
        rs = get5(op, 21);
        print1i("teqi", rs, imm, "");
    }
    else if(id == 0b01000) //TGEI
    {
        rs = get5(op, 21);
        print1i("tgei", rs, imm, "");
    }
    else if(id == 0b01001) //TGEIu
    {
        rs = get5(op, 21);
        print1i("tgeiu", rs, imm, "");
    }
    else if(id == 0b01010) //TGEI
    {
        rs = get5(op, 21);
        print1i("tlti", rs, imm, "");
    }
    else if(id == 0b01011) //TGEIu
    {
        rs = get5(op, 21);
        print1i("tltiu", rs, imm, "");
    }



    imm <<= 2;
    if(id & 1)
    {
        printf("bgez");
    }
    else
    {
        printf("bltz");
    }

    if(id & 0b10000) //process call
    {
        printf("al");
    }
    else
    {}

    if(id & 0b00010) //likely
    {
        printf("l");
    }
    else {}

    printf(" 0x%x", imm);
}

static
void cache(uint op) 
{
    printf("		ni(cache)");
}

void parse(struct assembler *file)
{
    setup(file);
    
    uint* data = (uint*)file->data;

    size_t len = file->len / 4;

    uint op;

    for(size_t i = 0; i < len; i++)
    {
        op = data[i];
        printf("%08x: ", op);

        uint rs, rt, imm, base, offset;
        switch(getop(op))
        {
            case 0b000000: // SPECIAL
               special(op);
               break;

            case 0b011100:
               special2(op);
               break;

            case 0b010000:
               cop0(op);
               break;

            case 0b010001: // COP1
               cop1(op);
               break;

            case 0b010010: // COP2
               cop2(op);
               break;

            case 0b000001: // REGIMM
               regimm(op);
               break;

            case 0b101111:
               cache(op);
               break;

            // load and store
            

            // ALU OP

            case 0b001000: // ADDI
               rs = get5(op, 21);
               rt = get5(op, 16);
               imm = getimm(op);
               print2i("addi", rt, rs, imm, "");
               break;

            case 0b001001: // ADDIU
               rs = get5(op, 21);
               rt = get5(op, 16);
               imm = getimm(op);
               print2i("addiu", rt, rs, imm, "");
               break;
        
            case 0b001100: // ANDI
               rs = get5(op, 21);
               rt = get5(op, 16);
               imm = getimm(op);
               print2i("andi", rt, rs, imm, "");
               break;

            case 0b001101: // ORI
               rs = get5(op, 21);
               rt = get5(op, 16);
               imm = getimm(op);
               print2i("ori", rt, rs, imm, "");
               break;

            case 0b001110: // XORI
               rs = get5(op, 21);
               rt = get5(op, 16);
               imm = getimm(op);
               print2i("xori", rt, rs, imm, "");
               break;


            // LOAD OP

            case 0b100000: // LB
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("lb", rt, offset, base, "");
                break;
                
            case 0b100100: // LBU
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("lbu", rt, offset, base, "");
                break;
            
            case 0b100001: // lh
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("lh", rt, offset, base, "");
                break;
            
            case 0b100101: // LHU
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("lhu", rt, offset, base, "");
                break;

            case 0b110000: // LL
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("ll", rt, offset, base, "");
                break;

            case 0b001111: // LUI
                rt = get5(op, 16);
                imm = getimm(op);
                print1i("lui", rt, imm, "");
                break;

            case 0b110101: // LDC1
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("ldc1", rt, offset, base, "");
                break;
                
            case 0b110110: // LDC2
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("ldc2", rt, offset, base, "");
                break;

            case 0b100011: // LW
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("lw", rt, offset, base, "");
                break;

            case 0b110001: // LWC1
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("lwc1", rt, offset, base, "");
                break;

            case 0b110010: // LWC2
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("lwc2", rt, offset, base, "");
                break;

            case 0b101011: // SW
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("sw", rt, offset, base, "");
                break;

            case 0b111001: // SWC1
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("swc1", rt, offset, base, "");
                break;

            case 0b111010: // SWC2
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("swc2", rt, offset, base, "");
                break;


            case 0b100010: // LWL
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("lwl", rt, offset, base, "");
                break;

            case 0b100110: // LWR
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("lwr", rt, offset, base, "");
                break;

            case 0b101010: // SWL
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("swl", rt, offset, base, "");
                break;

            case 0b101110: // SWR
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("swr", rt, offset, base, "");
                break;


             case 0b110011: // PREF
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("pref", rt, offset, base, "(hint not parse)"); //TODO
                break;

            case 0b101000: // SB
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("sb", rt, offset, base, "");
                break;
            
            case 0b111000: // SC
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("sc", rt, offset, base, ""); 
                break;

            case 0b111101: // SC
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("sdc1", rt, offset, base, "");
                break;
            
            case 0b111110: // SC
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("sdc2", rt, offset, base, "");
                break;
            
            case 0b101001: // SC
                base = get5(op, 21);
                rt = get5(op, 16);
                offset = getimm(op);
                printoffb("sh", rt, offset, base, "");
                break;


            case 0b001010: // SLTI
                rs = get5(op, 21);
                rt = get5(op, 16);
                imm = getimm(op);
                print2i("slti", rs, rt, imm, "");
                break;
            
            case 0b001011: // SLTIU
                rs = get5(op, 21);
                rt = get5(op, 16);
                imm = getimm(op);
                print2i("sltiu", rs, rt, imm, "");
                break;


            // BRANCH OP

            case 0b000100: // BEQ
                rs = get5(op, 21);
                rt = get5(op, 16);
                imm = getimm(op);
                print2i("beq", rs, rt, imm, "");
                break;

            case 0b010100: // BEQL
                rs = get5(op, 21);
                rt = get5(op, 16);
                imm = getimm(op);
                print2i("beql", rs, rt, imm, "");
                break;
            
            case 0b000101: // BNE
                rs = get5(op, 21);
                rt = get5(op, 16);
                imm = getimm(op);
                print2i("bne", rs, rt, imm, "");
                break;

            case 0b010101: // BEQL
                rs = get5(op, 21);
                rt = get5(op, 16);
                imm = getimm(op);
                print2i("bnel", rs, rt, imm, "");
                break;

            case 0b000111: // BGTZ
                rs = get5(op, 21);
                imm = getimm(op);
                print1i("bgtz", rs, imm, "");
                break;

            case 0b010111: // BGTZL
                rs = get5(op, 21);
                imm = getimm(op);
                print1i("bgtzl", rs, imm, "");
                break;


            case 0b000110: // BLEZ
                rs = get5(op, 21);
                imm = getimm(op);
                print1i("blez", rs, imm, "");
                break;

            case 0b010110: // BLEZL
                rs = get5(op, 21);
                imm = getimm(op);
                print1i("blezl", rs, imm, "");
                break;
            
            case 0b000010: // J
                imm =  getlimm(op);
                print0i("j", imm, "");
                break;

            case 0b000011: // JAL
                imm = getlimm(op);
                print0i("jal", imm, "");
                break;

            default:
                printf("		ni(general)");

        }
        printf("\n");
    }
}

/* Not implemented:
 * 
 * BC1F
 *
 */
