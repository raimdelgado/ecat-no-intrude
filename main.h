/*
 *  This file is owned by the Embedded Systems Laboratory of Seoul National University of Science and Technology
 *  to test EtherCAT master protocol using the IgH EtherCAT master userspace library.	
 *  
 *  
 *  IgH EtherCAT master library for Linux is found at the following URL: 
 *  <http://www.etherlab.org/en/ethercat>
 *
 *
 *
 *
 *  2015 Raimarius Delgado
*/
/****************************************************************************/
/* main.h */
/****************************************************************************/
#include <embdECATM.h> //./libs/ecatservo 
#include <embdCOMMON.h>  //./libs/embedded
#include <embdMATH.h>  //./libs/embedded
/*****************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <malloc.h>
#include <pthread.h>
/*****************************************************************************/
/* Xenomai */
/*****************************************************************************/
#include <native/task.h>
#include <native/timer.h>
#include <rtdk.h> //rt_printf
/*****************************************************************************/
/* Real-time Task */
/*****************************************************************************/
#define ECATCTRL_TASK_MODE	T_FPU|T_CPU(0)
#define ECATCTRL_TASK_PRIORITY	(99) // xeno: 99 , preempt: 80
#define ECATCTRL_TASK_PERIOD	(1000000L)

RT_TASK TskEcatCtrl;
RTIME 	RtmEcatMasterAppTime;

#define MEASURE_TIMING //enable for timing analysis
#ifdef MEASURE_TIMING
RTIME RtmEcatPeriodStart=0, RtmEcatPeriodEnd=0, RtmEcatExecTime=0; 
int EcatPeriod=0, EcatExecution=0, EcatJitter=0;
int temp_EcatPeriod=0, temp_EcatExecution=0, temp_EcatJitter=0;
#endif

/*****************************************************************************/
/* global variables */
/*****************************************************************************/
#define	NANOSEC_PER_SEC		(1000000000L)
#define FREQ_PER_SEC(x)		(NANOSEC_PER_SEC/(x))
#define VEL_BUFF		(15000)
typedef enum{
	READY = 0,
	RUN,
	STOP,
}SERVO_OP;

/* ECAT_STATE: EtherCAT State Machine (libs/ecatservo/embdECATM.h)
 * unsigned int  master_state;
 * unsigned int	 slave_state;
 * unsigned int	 slave_number */
ECAT_STATE	EcatState;
SERVO_OP	sanyoServoOp = READY;

int		quitFlag	= 0;
int		timingFlag	= 0;
int		iSlavePos	= 0;

int sanyoStatusWord[SANYODENKI_SLAVENUM] = {0,};
int sanyoPrevStatusWord[SANYODENKI_SLAVENUM] = {0,};
int sanyoActualVelR[SANYODENKI_SLAVENUM] = {0,};
/*****************************************************************************/
#ifdef MEASURE_TIMING
#define PERF_EVAL //enable to analyze EtherCAT Master Performance
#ifdef PERF_EVAL
#define BUF_SIZE		(60*60*12) //1 minute data for 1ms Cyclic Task
/* MATH_STATS: Simple Statistical Analysis (libs/embedded/embdMATH.h)
 * 	float ave;
 * 	float max;
 * 	float min;
 * 	float std; */
MATH_STATS EcatPeriodStat, EcatExecStat, EcatJitterStat;
int  iBufEcatDataCnt = 0;
int BufEcatPeriodTime[BUF_SIZE] = {0,}; 
int BufEcatExecTime[BUF_SIZE] 	= {0,};
int BufEcatJitter[BUF_SIZE]	= {0,};

#define PRINT_TIMING //enable to print results
#ifdef PRINT_TIMING
FILE *FileEcatTiming;
#endif //PRINT_TIMING
#endif //PERFEVAL
#endif //MEASURE_TIMING















