#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include "bad_file.h"
#include <linux/slab.h>
#include <linux/types.h>


MODULE_AUTHOR("Saidgani Musaev <TUD SE/chair>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Just small benchmark code");


static uint64_t rdmsr0(unsigned int regno){
    unsigned int a, d;
    uint64_t res;
    asm volatile("\nrdmsr\n" : "=a"(a), "=d"(d): "c"(regno) );
    res = (uint64_t)a | ((uint64_t)d<<32);
    return res;
}

static inline uint64_t rdpmc0(unsigned int counter){
    unsigned int a, d;
    uint64_t res;
    asm volatile("\trdpmc\n" : "=a"(a), "=d"(d): "c"(counter) );
    res = (uint64_t)a | ((uint64_t)d<<32);
    return res;
}

static inline void wrmsr0(unsigned int msr, unsigned int low, unsigned int high){
    asm volatile("wrmsr\n"::"c"(msr),"a"(low),"d"(high):"memory");
}

static inline unsigned long rdpmc_actual_cycles(void)
{
	   unsigned a, d, c;

	      c = (1<<30)+1;
	         __asm__ volatile("rdpmc" : "=a" (a), "=d" (d) : "c" (c));

		    return ((unsigned long)a) | (((unsigned long)d) << 32);;
}


int __init __attribute((optimize("O0")))my_init(void)
{
    volatile unsigned int msr;
    volatile unsigned int low, high;
    uint64_t rd, rd2;
    int i;
    volatile int x1, x2;
    uint64_t sb[512];
    uint64_t minval = 0xffffffffffffffff;
    unsigned long long  exp;
    i = 0;
    get_cpu();


    // asm volatile("mfence");
    // buff[0] = 0x0;
    // asm volatile("mfence");
    for(exp = 0; exp < 10000000; ++exp){
        msr = 0x187;
        low = 0x4201A2; // 0x420e01 | (0x1<<23) | (0x1<<24)
        high = 0x0;
        wrmsr0(msr, 0, 0);
        wrmsr0(0xC2, 0, 0);
        wrmsr0(msr, low, high);    
        asm volatile("mfence");
        rd = rdmsr0(0x30a);
        // #pragma GCC unroll 5
        for(i = 0; i < 50;){
            // msr = i+100;
            sb[i] = i*4+100;
            // asm volatile("mfence");
            sb[i+1] = i*4+100;

            i = i+1;
        }        
            // asm volatile("mfence");
            rd2 = rdmsr0(0x30a);
            
            // rd = 0;
        if(minval > (rd2-rd)) minval = rd2-rd;
    }
        // msr;
        // msr;
        // msr;
        // msr;
        // msr;
        // msr;
        // msr;
        // msr;
        // msr;
        // asm volatile("sfence");
        // *(&x2) = i;
       

        // asm volatile("sfence");
        // *(&x2) = i;
       

        // asm volatile("sfence");
        // *(&x2) = i;
        
    // }
    // buff[1] = 0x1;

    //asm volatile("mfence");

    // rd2 = rdpmc_actual_cycles(); //rdpmc0(1<<30);
    // asm volatile("mfence");
    msr = 0x187;
    printk(KERN_ALERT "SAID:::val: %lu\n", minval);
    wrmsr0(msr, 0, 0);
    wrmsr0(0xC2, 0, 0);
    put_cpu();
    return  -1;
}



// static int my_init(void)
// {
//     unsigned int msr;
//     u64 low, high;
//     int i;
//     msr = 0x187;
//     low = 0x4201A2;
//     high = 0x0;
//     get_cpu();
//     asm volatile("1: wrmsr\n"
//             "2:\n"
//             : : "c" (msr), "a"(low), "d" (high) : "memory");

//     asm volatile("sfence");
//     for(i = 0; i < 512; ++i)
//         msr = i;
//     asm volatile("sfence");
//     msr = 0xC2;
    
//     asm volatile("1: rdmsr\n"
//             "2:\n"
//             : "=a" (low), "=d" (high) : "c" (msr)); 

//     printk("SAID:::val: %lu\n", (low) | ((high) << 32));
//     put_cpu();
//     return  -1;
// }



void __exit cleanup(void){

}

module_init(my_init);
module_exit(cleanup);
