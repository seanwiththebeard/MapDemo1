extern char SIDFILE[];
extern void SIDINIT(); 
extern void SIDPLAY();
extern void STOPSID();
void PlaySID(void);
void StopSID(void);

#define SIDSIZE (1024)*2
#define SIDLOAD 0xB000