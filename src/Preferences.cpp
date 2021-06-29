#include <lvgl.h>

lv_obj_t *scrPreferences;

void createPreferencesScreen()
{
  scrPreferences = lv_obj_create(NULL);

  lv_obj_t *label = lv_label_create(scrPreferences);
  lv_label_set_text(label, "Preferences");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -50);
}
  