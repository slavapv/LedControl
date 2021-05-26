#define MARK_EXCESS_MICROS 20
#define DECODE_NEC
#define IR_RECEIVE_PIN 2
#if !defined (FLASHEND)
#define FLASHEND 0xFFFF // Dummy value for platforms where FLASHEND is not defined
#endif
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)