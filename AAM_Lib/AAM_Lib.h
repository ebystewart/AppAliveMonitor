#ifndef AAM_LIB_H
#define AAM_LIB_H

#include "AAM_defines.h"


typedef unsigned int checkpointID_t;
typedef unsigned int healthStatusID_t;
typedef unsigned int healthStatus_t;

void AAM_ReportCheckpoint(checkpointID_t id);

void AAM_UpdateCheckpoint(checkpointID_t id);

void AAM_ReportHealthStatus(healthStatusID_t hId, healthStatus_t status);

void AAM_UpdatetHealthStatus(healthStatusID_t hId, healthStatus_t status);

#endif