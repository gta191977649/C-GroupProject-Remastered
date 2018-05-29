#define ONETWOEIGHT (0x80)
#define SIXFOUR (0x40)
#define THREETWO (0x20)
#define ONWSIX (0x10)
#define EIGHT (0x08)
#define FOUR (0x04)
#define TWO (0x02)
#define ONE (0x01)

static const unsigned char mask[8] = {
      ONETWOEIGHT,
      SIXFOUR,
      THREETWO,
      ONWSIX,
      EIGHT,
      FOUR,
      TWO,
      ONE
};

/*FileCompress Struct*/
#define BITS_PER_CHAR 8     /*the number of bits in a char*/
#define MAX_CHARS 256            /*the max number of chars*/
#define FILE_BUF_SIZE 8192  /*the size of Buffer for FILE I/O*/