/*
 * Helper for doing common tasks with I2C IMUs (and maybe other devices).
 * Copyright (C) 2017  Sakari Kapanen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "imu_i2c_helper.h"
#include "espway_config.h"

static uint16_t imu_i2c_freq = 0;
static int imu_i2c_scl = 5;
static int imu_i2c_sda = 0;

int imu_send_config(uint8_t i2c_address, const imu_register_value_t *config,
                    size_t n_register_values)
{
  for (size_t i = 0; i < n_register_values; ++i)
  {
    int ret = imu_write_register(i2c_address, config[i].address,
                                 config[i].value);
    if (ret != 0) return ret;
  }
  return 0;
}

int imu_write_register(uint8_t addr, uint8_t reg, uint8_t value)
{
  uint8_t buf[] = { reg, value };
  brzo_i2c_start_transaction(addr, imu_i2c_freq);
  brzo_i2c_write(buf, 2, false);
  return brzo_i2c_end_transaction();
}

int imu_read_register(uint8_t addr, uint8_t reg, uint8_t *data)
{
  return imu_read_registers(addr, reg, data, 1);
}

int imu_read_registers(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len)
{
  brzo_i2c_start_transaction(addr, imu_i2c_freq);
  brzo_i2c_write(&reg, 1, true);
  brzo_i2c_read(data, len, false);
  return brzo_i2c_end_transaction();
}

void imu_i2c_init(uint16_t freq)
{
  imu_i2c_freq = freq;
  brzo_i2c_setup(imu_i2c_sda, imu_i2c_scl, 2000);
}

void imu_i2c_configure(int scl, int sda)
{
  imu_i2c_scl = scl;
  imu_i2c_sda = sda;
}

