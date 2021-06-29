#include "power_screen.h"

void PowerScreen::Create(lv_obj_t *scr)
{
  static const lv_coord_t column_dsc[] = {50, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
  static const lv_coord_t row_dsc[] = {16, 16, 16, 16, 16, 16, 16, 16, LV_GRID_TEMPLATE_LAST};
  lv_obj_set_grid_dsc_array(scr, column_dsc, row_dsc);
  lv_obj_set_layout(scr, LV_LAYOUT_GRID);

  lv_obj_t *label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
  lv_label_set_text(label, "Solar In");
  solarIn = lv_label_create(scr);
  lv_obj_set_grid_cell(solarIn, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
  lv_label_set_text(solarIn, "**");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
  lv_label_set_text(label, "Solar Out");
  solarOut = lv_label_create(scr);
  lv_obj_set_grid_cell(solarOut, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
  lv_label_set_text(solarOut, "**");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 2, 1);
  lv_label_set_text(label, "B2B");
  b2b = lv_label_create(scr);
  lv_obj_set_grid_cell(b2b, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 2, 1);
  lv_label_set_text(b2b, "**");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 3, 1);
  lv_label_set_text(label, "Mains");
  mains = lv_label_create(scr);
  lv_obj_set_grid_cell(mains, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 3, 1);
  lv_label_set_text(mains, "**");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 4, 1);
  lv_label_set_text(label, "Battery");
  battery = lv_label_create(scr);
  lv_obj_set_grid_cell(battery, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 4, 1);
  lv_label_set_text(battery, "**");

}