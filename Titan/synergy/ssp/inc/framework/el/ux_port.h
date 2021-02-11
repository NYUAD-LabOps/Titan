/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2019 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */ 
/*                                                                        */ 
/*    ux_port.h                                            Synergy        */ 
/*                                                           5.0          */ 
/*                                                                        */
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    TCRG, Express Logic, Inc.                                           */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file contains data type definitions that make USBX function    */ 
/*    identically on a variety of different processor architectures.      */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  09-25-2015     TCRG                     Initial Synergy Support       */
/*                                            Version 5.0                 */
/*                                                                        */ 
/**************************************************************************/ 

#ifndef UX_PORT_H
#define UX_PORT_H


/* Determine if the optional USBX user define file should be used.  */

#define UX_INCLUDE_USER_DEFINE_FILE
#ifdef UX_INCLUDE_USER_DEFINE_FILE


/* Yes, include the user defines in ux_user.h. The defines in this file may 
   alternately be defined on the command line.  */

#include "ux_user.h"
#endif


/* Include library header files.  */

#include <stdio.h>
#include <string.h>


/* CPU definition for X86 systems without preemptive timer function.
   This will make USBX uses the controller for the timer. */

#undef THREADX_X86_NO_PTIMER


/* For X86 systems, the define #define UX_USE_IO_INSTRUCTIONS should be used.  */


/* Define additional generic USBX types.  */

typedef long                        SLONG;


/*  Generic USBX Project constants follow.  */

#define UX_DISABLE_INCLUDE_SOURCE_CODE

#ifdef TX_ENABLE_EVENT_TRACE
#ifndef UX_TRACE_INSERT_MACROS
#define UX_TRACE_INSERT_MACROS
#endif
#endif

#if defined(BSP_MCU_GROUP_S124) || defined(BSP_MCU_GROUP_S128) || defined(BSP_MCU_GROUP_S1JA)
/* Apply Device Only configuration for small foot print Synergy parts to reduce memory size used by USBX. */
#ifndef UX_SYSTEM_DEVICE_ONLY
#define UX_SYSTEM_DEVICE_ONLY
#endif
#ifndef UX_DEVICE_SIDE_ONLY
#define UX_DEVICE_SIDE_ONLY
#endif
#endif

/* Defines the number of ThreadX timer ticks per seconds. By default, the ThreadX timer tick is 10ms,
   so the default value for this constant is 100.  If TX_TIMER_TICKS_PER_SECOND is defined,
   the value for UX_PREIODIC_RATE will be derived from TX_TIMER_TICKS_PER_SECOND.  */

#ifndef UX_PERIODIC_RATE
#ifdef TX_TIMER_TICKS_PER_SECOND
#define UX_PERIODIC_RATE                                    (TX_TIMER_TICKS_PER_SECOND)
#else
#define UX_PERIODIC_RATE                                    100
#endif
#endif


#ifndef UX_MAX_CLASS_DRIVER
#define UX_MAX_CLASS_DRIVER                                 8
#endif

#ifndef UX_MAX_SLAVE_CLASS_DRIVER
#define UX_MAX_SLAVE_CLASS_DRIVER                           3
#endif

#ifndef UX_MAX_HCD
#define UX_MAX_HCD                                          2
#endif

#ifndef UX_MAX_DEVICES
#define UX_MAX_DEVICES                                      8
#endif

#ifndef UX_MAX_ED
#define UX_MAX_ED                                           80
#endif

#ifndef UX_MAX_TD
#define UX_MAX_TD                                           128
#endif

#ifndef UX_MAX_ISO_TD
#define UX_MAX_ISO_TD                                       128
#endif

#ifndef UX_THREAD_STACK_SIZE
#ifdef UX_SYSTEM_DEVICE_ONLY
#define UX_THREAD_STACK_SIZE                                512
#else
#define UX_THREAD_STACK_SIZE                                1024
#endif
#endif

#ifndef UX_THREAD_PRIORITY_ENUM
#define UX_THREAD_PRIORITY_ENUM                             20
#endif

#ifndef UX_THREAD_PRIORITY_CLASS
#define UX_THREAD_PRIORITY_CLASS                            20
#endif

#ifndef UX_THREAD_PRIORITY_KEYBOARD
#define UX_THREAD_PRIORITY_KEYBOARD                         20
#endif

#ifndef UX_THREAD_PRIORITY_HCD
#define UX_THREAD_PRIORITY_HCD                              2
#endif

#ifndef UX_THREAD_PRIORITY_DCD
#define UX_THREAD_PRIORITY_DCD                              2
#endif

#ifndef UX_NO_TIME_SLICE
#define UX_NO_TIME_SLICE                                    0
#endif

#ifndef UX_MAX_SLAVE_LUN
#define UX_MAX_SLAVE_LUN                                    2
#endif

#ifndef UX_MAX_HOST_LUN
#define UX_MAX_HOST_LUN                                     16
#endif


#ifndef UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH
#define UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH                 256
#endif

#ifndef UX_SLAVE_REQUEST_DATA_MAX_LENGTH
#ifdef UX_SYSTEM_DEVICE_ONLY
#define UX_SLAVE_REQUEST_DATA_MAX_LENGTH                    512
#else
#define UX_SLAVE_REQUEST_DATA_MAX_LENGTH                    4096
#endif
#endif

#ifndef UX_USE_IO_INSTRUCTIONS

/* Don't use IO instructions if this define is not set.  Default to memory mapped.  */

#define inpb(a)                                            *((UCHAR *)  (a))
#define inpw(a)                                            *((USHORT *) (a))
#define inpl(a)                                            *((ULONG *)  (a))
#define outpb(a, b)                                        *((UCHAR *)  (a)) =  ((UCHAR)  (b))
#define outpw(a, b)                                        *((USHORT *) (a)) =  ((USHORT) (b))
#define outpl(a, b)                                        *((ULONG *)  (a)) =  ((ULONG)  (b))
#else


/* Define simple prototypes for non-memory mapped hardware access.  */

UCHAR   inpb(ULONG);
USHORT  inpw(ULONG);
ULONG   inpl(ULONG);

VOID    outpb(ULONG,UCHAR);
VOID    outpw(ULONG,USHORT);
VOID    outpl(ULONG,ULONG);

#endif


/* Define interrupt lockout constructs to protect the memory allocation/release which could happen
   under ISR in the device stack.  */

#define UX_INT_SAVE_AREA        unsigned int  old_interrupt_posture;
#define UX_DISABLE_INTS         old_interrupt_posture =  tx_interrupt_control(TX_INT_DISABLE);
#define UX_RESTORE_INTS         tx_interrupt_control(old_interrupt_posture);


/* Define the version ID of USBX.  This may be utilized by the application.  */

#ifdef  UX_SYSTEM_INIT
CHAR                            _ux_version_id[] = 
                                    "Copyright (c) 1996-2019 Express Logic Inc. * USBX Synergy Version G5.9SP1.5.0 SN: 4154-280-5000 *";
#else
extern  CHAR                    _ux_version_id[];
#endif

#endif

