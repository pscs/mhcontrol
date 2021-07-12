#pragma once

#include <lvgl.h>
#include "window.h"

class LevelWindow: public Window {
public:
	LevelWindow(Window *parent);

	void fastUpdate();

private:
	lv_obj_t *text;
};