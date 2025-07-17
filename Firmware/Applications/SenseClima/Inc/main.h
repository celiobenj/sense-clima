/*

           _           _       _  _    ___  
          | |         | |     | || |  / _ \ 
          | |     __ _| |__   | || |_| | | |
          | |    / _` | '_ \  |__   _| | | |
          | |___| (_| | |_) |    | |_| |_| |
          |______\__,_|_.__/     |_(_)\___/  
 =================== Lab 4.0 =====================

 Copyright (c) 2025 Laboratório 4.0 - Projeto Educacional
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

*/
#pragma once

#include "htnb32lxxx_hal_usart.h"
#include "bsp.h"
#include "HT_BSP_Custom.h"
#include "osasys.h"
#include "ostask.h"
#include "queue.h"
#include "ps_event_callback.h"
#include "cmisim.h"
#include "cmimm.h"
#include "cmips.h"
#include "sockets.h"
#include "psifevent.h"
#include "ps_lib_api.h"
#include "lwip/netdb.h"
#include "debug_log.h"
#include "slpman_qcx212.h"
#include "MQTTClient.h"
#include "plat_config.h"
#include "debug_trace.h"
#include "hal_uart.h"
#include "flash_qcx212.h"
#include "flash_qcx212_rt.h"
#include "slpman_qcx212.h"
#include "FreeRTOS.h"
#include "netmgr.h"
#include <stdio.h>
#include "cmsis_os2.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "slpman_qcx212.h"
#include "pad_qcx212.h"
#include "HT_gpio_qcx212.h"
#include "ic_qcx212.h"
#include "HT_ic_qcx212.h"
#include "hal_adc.h"
#include "HT_adc_qcx212.h"
#include <stdio.h>
#include "string.h"
#include "adc_qcx212.h"
#include "HT_bsp.h"
#include "stdint.h"
#include <stdio.h>
#include "bsp.h"
#include "main.h"
#include "pad_qcx212.h"
#include "HT_gpio_qcx212.h"
#include "ic_qcx212.h"
#include "HT_ic_qcx212.h"
#include "htnb32lxxx_hal_usart.h"
#include "HT_bsp.h"
#include <stdio.h>
#include <stdbool.h> 
#include "htnb32lxxx_hal_usart.h"
#include "slpman_qcx212.h"
#include "HT_Peripheral_Config.h"
#include "HT_usart_unilog.h"