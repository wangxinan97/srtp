#ifdef SOFTWAREDEBUG
	IO0PIN = 0nxffffffff;
	SaveParameterToFlash();
#endif

#define int32 long int
#define uint8 unsigned short int

int simulation();