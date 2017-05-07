#include"memory.h"
#include<stdlib.h>
#include"instDef.h"
#include<stdio.h>
#include"structs.h"



uint32_t programImage1[MEM_SIZE]={ 
  0x00000bb8,//$sp = 3000
  0x00000bb8,//$fp = 3000
  0x00000000,
  0x00000000,
  0x00000000,
  0x0000008c,//$pc = 140
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x18c00008,   // bleza2,400854 <copy_array+0x24>
  0x00801025,   // movev0,a0
  0x00063080,   // slla2,a2,0x2
  0x00862021,   // addua0,a0,a2
  0x8c430000,   // lwv1,0(v0)
  0xaca30000,   // swv1,0(a1)
  0x24420004,   // addiuv0,v0,4
  0x1444fffc,   // bnev0,a0,400840 <copy_array+0x10>
  0x24a50004,   // addiua1,a1,4
  0x03e00008,   // jrra
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x1ca00018,   // bgtza1,4008c0 <bubble_sort+0x64>
  0x00005825,   // movet3,zero
  0x03e00008,   // jrra
  0x00001025,   // movev0,zero
  0x8c660000,   // lwa2,0(v1)
  0x8c670004,   // lwa3,4(v1)
  0x00e6402a,   // sltt0,a3,a2
  0x11000004,   // beqzt0,40088c <bubble_sort+0x30>
  0x00000000,   // nop
  0xac670000,   // swa3,0(v1)
  0xac660004,   // swa2,4(v1)
  0x240a0001,   // lit2,1
  0x24630004,   // addiuv1,v1,4
  0x1469fff6,   // bnev1,t1,40086c <bubble_sort+0x10>
  0x00000000,   // nop
  0x004c1021,   // adduv0,v0,t4
  0x10000002,   // b4008a8 <bubble_sort+0x4c>
  0x00aa580a,   // movzt3,a1,t2
  0x00a05825,   // movet3,a1
  0x256b0001,   // addiut3,t3,1
  0x0165182a,   // sltv1,t3,a1
  0x14600006,   // bnezv1,4008cc <bubble_sort+0x70>
  0x01ab6023,   // subut4,t5,t3
  0x03e00008,   // jrra
  0x8c000820,//0x00000000,   // nop
  0x00001025,   // movev0,zero
  0x24adffff,   // addiut5,a1,-1
  0x01ab6023,   // subut4,t5,t3
  0x1980fff5,   // blezt4,4008a4 <bubble_sort+0x48>
  0x00801825,   // movev1,a0
  0x000c4880,   // sllt1,t4,0x2
  0x00894821,   // addut1,a0,t10x8c190812,//flush memory location for testing
  0x1000ffe3,   // b40086c <bubble_sort+0x10>
  0x00005025,   // movet2,zero
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x28a20002,   // sltiv0,a1,2
  0x14400029,   // bnezv0,400990 <insertion_sort+0xac>
  0x248e0004,   // addiut6,a0,4
  0x0080c025,   // movet8,a0
  0x24190004,   // lit9,4
  0x00001025,   // movev0,zero
  0x10000016,   // b400958 <insertion_sort+0x74>
  0x240f0001,   // lit7,1
  0xac690000,   // swt1,0(v1)
  0x24420001,   // addiuv0,v0,1
  0x104d000c,   // beqv0,t5,400940 <insertion_sort+0x5c>
  0xacc80000,   // swt0,0(a2)
  0x006c1821,   // adduv1,v1,t4
  0x00831821,   // adduv1,a0,v1
  0x8ce8fffc,   // lwt0,-4(a3)
  0x00cb3021,   // addua2,a2,t3
  0x24e7fffc,   // addiua3,a3,-4
  0x8ce9fffc,   // lwt1,-4(a3)
  0x0109502a,   // sltt2,t0,t1
  0x1540fff4,   // bnezt2,400904 <insertion_sort+0x20>
  0x00863021,   // addua2,a0,a2
  0x10000003,   // b400948 <insertion_sort+0x64>
  0x25ef0001,   // addiut7,t7,1
  0x01a01025,   // movev0,t5
  0x25ef0001,   // addiut7,t7,1
  0x25ce0004,   // addiut6,t6,4
  0x27180004,   // addiut8,t8,4
  0x10af0010,   // beqa1,t7,400994 <insertion_sort+0xb0>
  0x27390004,   // addiut9,t9,4
  0x19e0fffa,   // blezt7,400944 <insertion_sort+0x60>
  0x01c01825,   // movev1,t6
  0x8dc80000,   // lwt0,0(t6)
  0x8f090000,   // lwt1,0(t8)
  0x0109382a,   // slta3,t0,t1
  0x10e0fff5,   // beqza3,400944 <insertion_sort+0x60>
  0x03003025,   // movea2,t8
  0x004f6821,   // addut5,v0,t7
  0x01c03825,   // movea3,t6
  0x272cfffc,   // addiut4,t9,-4
  0x018e6023,   // subut4,t4,t6
  0x272bfff8,   // addiut3,t9,-8
  0x1000ffde,   // b400904 <insertion_sort+0x20>
  0x01785823,   // subut3,t3,t8
  0x00001025,   // movev0,zero
  0x03e00008,   // jrra
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x18c0000e,   // bleza2,4009dc <check_result+0x3c>
  0x00801825,   // movev1,a0
  0x00063080,   // slla2,a2,0x2
  0x00862021,   // addua0,a0,a2
  0x00001025,   // movev0,zero
  0x24060001,   // lia2,1
  0x8c670000,   // lwa3,0(v1)
  0x8ca80000,   // lwt0,0(a1)
  0x00e83826,   // xora3,a3,t0
  0x00c7100b,   // movnv0,a2,a3
  0x24630004,   // addiuv1,v1,4
  0x1464fffa,   // bnev1,a0,4009b8 <check_result+0x18>
  0x24a50004,   // addiua1,a1,4
  0x03e00008,   // jrra
  0x00000000,   // nop
  0x03e00008,   // jrra
  0x00001025,   // movev0,zero
  0x00000000,   // nop
  0x00000000,   // nop
  0x00000000,   // nop
  0x27bdf7f8,   // addiusp,sp,-2056
  0xafbf0804,   // swra,2052(sp)
  0xafb30800,   // sws3,2048(sp)
  0xafb207fc,   // sws2,2044(sp)
  0xafb107f8,   // sws1,2040(sp)
  0xafb007f4,   // sws0,2036(sp)
  0x8c420000,   // lwv0,0(v0)
  0xafa207ec,   // swv0,2028(sp)
  0x240600fa,   // lia2,250
  0x27b10404,   // addius1,sp,1028
  0x02202825,   // movea1,s1
  0x27b0001c,   // addius0,sp,28
  0x02002025,   // movea0,s0
  0x0c00000a,   // jal10 <copy_array>
  0x00000000,   // nop
  0x240500fa,   // lia1,250
  0x02002025,   // movea0,s0
  0x0c00001e,   // jal30 <bubble_sort>
  0x00000000,   // nop
  0x00409025,   // moves2,v0
  0x240500fa,   // lia1,250
  0x02202025,   // movea0,s1
  0x0c000046,   // jal 70 <insertion_sort>
  0x00000000,   // nop
  0x00409825,   // moves3,v0
  0x240600fa,   // lia2,250
  0x02202825,   // movea1,s1
  0x02002025,   // movea0,s0
  0x0c000078,   // jal120 <check_result>
  0x00000000,   // nop
  0x0240a020,  // move $s32 to $s4 to be stored into result memory location
  0x0272902a,   // slts2,s3,s2
  0x16400003,   // bnezs2,400a90 <main+0xac>
  0x3c03000f,   // luiv1,0xf
  0x24634240,   // addiuv1,v1,16960
  0x00431021,   // adduv0,v0,v1
  0xac14001c,  //sw $s4, 28($zero)store number of passes through bubble sort
  0xac130020,  //sw $s3, 32($zero) store number of passes through insertion sort
  0xac020024,  //sw $v0, 36($zero)if 0, the two arrays are equal, 1 they are not equal
  0x8c0804b0,  //lw $t0, 1200($zero)Using the data in memory location 300 to verify sort
  0xac080018,  //sw $t0, 24($zero)
  0x8c001018,  //flush cache to memory to view via memory
  0x8c00101c,  //flush cache to memory to view via memory
  0x8c001020,  //flush cache to memory to view via memory
  0x8c001024,  //flush cache to memory to view via memory
  0x00000000,  //nop
  0x00000000,  //nop
  0x00000000,  //nop
  0x00000000,  //nop
  0x00000008,   // jrto PC = 0, indicating end of program
  0x00000000,  //nop
  0x00000000,  //nop
  0x00000000,  //nop
  0x00000000,  //nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x0a000000,
  0x00000d00,    // nop
  0x00b00000,    // nop
  0x000000e0,
  0x00030000,    // nop
  0x0f000000,    // nop
  0x0000f000,
  0xffffffff,
  0x0a0a0000,
  0x0f0f0f0f,
  0xf0f0f0f0,
  0x00010020,
  0xf0000000,
  0x0aa0bab0,
  0xa5a5a5a5,
  0x5a5a5a5a,
  0x0acccbaa,
  0x00000000,
  0xffffffff,
  0xf034a04a,
  0xaaaaaaaa,
  0xbbbbbbbb,
  0xcccccccc,
  0xdddddddd,
  0xeeeeeeee,
  0x99999999,
  0x88888888,
  0x77777777,
  0x66666666,
  0x55555555,
  0x44444444,
  0x33333333,
  0x22222222,
  0x11111111,
  0x00000001,
  0x00000010,
  0x00000100,
  0x00001000,
  0x00010000,
  0x00100000,
  0x01000000,
  0x10000000,
  0x00000002,
  0x00000020,
  0x00000200,
  0x00002000,
  0x00020000,
  0x00200000,
  0x02000000,
  0x20000000,
  0x00000003,
  0x00000030,
  0x00000300,
  0x00003000,
  0x00030000,
  0x00300000,
  0x03000000,
  0x30000000,
  0x00000004,
  0x00000040,
  0x00000400,
  0x00004000,
  0x00040000,
  0x00400000,
  0x04000000,
  0x40000000,
  0x00000005,
  0x00000050,
  0x00000500,
  0x00005000,
  0x00050000,
  0x00500000,
  0x05000000,
  0x50000000,
  0x00000006,
  0x00000060,
  0x00000600,
  0x00006000,
  0x00060000,
  0x00600000,
  0x06000000,
  0x60000000,
  0x00000007,
  0x00000070,
  0x00000700,
  0x00007000,
  0x00070000,
  0x00700000,
  0x07000000,
  0x70000000,
  0x00000008,
  0x00000080,
  0x00000800,
  0x00008000,
  0x00080000,
  0x00800000,
  0x08000000,
  0x80000000,
  0x00000009,
  0x00000090,
  0x00000900,
  0x00009000,
  0x00090000,
  0x00900000,
  0x09000000,
  0x90000000,
  0x0000000a,
  0x000000a0,
  0x00000a00,
  0x0000a000,
  0x000a0000,
  0x00a00000,
  0x0a000000,
  0xa0000000,
  0x0000000b,
  0x000000b0,
  0x00000b00,
  0x0000b000,
  0x000b0000,
  0x00b00000,
  0x0b000000,
  0xb0000000,
  0x0000000c,
  0x000000c0,
  0x00000c00,
  0x0000c000,
  0x000c0000,
  0x00c00000,
  0x0c000000,
  0xc0000000,
  0x0000000d,
  0x000000d0,
  0x00000d00,
  0x0000d000,
  0x000d0000,
  0x00d00000,
  0x0d000000,
  0xd0000000,
  0x0000000e,
  0x000000e0,
  0x00000e00,
  0x0000e000,
  0x000e0000,
  0x00e00000,
  0x0e000000,
  0xe0000000,
  0x0000000f,
  0x000000f0,
  0x00000f00,
  0x0000f000,
  0x000f0000,
  0x00f00000,
  0x0f000000,
  0xf0000000,
  0x00000011,
  0x00001100,
  0x00110000,
  0x22000000,
  0x00000022,
  0x00002200,
  0x00220000,
  0x22000000,
  0x00000033,
  0x00003300,
  0x00330000,
  0x33000000,
  0x00000044,
  0x00004400,
  0x00440000,
  0x44000000,
  0x00000055,
  0x00005500,
  0x00550000,
  0x55000000,
  0x00000066,
  0x00006600,
  0x00660000,
  0x66000000,
  0x00000077,
  0x00007700,
  0x00770000,
  0x77000000,
  0x00000088,
  0x00008800,
  0x00880000,
  0x88000000,
  0x00000099,
  0x00009900,
  0x00990000,
  0x99000000,
  0x000000aa,
  0x0000aa00,
  0x00aa0000,
  0xaa000000,
  0x000000bb,
  0x0000bb00,
  0x00bb0000,
  0xbb000000,
  0x000000cc,
  0x0000cc00,
  0x00cc0000,
  0xcc000000,
  0x000000dd,
  0x0000dd00,
  0x00dd0000,
  0xdd000000,
  0x000000ee,
  0x0000ee00,
  0x00ee0000,
  0xee000000,
  0x000000ff,
  0x0000ff00,
  0x00ff0000,
  0xff000000,
  0x00000101,
  0x00001010,
  0x00010100,
  0x00101000,
  0x01010000,
  0x30300000,
  0x00000303,
  0x00003030,
  0x00030300,
  0x00303000,
  0x03030000,
  0x30300000,
  0x00000202,
  0x00002020,
  0x00020200,
  0x00202000,
  0x02020000,
  0x20200000,
  0x00000505,
  0x00005050,
  0x00050500,
  0x00505000,
  0x05050000,
  0x50500000,
  0x00000606,
  0x00006060,
  0x00060600,
  0x00606000,
  0x06060000,
  0x60600000,
  0x00000f0f,
  0x0000f0f0,
  0x000f0f00,
  0x00f0f000,
  0x0f0f0000,
  0x0000e000
};
uint32_t makeRtype(uint8_t inst, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct){
    uint32_t machInst = (inst<<26)|(rs<<21)|(rt<<16)|(rd<<11)|(shamt<<6)|funct;
      return machInst;
}
uint32_t makeItype(uint8_t inst, uint8_t rs, uint8_t rt, uint16_t immed){
    uint32_t machInst = (inst<<26)|(rs<<21)|(rt<<16)|immed;
      return machInst;
}

void initMemory(){
    iMem = malloc(MEM_SIZE*sizeof(uint32_t));
    //dMem = malloc(MEM_SIZE*sizeof(uint32_t));

//    iMem[5] = 0x6;
//    iMem[6] = 0x00000000; //makeRtype(0, $s0, $s1, $t0, 0, SLT);
//    iMem[7] = makeItype(ADDI, $v0, $v1, 0xffc4);
//    iMem[8] = makeItype(ADDIU, $v0, $v1, 0xffc4);

    int i;
    for( i = 0; i < MEM_SIZE; i++){
       iMem[i] = programImage1[i];
    }

}

void freeMemory(){
    free(iMem);
    free(dMem);
}
