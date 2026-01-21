/**
 * 
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

void init_heartrate(void);
void heartrate_Task(void *pvParameters);
void init_spo2(void);
void spo2_task(void *pvParameters);


#ifdef __cplusplus
}
#endif

#endif
