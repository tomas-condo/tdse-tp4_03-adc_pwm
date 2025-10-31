/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file   : task_pwm.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include <stdlib.h>
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"

/********************** macros and definitions *******************************/


/********************** internal data declaration ****************************/


/********************** internal functions declaration ***********************/
void setPWM(TIM_HandleTypeDef *timer, uint32_t canal, uint16_t valor);

/********************** external data declaration *****************************/
extern TIM_HandleTypeDef htim3;

/********************** internal data definition *****************************/

const char *p_task_pwm = "Task PWM";
/********************** external functions definition ************************/


void task_pwm_init(void *parameters)
{
	shared_data_type *shared_data = (shared_data_type *) parameters;
	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_pwm_init), p_task_pwm);

	shared_data->pwm_active = 0;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void task_pwm_update(void *parameters)
{
	shared_data_type *shared_data = (shared_data_type *) parameters;

	if (shared_data->adc_end_of_conversion)
	{
		shared_data->adc_end_of_conversion = false;

		// Escalado del valor ADC (0–4095) al rango PWM
		uint32_t period = __HAL_TIM_GET_AUTORELOAD(&htim3);
		uint32_t pulse = (shared_data->adc_value * period) / 4095;

		setPWM(&htim3, TIM_CHANNEL_1, pulse);
		shared_data->pwm_active = pulse;
	}
}

void setPWM(TIM_HandleTypeDef *timer, uint32_t channel, uint16_t valor)
{
	__HAL_TIM_SET_COMPARE(timer, channel, valor);
}
/********************** end of file ******************************************/
