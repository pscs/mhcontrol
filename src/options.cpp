#include "options.h"

#include "DS3231.h"

#include "timezone.h"

OptionsScreen Options;

void OptionsScreen::Create(lv_obj_t *scr)
{
  static const lv_coord_t column_dsc[] = {50, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
  static const lv_coord_t row_dsc[] = {16, 16, 16, 16, 16, 16, 16, 16, LV_GRID_TEMPLATE_LAST};
  lv_obj_set_grid_dsc_array(scr, column_dsc, row_dsc);
  lv_obj_set_layout(scr, LV_LAYOUT_GRID);

  lv_obj_t *label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, 0, 1);
  lv_label_set_text(label, "Options");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
  lv_label_set_text(label, "Time");
  timeField = lv_label_create(scr);
  lv_obj_set_grid_cell(timeField, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
  lv_label_set_text(timeField, "**");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 2, 1);
  lv_label_set_text(label, "Timezone");
  timezoneField = lv_label_create(scr);
  lv_obj_set_grid_cell(timezoneField, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 2, 1);
  lv_label_set_text(timezoneField, "**");

}

void OptionsScreen::Update()
{
  log_v("optionsUpdate\n");
	char buf[100];
	time_t clockTime = Clock.Get();
	clockTime += TZ.getOffset();
	tm *clockTm = gmtime(&clockTime);
	strftime(buf, sizeof(buf), "%a %F %T", clockTm);
	lv_label_set_text(timeField, buf);

	lv_label_set_text(timezoneField, TZ.getName().c_str());
}
