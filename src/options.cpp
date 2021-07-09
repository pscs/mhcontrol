#include "options.h"

#include "DS3231.h"

#include "timezone.h"
#include "logger.h"

OptionsScreen Options;

void OptionsScreen::Create(lv_obj_t *scr)
{
	static const lv_coord_t column_dsc[] = {50, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
	static const lv_coord_t row_dsc[] = {16, 16, 16, 16, 16, 16, 16, 16, LV_GRID_TEMPLATE_LAST};
	lv_obj_set_grid_dsc_array(scr, column_dsc, row_dsc);
	lv_obj_set_layout(scr, LV_LAYOUT_GRID);

	int row = 0;
	lv_obj_t *label = lv_label_create(scr);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Time");
	timeField = lv_label_create(scr);
	lv_obj_set_grid_cell(timeField, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_label_set_text(timeField, "**");

	label = lv_label_create(scr);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Timezone");
	timezoneField = lv_label_create(scr);
	lv_obj_set_grid_cell(timezoneField, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_label_set_text(timezoneField, "**");

	label = lv_label_create(scr);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Uptime");
	uptimeField = lv_label_create(scr);
	lv_obj_set_grid_cell(uptimeField, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_label_set_text(uptimeField, "**");

	label = lv_label_create(scr);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Memory");
	freeMemField = lv_label_create(scr);
	lv_obj_set_grid_cell(freeMemField, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_label_set_text(freeMemField, "**");
}

void OptionsScreen::Update()
{
	static uint32_t secs = 0;
	static uint32_t lastMillis = 0;

	logger.send(LOG_OPTIONS, LOG_DEBUG, "optionsUpdate\n");
	char buf[100];
	time_t clockTime = Clock.Get();
	clockTime += TZ.getOffset();
	tm *clockTm = gmtime(&clockTime);
	strftime(buf, sizeof(buf), "%a %F %T", clockTm);
	lv_label_set_text(timeField, buf);

	lv_label_set_text(timezoneField, TZ.getName().c_str());

	snprintf(buf, sizeof(buf), "%.1fkB (%.1fkB)", ESP.getFreeHeap() / 1024.0, ESP.getFreePsram() / 1024.0);
  	lv_label_set_text(freeMemField, buf);

	uint32_t now = millis();
	uint32_t diffSecs = (now - lastMillis) / 1000;
	if (diffSecs) {
		secs += diffSecs;
		lastMillis += diffSecs * 1000;

		snprintf(buf, sizeof(buf), "%d d %d h %d m %ds",
		 	secs / (24 * 60 * 60), (secs / (60 * 60)) % 24, (secs / 60) % 60, secs % 60);
		lv_label_set_text(uptimeField, buf);
	}
}
