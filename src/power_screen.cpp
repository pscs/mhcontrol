#include "power_screen.h"

#include "victron_smartsolar.h"
#include "victron_B2B.h"
#include "victron_mainscharger.h"
#include "ksenergy_battery.h"

PowerScreen powerScreen;

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

void PowerScreen::Update() {
  if (victronSmartSolar.isConnected()) {
    lv_label_set_text_fmt(solarIn, "%dV %dA %dW", victronSmartSolar.getVoltage(0), victronSmartSolar.getCurrent(0), victronSmartSolar.getPower(0));
    lv_label_set_text_fmt(solarOut, "%dV %dA %d %dC", victronSmartSolar.getVoltage(1), victronSmartSolar.getCurrent(1), victronSmartSolar.getState(),
        victronSmartSolar.getTemperature());
  } else {
    lv_label_set_text(solarIn, "not connected");
    lv_label_set_text(solarOut, "not connected");
  }

  if (victronB2B.isConnected()) {
    lv_label_set_text_fmt(b2b, "%dV %dV %d %dC", victronB2B.getVoltage(0), victronB2B.getVoltage(1), victronB2B.getState(),
        victronB2B.getTemperature());
  } else {
    lv_label_set_text(b2b, "not connected");
  }

  if (victronMainsCharger.isConnected()) {
    lv_label_set_text_fmt(mains, "%dV %dA %dW %d %C", victronMainsCharger.getVoltage(0), victronMainsCharger.getCurrent(0), 
        victronMainsCharger.getPower(0), victronMainsCharger.getState(), victronMainsCharger.getTemperature());
  } else {
    lv_label_set_text(mains, "not connected");  }

  if (ksEnergyBattery.isConnected()) {
    lv_label_set_text_fmt(battery, "%dV %dA %d%% %d %dC", ksEnergyBattery.getVoltage(), ksEnergyBattery.getCurrent(), 
      ksEnergyBattery.getSOC(), ksEnergyBattery.getState(), ksEnergyBattery.getTemperature());
  } else {
    lv_label_set_text(battery, "not connected");
  }

}