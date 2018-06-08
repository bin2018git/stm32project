#ifndef DS1302_H
#define DS1302_H

void InitClock(void);
void ReadDS1302Clock(unsigned char *p);
void WriteDS1302Clock(unsigned char *p);
void ReadDSRam(unsigned char *p,unsigned char add,unsigned char cnt);
void WriteDSRam(unsigned char *p,unsigned char add,unsigned char cnt);
//void TestDS1302(void);

#endif

