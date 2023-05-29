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

#include "../../inc/MarlinConfig.h"

#if HAS_HOTEND_OFFSET

#include "../gcode.h"
#include "../../module/motion.h"

#if ENABLED(DELTA)
  #include "../../module/planner.h"
#endif

extern uint8_t wtvar_tune_x1;
extern uint8_t wtvar_tune_x2;
extern uint8_t wtvar_tune_x3;
extern uint8_t wtvar_tune_y1;
extern uint8_t wtvar_tune_y2;
extern uint8_t wtvar_tune_y3;

float absolute(float s)
{
  if (s < 0) 
    {
    s = s * (-1);
    }
  return s;
}


/**
 * M218 - set hotend offset (in linear units)
 *
 *   T<tool>
 *   X<xoffset>
 *   Y<yoffset>
 *   Z<zoffset>
 */
void GcodeSuite::M218() {

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  if (parser.seenval('X')) 
  {
      hotend_offset[target_extruder].x = parser.value_linear_units();

      // X40 PRO
      if (target_extruder == 1)
      {
        wtvar_tune_x1 = (uint8_t)((int)(hotend_offset[1].x) - T1_OFFSET_X) + 3;

        wtvar_tune_x2 = (uint8_t)(((float)((float)hotend_offset[1].x - (int)(hotend_offset[1].x))) * 10 + 5); 

        wtvar_tune_x3 = (uint8_t)(((float)((float)hotend_offset[1].x - (int)(hotend_offset[1].x))) * 100 + 5.5);

        while(wtvar_tune_x3 > 10)
        {
          wtvar_tune_x3 -= 10;
        } 

       
        //Rounding
        if (wtvar_tune_x3 < 5) wtvar_tune_x2 += 1;

        if (wtvar_tune_x2 > 10 || wtvar_tune_x2 < 5) wtvar_tune_x1 += 1;

        while(wtvar_tune_x2 > 10)
        {
          wtvar_tune_x2 -= 10;
        } 


        if (wtvar_tune_x1 > 5)
        {
          wtvar_tune_x1 = 5;
          wtvar_tune_x2 = 10;
          wtvar_tune_x3 = 10;
        }

        //Max. limit
        if (hotend_offset[1].x >= 349.55) 
        {
          wtvar_tune_x1 = 5;
          wtvar_tune_x2 = 10;
          wtvar_tune_x3 = 10;
        }

        //Min. limit
        if (hotend_offset[1].x <= 344.56) 
        {
          wtvar_tune_x1 = 1;
          wtvar_tune_x2 = 1;
          wtvar_tune_x3 = 1;
        }



      hotend_offset[1].x = T1_OFFSET_X + (wtvar_tune_x1 - 3) + ((float)wtvar_tune_x2 - 5) / 10 + ((float)wtvar_tune_x3 - 5) / 100;



      }
  }

  if (parser.seenval('Y'))
  {
      hotend_offset[target_extruder].y = parser.value_linear_units();

      // X40 PRO
      if (target_extruder == 1)
      {
        if (hotend_offset[1].y >= 0)    //Positiver Offset
        {
          wtvar_tune_y1 = (uint8_t)(hotend_offset[1].y + 3);

          wtvar_tune_y2 = (uint8_t)(((float)((float)hotend_offset[1].y - (int)(hotend_offset[1].y))) * 10 + 5);

          wtvar_tune_y3 = (uint8_t)(((float)((float)hotend_offset[1].y - (int)(hotend_offset[1].y))) * 100 + 5.5);

          //Max. limit positiv
          if (hotend_offset[1].y > 2.55) 
          {
            wtvar_tune_y1 = 5;
            wtvar_tune_y2 = 10;
            wtvar_tune_y3 = 10;
          }
          else
          {
          //Aufrunden positiv 
            if (wtvar_tune_y3 > 10)
            {
                while(wtvar_tune_y3 > 10)
                {
                  wtvar_tune_y3 -= 10;
                } 
                if (wtvar_tune_y3 <= 5)
                {
                  wtvar_tune_y2 += 1;
                }
            }

            if (wtvar_tune_y2 > 10)
            {
                while(wtvar_tune_y2 > 10)
                {
                  wtvar_tune_y2 -= 10;
                } 
                if (wtvar_tune_y2 <= 5)
                {
                wtvar_tune_y1 += 1;
                }


                if (wtvar_tune_y1 > 5)
                {
                  wtvar_tune_y1 = 5;
                  wtvar_tune_y2 = 10;
                  wtvar_tune_y3 = 10;
                }
            }
          //Abrunden positiv
          if (wtvar_tune_y3 <1) wtvar_tune_y3 = 1;
          if (wtvar_tune_y2 <1) wtvar_tune_y2 = 1;
          if (wtvar_tune_y1 <1) wtvar_tune_y1 = 1;


          }















        }
//--------------------------------------------------------------------------------------------------------------
        if (hotend_offset[1].y < 0)   //Negativer Offset
        {
         float tmp_tune_y1; 
         float tmp_tune_y2; 
         float tmp_tune_y3; 


          tmp_tune_y1 = (int)hotend_offset[1].y + 3;  //Geprüft ist richtig
          // bei -1 + 3 => Taste 2

          if (tmp_tune_y1 < 1) 
          {
            tmp_tune_y1 = 1;
            tmp_tune_y2 = 1;
            tmp_tune_y3 = 1;
          }

          //Berechnung absolutwert ohne Nachkommastellen

          tmp_tune_y2 = (int)((hotend_offset[1].y + absolute((int)hotend_offset[1].y))  * 10);
          tmp_tune_y2 += 5; //Tastenversatz hinzugefügt
          //                         (-1,01         +  1        ) => -0.1 * 10 = -1 + 5 = 0



          SERIAL_ECHO_START();
          SERIAL_ECHO("Offset: ");
          SERIAL_ECHO(hotend_offset[1].y);
          SERIAL_ECHO("\n");
          SERIAL_ECHO_START();
          SERIAL_ECHO("tmp_tune_y: ");
          SERIAL_ECHO(tmp_tune_y1);
          SERIAL_ECHO("\n");
          SERIAL_ECHO_START();
          SERIAL_ECHO("tmp_tune_y2 mit Tasterturversatz: ");
          SERIAL_ECHO(tmp_tune_y2);
          SERIAL_ECHO("\n");

          if(tmp_tune_y2 < 1) 
          {
            while (tmp_tune_y2 < 1) tmp_tune_y2 += 10;
            tmp_tune_y1 -= 1;
            if (tmp_tune_y1 < 1) 
              {
                tmp_tune_y1 = 1;
                tmp_tune_y2 = 1;
                tmp_tune_y3 = 1;
              }
          }
       

          tmp_tune_y3 = (float)(   (hotend_offset[1].y +     absolute((int)hotend_offset[1].y)    )  * 100);
          SERIAL_ECHO_START();
          SERIAL_ECHO("hotend_offset[1].y: ");
          SERIAL_ECHO(hotend_offset[1].y);
          SERIAL_ECHO("\n");

          SERIAL_ECHO_START();
          SERIAL_ECHO("absolute: ");
          SERIAL_ECHO(absolute((int)hotend_offset[1].y));
          SERIAL_ECHO("\n");

          SERIAL_ECHO_START();
          SERIAL_ECHO("hotend_offset[1].y + absolute((int)hotend_offset[1].y): ");
          SERIAL_ECHO(hotend_offset[1].y + absolute((int)hotend_offset[1].y));
          SERIAL_ECHO("\n");

          SERIAL_ECHO_START();
          SERIAL_ECHO("(hotend_offset[1].y + absolute((int)hotend_offset[1].y)) * 100: ");
          SERIAL_ECHO((hotend_offset[1].y + absolute((int)hotend_offset[1].y)) * 100);
          SERIAL_ECHO("\n");


          SERIAL_ECHO_START();
          SERIAL_ECHO("tmp_tune_y3 ohne Korrektur: ");
          SERIAL_ECHO(tmp_tune_y3);
          SERIAL_ECHO("\n");


          while (tmp_tune_y3 < -9) tmp_tune_y3 += 10; //Erste Nachkommastelle entfernen
          tmp_tune_y3 += 5; //Tastenversatz hinzugefügt
      


          if(tmp_tune_y3 <= 0) 
          {
            while (tmp_tune_y3 <= 0) tmp_tune_y3 += 10;
            tmp_tune_y2 -= 1;
            if (tmp_tune_y2 < 1) 
              {
                tmp_tune_y1 -= 1;
                tmp_tune_y2 += 10;
                if (tmp_tune_y1 < 1) 
                {
                  tmp_tune_y1 = 1;
                  tmp_tune_y2 = 1;
                  tmp_tune_y3 = 1;
                }
              }
          }

          SERIAL_ECHO_START();
          SERIAL_ECHO("tmp_tune_y3 mit Tasterturversatz und Korrektur: ");
          SERIAL_ECHO(tmp_tune_y3);
          SERIAL_ECHO("\n");

          wtvar_tune_y1 = (uint8_t)tmp_tune_y1;
          wtvar_tune_y2 = (uint8_t)tmp_tune_y2;
          wtvar_tune_y3 = (uint8_t)(tmp_tune_y3 + 0.1);

          SERIAL_ECHO_START();
          SERIAL_ECHO("wtvar_tune_y1: ");
          SERIAL_ECHO(wtvar_tune_y1);
          SERIAL_ECHO("\n");
          SERIAL_ECHO_START();
          SERIAL_ECHO("wtvar_tune_y2: ");
          SERIAL_ECHO(wtvar_tune_y2);
          SERIAL_ECHO("\n");
          SERIAL_ECHO_START();
          SERIAL_ECHO("wtvar_tune_y3: ");
          SERIAL_ECHO(wtvar_tune_y3);


          //Mim. limit negativ
          if (hotend_offset[1].y < -2.44) 
          {
            wtvar_tune_y1 = 1;
            wtvar_tune_y2 = 1;
            wtvar_tune_y3 = 1;
          }
       
        }


        //Für Positive und negative Werte
        hotend_offset[1].y = (wtvar_tune_y1 - 3) + ((float)wtvar_tune_y2 - 5) / 10 + ((float)wtvar_tune_y3 - 5) / 100;  //Geprüft ist richtig
        //                   (   3          - 3) +         (5 -5 ) / 10            +        (4-5)= -1 / 100            ==> -0,01
        //                   (   3           -3) +         (6 -5 ) = 1 / 10 

      }
  }

  if (parser.seenval('Z')) hotend_offset[target_extruder].z = parser.value_linear_units();

  if (!parser.seen("XYZ")) {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(STR_HOTEND_OFFSET);
    HOTEND_LOOP() {
      SERIAL_CHAR(' ');
      SERIAL_ECHO(hotend_offset[e].x);
      SERIAL_CHAR(',');
      SERIAL_ECHO(hotend_offset[e].y);
      SERIAL_CHAR(',');
      SERIAL_ECHO_F(hotend_offset[e].z, 3);
    }
    SERIAL_EOL();
  }

  #if ENABLED(DELTA)
    if (target_extruder == active_extruder)
      do_blocking_move_to_xy(current_position, planner.settings.max_feedrate_mm_s[X_AXIS]);
  #endif
}

#endif // HAS_HOTEND_OFFSET
