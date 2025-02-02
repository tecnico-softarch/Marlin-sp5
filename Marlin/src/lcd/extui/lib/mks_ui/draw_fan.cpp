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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "../../../../MarlinCore.h"
#include "lv_conf.h"
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"
#include "../../../../../Configuration.h"
#include "draw_ui.h"
#include "../../../../module/temperature.h"
#include "../../../../gcode/queue.h"
#include "../../../../gcode/gcode.h"

extern lv_group_t * g;
static lv_obj_t * scr;
static lv_obj_t * fanText;

#define ID_F_ADD     2
#define ID_F_DEC     1
#define ID_F_HIGH    3
#define ID_F_MID     4
#define ID_F_OFF     5
#define ID_F_RETURN  6

static uint8_t fanSpeed;

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_F_ADD:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (fanSpeed + 1 <= 255) {
          fanSpeed=fanSpeed+5;
          ZERO(public_buf_l);
          sprintf_P(public_buf_l, PSTR("M106 S%d"), fanSpeed);
          gcode.process_subcommands_now(public_buf_l);
        }
      }
      break;
    case ID_F_DEC:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (fanSpeed > 0) {
          fanSpeed=fanSpeed-5;
          ZERO(public_buf_l);
          sprintf_P(public_buf_l, PSTR("M106 S%d"), fanSpeed);
          gcode.process_subcommands_now(public_buf_l);
        }
      }

      break;
    case ID_F_HIGH:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        gcode.process_subcommands_now_P(PSTR("M106 S255"));
      }
      break;
    case ID_F_MID:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        gcode.process_subcommands_now_P(PSTR("M106 S127"));
      }
      break;
    case ID_F_OFF:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        gcode.process_subcommands_now_P(PSTR("M107"));
      }
      break;
    case ID_F_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        clear_cur_ui();
        draw_return_ui();
      }
      break;
  }
}

