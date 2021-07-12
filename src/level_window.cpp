#include "level_window.h"

#include "sound.h"
#include "accelerometer.h"

LevelWindow::LevelWindow(Window *parent_): Window(parent_) {
	scr = lv_win_create(lv_scr_act(), 40);
	lv_win_add_title(scr, "Levelling");

	lv_obj_t *btn = lv_win_add_btn(scr, LV_SYMBOL_CLOSE, 40);
	lv_obj_add_event_cb(btn, [](lv_event_t *e) {
		sound.addNote(1000, 50);
		sound.addNote(500, 50);
		accelerometer.sleep();
		Window::registry("options")->removeChild("win");
	}, LV_EVENT_CLICKED, NULL);

	lv_obj_t *cont = lv_win_get_content(scr);

	text = lv_label_create(cont);

//	accelerometer.wake(ADXL345_DATA_RATE_400);
	accelerometer.startMinMax();	
	update();
}

void LevelWindow::fastUpdate() {
	char buf[200];

	xyzFloat v = accelerometer.getRawValues();
	xyzFloat minV = accelerometer.getMinValues();
	xyzFloat maxV = accelerometer.getMaxValues();

	snprintf(buf, sizeof(buf), "%f %f %f\n%f %f %f\n%f %f %f", v.x, v.y, v.z, 
		minV.x, minV.y, minV.z, maxV.x, maxV.y, maxV.z);
	lv_label_set_text(text, buf);
}
