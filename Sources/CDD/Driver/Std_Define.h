/*
 * StdDefine.h
 *
 *  Created on: 10-Nov-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_DRIVER_STD_DEFINE_H_
#define SOURCES_CDD_DRIVER_STD_DEFINE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef uint8
#define uint8 uint8_t
#endif

#ifndef uint16
#define uint16 uint16_t
#endif

#ifndef uint32
#define uint32 uint32_t
#endif

#ifndef uint64
#define uint64 uint64_t
#endif

#ifndef sint8
#define sint8 int8_t
#endif

#ifndef sint16
#define sint16 int16_t
#endif

#ifndef sint32
#define sint32 int32_t
#endif

#ifndef sint64
#define sint64 int64_t
#endif

#ifndef BOOL
#define BOOL bool
#endif

typedef uint8_t CDDDriverStatus;

#define SATURATION(Var,Minval,Maxval)                                          /* PRQA S 3453 # Function macro is acceptable */ \
(((Var)>=(Maxval)) ? ((Var)=(Maxval)) : \
(((Var)<=(Minval)) ? ((Var)=(Minval)) : ((Var)=(Var))))

#ifndef BASIC
#define BASIC 0U
#endif /* BASIC */

#ifndef ALL
#define ALL 1U
#endif /* ALL */

#ifndef CDD_STANDBY
#define CDD_STANDBY 0U
#endif /* CDD_STANDBY */

#ifndef CDD_INIT
#define CDD_INIT 1U
#endif /* CDD_INIT */

#ifndef CDD_RUN
#define CDD_RUN 2U
#endif /* CDD_RUN */

#ifndef CDD_SHUTDOWN
#define CDD_SHUTDOWN 3U
#endif /* CDD_SHUTDOWN */

#ifndef CDD_FAULTY
#define CDD_FAULTY 4U
#endif /* CDD_FAULTY */

#ifndef FALSE
#define FALSE 0U
#endif /* FALSE */

#ifndef TRUE
#define TRUE 1U
#endif /* TRUE */

#ifndef SET
#define SET 1U
#endif /* SET */

#ifndef RESET
#define RESET 0U
#endif /* RESET */

/* Assert macro does nothing */
#define DEV_ASSERT(x) ((void)0)

#endif /* SOURCES_CDD_DRIVER_STD_DEFINE_H_ */