void lv_draw_fan(void) {
  lv_obj_t *buttonAdd, *buttonDec, *buttonHigh, *buttonMid;
  lv_obj_t *buttonOff, *buttonBack;

  #if HAS_FAN
    fanSpeed = thermalManager.fan_speed[0];
  #endif
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != FAN_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = FAN_UI;
  }
  disp_state = FAN_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());


  /*Create an Image button*/
  buttonAdd = lv_imgbtn_create(scr, NULL);
  buttonDec = lv_imgbtn_create(scr, NULL);
  buttonHigh = lv_imgbtn_create(scr, NULL);
  buttonMid = lv_imgbtn_create(scr, NULL);
  buttonOff = lv_imgbtn_create(scr, NULL);
  buttonBack = lv_imgbtn_create(scr, NULL);

  lv_obj_set_event_cb_mks(buttonAdd, event_handler, ID_F_ADD, NULL, 0);
  lv_imgbtn_set_src(buttonAdd, LV_BTN_STATE_REL, "F:/bmp_Add.bin");
  lv_imgbtn_set_src(buttonAdd, LV_BTN_STATE_PR, "F:/bmp_Add.bin");
  lv_imgbtn_set_style(buttonAdd, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonAdd, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_obj_clear_protect(buttonAdd, LV_PROTECT_FOLLOW);
  
  #if 1
    lv_obj_set_event_cb_mks(buttonDec, event_handler, ID_F_DEC, NULL, 0);
    lv_imgbtn_set_src(buttonDec, LV_BTN_STATE_REL, "F:/bmp_Dec.bin");
    lv_imgbtn_set_src(buttonDec, LV_BTN_STATE_PR, "F:/bmp_Dec.bin");
    lv_imgbtn_set_style(buttonDec, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonDec, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonHigh, event_handler,ID_F_HIGH, NULL,0);
    lv_imgbtn_set_src(buttonHigh, LV_BTN_STATE_REL, "F:/bmp_speed255.bin");
    lv_imgbtn_set_src(buttonHigh, LV_BTN_STATE_PR, "F:/bmp_speed255.bin");
    lv_imgbtn_set_style(buttonHigh, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonHigh, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonMid, event_handler,ID_F_MID, NULL,0);
    lv_imgbtn_set_src(buttonMid, LV_BTN_STATE_REL, "F:/bmp_speed127.bin");
    lv_imgbtn_set_src(buttonMid, LV_BTN_STATE_PR, "F:/bmp_speed127.bin");
    lv_imgbtn_set_style(buttonMid, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonMid, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonOff, event_handler,ID_F_OFF, NULL,0);
    lv_imgbtn_set_src(buttonOff, LV_BTN_STATE_REL, "F:/bmp_speed0.bin");
    lv_imgbtn_set_src(buttonOff, LV_BTN_STATE_PR, "F:/bmp_speed0.bin");
    lv_imgbtn_set_style(buttonOff, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonOff, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonBack, event_handler,ID_F_RETURN, NULL,0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
    
  #endif

  lv_obj_set_pos(buttonDec, INTERVAL_V, titleHeight);
  lv_obj_set_pos(buttonAdd, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
  lv_obj_set_pos(buttonHigh, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonMid, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonOff, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

  /*Create a label on the Image button*/
  lv_btn_set_layout(buttonAdd, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonDec, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonHigh, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonMid, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonOff, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

  /*lv_obj_t * labelAdd = lv_label_create(buttonAdd, NULL);
  lv_obj_t * labelDec = lv_label_create(buttonDec, NULL);
  lv_obj_t * labelHigh = lv_label_create(buttonHigh, NULL);
  lv_obj_t * labelMid = lv_label_create(buttonMid, NULL);
  lv_obj_t * labelOff = lv_label_create(buttonOff, NULL);
  lv_obj_t * label_Back = lv_label_create(buttonBack, NULL);


  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(labelAdd, fan_menu.add);
    lv_obj_align(labelAdd, buttonAdd, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelDec, fan_menu.dec);
    lv_obj_align(labelDec, buttonDec, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelHigh, fan_menu.full);
    lv_obj_align(labelHigh, buttonHigh, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelMid, fan_menu.half);
    lv_obj_align(labelMid, buttonMid, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelOff, fan_menu.off);
    lv_obj_align(labelOff, buttonOff, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }*/
  #if BUTTONS_EXIST(EN1, EN2, ENC)
	if (gCfgItems.encoder_enable == true) {
		lv_group_add_obj(g, buttonAdd);
  		lv_group_add_obj(g, buttonDec);
		lv_group_add_obj(g, buttonHigh);
		lv_group_add_obj(g, buttonMid);
		lv_group_add_obj(g, buttonOff);
		lv_group_add_obj(g, buttonBack);
	}
  #endif // BUTTONS_EXIST(EN1, EN2, ENC)

  fanText = lv_label_create(scr, NULL);
  lv_obj_set_style(fanText, &tft_style_label_rel);
  disp_fan_value();
}

void disp_fan_value() {
  char buf1[10] = {0};
  public_buf_l[0] = '\0';
  strcat(public_buf_l, fan_menu.state);
  strcat_P(public_buf_l, PSTR(": "));
  sprintf_P(buf1, PSTR("%3d"), thermalManager.fan_speed[0]);
  strcat(public_buf_l, buf1);
  lv_label_set_text(fanText, public_buf_l);
  lv_obj_align(fanText, NULL, LV_ALIGN_CENTER, 0, -65);
}

void lv_clear_fan() {
	#if BUTTONS_EXIST(EN1, EN2, ENC)
	if (gCfgItems.encoder_enable == true) {
		lv_group_remove_all_objs(g);
	}
  	#endif // BUTTONS_EXIST(EN1, EN2, ENC)
	lv_obj_del(scr); 
}

#endif // HAS_TFT_LVGL_UI
