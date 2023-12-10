// $Id: VDefaults.h

#ifndef VDEFAULTS_H
#define VDEFAULTS_H

/* hard limits for VOLT memory pooling, the max amount of memory pointers that
can be stored in a pool. */
#define VOLT_DEFAULT_HARD_LIMIT 1000000

/* VOLT memory increment*/
#define VOLT_DEFAULT_INCREMENT 50

/* VOLT VM specs*/
#define VOLT_VM_VERSION "0.0.1"
#define VOLT_VM_MAXREGISTERS 512

/* OpenLUD OBI specs */
#define VOLT_OPENLUD_BYTE_LIMIT 256
#define VOLT_OPENLUD_MAXREGISTERS 256

/* NexFUSE specs */
#define VOLT_NEXFUSE_MAXREGISTERS VOLT_OPENLUD_MAXREGISTERS
#define VOLT_NEXFUSE_BYTE_LIMIT VOLT_OPENLUD_BYTE_LIMIT

#endif // VDEFAULTS_H
