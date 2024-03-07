#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef struct 
{
  uint8_t HV_on:4;
  uint8_t Calib_enable:4;
  uint16_t DACs[4];
  uint8_t Urefcal;
} control_AMP_module_table_t;

typedef struct
{
  uint8_t live_flag;
  uint16_t ADC_HV[4];
  uint16_t Power;
  int16_t temperature;
} measure_AMP_module_table_t;

typedef struct
{
  uint8_t size;
  uint8_t responce_error;
  int8_t errorcode;
  measure_AMP_module_table_t minfo[6];
  uint16_t temperatures[2];//термопары на модуле контроллера предусилителей
} AMP_measure_table_t;

#define ONCE_CALIBRATION	0
#define CYCLE_CALIBRATION	1

typedef struct
{
  control_AMP_module_table_t mctr[6];
  uint8_t do_calibration_pulse;
  uint8_t once_or_cycle_calibration_pulse;//0 - ONCE, 1 - CYCLE
  uint8_t wide_calibration_pulse;
  uint8_t wide_calibration_pulse_scale;
  uint16_t wide_calibration_singlepulse;
} AMP_control_table_t;

typedef struct
{
  uint8_t mode[4];
} control_ADC_module_table_t;

typedef struct
{
  control_ADC_module_table_t mctradc[6];
} ADC_control_table_t;

typedef struct
{
  AMP_control_table_t ctr;
  ADC_control_table_t adcctr;
 
} Maincontroller_control_table_t;

typedef struct 
{
  AMP_control_table_t ctr;
  AMP_measure_table_t meas;
} global_table_t;

typedef struct
{
  uint8_t regs[16];
} AMP_module_state_t;

typedef struct
{
  AMP_module_state_t st[6];
} registers_t;

#ifdef __cplusplus
}
#endif

#pragma pack(0)

#endif
