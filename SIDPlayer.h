extern char SIDFILE[];
extern void SIDINIT(); 
extern void SIDPLAY();
extern void STOPSID();
extern void PlaySID(void);
extern void StopSID(void);

#define SIDSIZE (1024)*2
#define SIDLOAD 0xB000