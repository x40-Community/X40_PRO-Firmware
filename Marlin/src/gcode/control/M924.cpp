/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 *
 */

#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "../../wtlib/WTCMD.h"

#if MOTHERBOARD == BOARD_WEEDO_73B
#include "../gcode.h"

/**
 * M924: Weedo X40 change-over to extruder
 *   
 *            
 */
void GcodeSuite::M924() {
  int8_t target_extruder = 99;
  int8_t cooldown_extruder = 1; //Default cooldown enabled
  target_extruder = get_target_extruder_from_command();
  if (target_extruder == 1 && active_extruder != 1 && (card.flag.sdprinting || wt_onlineprinting == SPARK_PRINTING))  
  {
     SERIAL_ECHO_START();
     SERIAL_ECHOPGM("Change over to right Extruder\n");
	  wt_changetool();
    if (parser.seen('S'))
    {
        cooldown_extruder = parser.value_int();
        if (cooldown_extruder == 0)
        {
           SERIAL_ECHO_START();
           SERIAL_ECHOPGM("Left Extruder is waiting for next change-over... ");
        }
        else
        {
          queue.priority_enqueue_one_P(PSTR("M109 T0 S0")); // X40 PRO
          SERIAL_ECHO_START();
          SERIAL_ECHOPGM("Left Extruder is cooling down... ");
        }
    }
    else
    {
       queue.priority_enqueue_one_P(PSTR("M109 T0 S0")); // X40 PRO
       SERIAL_ECHO_START();
       SERIAL_ECHOPGM("Left Extruder is cooling down... ");
    }


  } 
  else if (target_extruder == 0 && active_extruder != 0 && (card.flag.sdprinting || wt_onlineprinting == SPARK_PRINTING))  
  {
     SERIAL_ECHO_START();
     SERIAL_ECHOPGM("Change over to left Extruder\n");
	  wt_changetool();
    if (parser.seen('S'))
    {
        cooldown_extruder = parser.value_int();
        if (cooldown_extruder == 0)
        {
           SERIAL_ECHO_START();
           SERIAL_ECHOPGM("Right Extruder is waiting for next change-over... ");
        }
        else
        {
           queue.priority_enqueue_one_P(PSTR("M109 T1 S0")); // X40 PRO
           SERIAL_ECHO_START();
           SERIAL_ECHOPGM("Right Extruder is cooling down... ");
        }
    }
    else
    {
       queue.priority_enqueue_one_P(PSTR("M109 T1 S0")); // X40 PRO
       SERIAL_ECHO_START();
       SERIAL_ECHOPGM("Right Extruder is cooling down... ");
    }


  }
  else
  {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Wrong Extruder change-over command. Extruder change-over command works only during printing!\n");
  }
}

#endif
