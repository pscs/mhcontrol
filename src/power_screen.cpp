#include "power_screen.h"

#include "victron_smartsolar.h"
#include "victron_B2B.h"
#include "victron_mainscharger.h"
#include "ksenergy_battery.h"
#include "Controller.h"
#include "config.h"
#include "sound.h"

PowerScreen powerScreen;

extern const lv_img_dsc_t imgPlug;
extern const lv_img_dsc_t imgSolarpanel;
extern const lv_img_dsc_t imgBattery;

void PowerScreen::Create(lv_obj_t *scr_)
{
	static const lv_coord_t column_dsc[] = {100, 100, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
	static const lv_coord_t row_dsc[] = {30, 70, 16, 16, 16, 16, 16, 16, 16, LV_GRID_TEMPLATE_LAST};
	scr = scr_;

	Window::registerWindow("powerscreen", this);

	int secondWidth = 2;
	lv_obj_set_grid_dsc_array(scr, column_dsc, row_dsc);
	lv_obj_set_layout(scr, LV_LAYOUT_GRID);

	int row = 0;

	lv_obj_t *label = lv_label_create(scr);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Current");

	mainsImg = lv_img_create(scr);
	lv_obj_add_flag(mainsImg, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_CLICKABLE);
	lv_obj_align(mainsImg, LV_ALIGN_TOP_LEFT, 0, 30 + 0);
	lv_img_set_src(mainsImg, &imgPlug);

	lv_obj_add_event_cb(mainsImg, [](lv_event_t *e) {
		powerScreen.addChild("win", new MainsChargerInfoWindow(&powerScreen));
		sound.addNote(500, 50);
		sound.addNote(1000, 50);
	}, LV_EVENT_CLICKED, NULL);

	solarImg = lv_img_create(scr);
	lv_obj_add_flag(solarImg, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_CLICKABLE);
	lv_obj_align(solarImg, LV_ALIGN_TOP_LEFT, 34, 30 + 19);
	lv_img_set_src(solarImg, &imgSolarpanel);
	
	lv_obj_add_event_cb(solarImg, [](lv_event_t *e) {
		powerScreen.addChild("win", new SolarInfoWindow(&powerScreen));
		sound.addNote(500, 50);
		sound.addNote(1000, 50);
	}, LV_EVENT_CLICKED, NULL);

	b2bImg = lv_img_create(scr);
	lv_obj_add_flag(b2bImg, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_CLICKABLE);
	lv_obj_align(b2bImg, LV_ALIGN_TOP_LEFT, 68, 30 + 38);
	lv_img_set_src(b2bImg, &imgBattery);

	lv_obj_add_event_cb(b2bImg, [](lv_event_t *e) {
		powerScreen.addChild("win", new B2BInfoWindow(&powerScreen));
		sound.addNote(500, 50);
		sound.addNote(1000, 50);
	}, LV_EVENT_CLICKED, NULL);


	currentMeter = lv_meter_create(scr);
	lv_obj_set_grid_cell(currentMeter, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row, 2);
	lv_obj_set_size(currentMeter, 100, 100);
	lv_obj_remove_style(currentMeter, NULL, LV_PART_INDICATOR);
	lv_obj_set_style_text_font(currentMeter, &lv_font_montserrat_8, LV_PART_TICKS);
	currentMeterScale = lv_meter_add_scale(currentMeter);
	lv_meter_set_scale_range(currentMeter, currentMeterScale, -200, 200, 300, 120);
	lv_meter_set_scale_ticks(currentMeter, currentMeterScale, 21, 1, 2, lv_palette_main(LV_PALETTE_GREY));
	lv_meter_scale_t *scaleAmps = lv_meter_add_scale(currentMeter);
	lv_meter_set_scale_range(currentMeter, scaleAmps, -20, 20, 300, 120);
	lv_meter_set_scale_ticks(currentMeter, scaleAmps, 5, 1, 2, lv_palette_main(LV_PALETTE_GREY));
	lv_meter_set_scale_major_ticks(currentMeter, scaleAmps, 1, 1, 10, lv_color_black(), 10);
	currentIndicator = lv_meter_add_needle_line(currentMeter, currentMeterScale, 2, lv_palette_main(LV_PALETTE_BLUE), -10);

	socMeter = lv_bar_create(scr);
	lv_obj_set_grid_cell(socMeter, LV_GRID_ALIGN_CENTER, secondWidth, 1, LV_GRID_ALIGN_CENTER, row, 2);
	lv_obj_set_size(socMeter, 20, 100);
	lv_bar_set_range(socMeter, 0, 100);

	row += 2;

	label = lv_label_create(scr);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Solar");
	solarBar = lv_bar_create(scr);
	lv_obj_set_grid_cell(solarBar, LV_GRID_ALIGN_CENTER, 1, secondWidth, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_obj_set_size(solarBar, 200, 20);
	lv_bar_set_range(solarBar, 0, SOLAR_MAX_POWER);

	label = lv_label_create(scr);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Battery");
	battery = lv_label_create(scr);
	lv_obj_set_grid_cell(battery, LV_GRID_ALIGN_CENTER, 1, secondWidth, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_label_set_text(battery, "**");

}

void PowerScreen::Update() {
  if (victronSmartSolar.isConnected() || victronSmartSolar.isDemoMode()) {
    lv_bar_set_value(solarBar, victronSmartSolar.getPower(0) / 100, LV_ANIM_ON);

	if (lv_obj_has_flag(solarImg, LV_OBJ_FLAG_HIDDEN)) {
		lv_obj_clear_flag(solarImg, LV_OBJ_FLAG_HIDDEN);
	}
  } else {
    lv_bar_set_value(solarBar, 0, LV_ANIM_ON);
	if (!lv_obj_has_flag(solarImg, LV_OBJ_FLAG_HIDDEN)) {
		lv_obj_add_flag(solarImg, LV_OBJ_FLAG_HIDDEN);
	}

  }

  if (victronB2B.isConnected() || victronB2B.isDemoMode()) {
	if (lv_obj_has_flag(b2bImg, LV_OBJ_FLAG_HIDDEN)) {
		lv_obj_clear_flag(b2bImg, LV_OBJ_FLAG_HIDDEN);
	}
  } else {
 	if (!lv_obj_has_flag(b2bImg, LV_OBJ_FLAG_HIDDEN)) {
		lv_obj_add_flag(b2bImg, LV_OBJ_FLAG_HIDDEN);
	}
  }

  if (victronMainsCharger.isConnected() || victronMainsCharger.isDemoMode()) {

	if (lv_obj_has_flag(mainsImg, LV_OBJ_FLAG_HIDDEN)) {
		lv_obj_clear_flag(mainsImg, LV_OBJ_FLAG_HIDDEN);
	}
  } else {
	if (!lv_obj_has_flag(mainsImg, LV_OBJ_FLAG_HIDDEN)) {
		lv_obj_add_flag(mainsImg, LV_OBJ_FLAG_HIDDEN);
	}
  }

  if (ksEnergyBattery.isConnected() || ksEnergyBattery.isDemoMode()) {
    char buf[100];
    snprintf(buf, sizeof(buf), "%.2fV %.2fA %d%% %d %dC", ksEnergyBattery.getVoltage() / 1000.0, ksEnergyBattery.getCurrent() / 1000.0, 
      ksEnergyBattery.getSOC(), ksEnergyBattery.getState(), ksEnergyBattery.getTemperature() - 40);
    lv_label_set_text(battery, buf);
    lv_meter_set_indicator_value(currentMeter, currentIndicator, ksEnergyBattery.getCurrent() / 100);
    lv_bar_set_value(socMeter, ksEnergyBattery.getSOC(), LV_ANIM_ON);
  } else {
    lv_label_set_text(battery, "not connected");
    lv_meter_set_indicator_value(currentMeter, currentIndicator, 0);
    lv_bar_set_value(socMeter, 0, LV_ANIM_ON);
  }

  Window::Update();
}

//////////////////

MainsChargerInfoWindow::MainsChargerInfoWindow(Window *parent_): Window(parent_) {
	printf("show Mains Info Window\n");
	scr = lv_win_create(lv_scr_act(), 40);
	lv_win_add_title(scr, "Mains Charger");

	lv_obj_t *btn = lv_win_add_btn(scr, LV_SYMBOL_CLOSE, 40);
	lv_obj_add_event_cb(btn, [](lv_event_t *e) {
		printf("close clicked\n");
		sound.addNote(1000, 100);
		sound.addNote(500, 100);
		Window::registry("powerscreen")->removeChild("win");
		//lv_obj_del_async(powerScreen.win);
	}, LV_EVENT_CLICKED, NULL);

	lv_obj_t *cont = lv_win_get_content(scr);

	static const lv_coord_t column_dsc[] = {100, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
	static const lv_coord_t row_dsc[] = {16, 16, 16, 16, 16, 16, 16, 16, 16, LV_GRID_TEMPLATE_LAST};

	lv_obj_set_grid_dsc_array(cont, column_dsc, row_dsc);
	lv_obj_set_layout(cont, LV_LAYOUT_GRID);

	int row = 0;

	lv_obj_t *label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "State");
	connState = lv_label_create(cont);
	lv_obj_set_grid_cell(connState, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Last update");
	lastUpdate = lv_label_create(cont);
	lv_obj_set_grid_cell(lastUpdate, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Voltage");
	voltage = lv_label_create(cont);
	lv_obj_set_grid_cell(voltage, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Current");
	current = lv_label_create(cont);
	lv_obj_set_grid_cell(current, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Temp");
	temperature = lv_label_create(cont);
	lv_obj_set_grid_cell(temperature, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Status");
	status = lv_label_create(cont);
	lv_obj_set_grid_cell(status, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	Update();
}

void MainsChargerInfoWindow::Update() {
	char buf[100];

	snprintf(buf, sizeof(buf), "%s %s %s", victronMainsCharger.isEnabled() ? "Enabled" : "Disabled",
	victronMainsCharger.isConnected() ? "Connected" : "Disconnected",
	victronMainsCharger.isDemoMode() ? "Demo Mode" : "");
	lv_label_set_text(connState, buf);

	lv_label_set_text(lastUpdate, victronMainsCharger.getLastUpdateTimeText());

	snprintf(buf, sizeof(buf), "%.2f V", victronMainsCharger.getVoltage(1) / 100.0);
	lv_label_set_text(voltage, buf);
	snprintf(buf, sizeof(buf), "%.2f A", victronMainsCharger.getCurrent(1) / 100.0);
	lv_label_set_text(current, buf);
	snprintf(buf, sizeof(buf), "%s", victronMainsCharger.getStateText());
	lv_label_set_text(status, buf);
	snprintf(buf, sizeof(buf), "%.1f\u00b0C", victronMainsCharger.getTemperature() / 100.0);
	lv_label_set_text(temperature, buf);
}

/////////////////////////////////////////


SolarInfoWindow::SolarInfoWindow(Window *parent_): Window(parent_) {
	scr = lv_win_create(lv_scr_act(), 40);
	lv_win_add_title(scr, "Solar Charger");

	lv_obj_t *btn = lv_win_add_btn(scr, LV_SYMBOL_CLOSE, 40);
	lv_obj_add_event_cb(btn, [](lv_event_t *e) {
		sound.addNote(1000, 100);
		sound.addNote(500, 100);
		Window::registry("powerscreen")->removeChild("win");
		//lv_obj_del_async(powerScreen.win);
	}, LV_EVENT_CLICKED, NULL);

	lv_obj_t *cont = lv_win_get_content(scr);

	static const lv_coord_t column_dsc[] = {100, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
	static const lv_coord_t row_dsc[] = {16, 16, 16, 16, 16, 16, 16, 16, 16, LV_GRID_TEMPLATE_LAST};

	lv_obj_set_grid_dsc_array(cont, column_dsc, row_dsc);
	lv_obj_set_layout(cont, LV_LAYOUT_GRID);

	int row = 0;

	lv_obj_t *label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "State");
	connState = lv_label_create(cont);
	lv_obj_set_grid_cell(connState, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Last update");
	lastUpdate = lv_label_create(cont);
	lv_obj_set_grid_cell(lastUpdate, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Panel voltage");
	panelVoltage = lv_label_create(cont);
	lv_obj_set_grid_cell(panelVoltage, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Panel power");
	panelPower = lv_label_create(cont);
	lv_obj_set_grid_cell(panelPower, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Charge voltage");
	chargeVoltage = lv_label_create(cont);
	lv_obj_set_grid_cell(chargeVoltage, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Charge current");
	chargeCurrent = lv_label_create(cont);
	lv_obj_set_grid_cell(chargeCurrent, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Temp");
	temperature = lv_label_create(cont);
	lv_obj_set_grid_cell(temperature, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Status");
	status = lv_label_create(cont);
	lv_obj_set_grid_cell(status, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	Update();
}

void SolarInfoWindow::Update() {
	char buf[100];

	snprintf(buf, sizeof(buf), "%s %s %s", victronSmartSolar.isEnabled() ? "Enabled" : "Disabled",
	victronSmartSolar.isConnected() ? "Connected" : "Disconnected",
	victronSmartSolar.isDemoMode() ? "Demo Mode" : "");
	lv_label_set_text(connState, buf);

	lv_label_set_text(lastUpdate, victronSmartSolar.getLastUpdateTimeText());

	snprintf(buf, sizeof(buf), "%.2f V", victronSmartSolar.getVoltage(0) / 100.0);
	lv_label_set_text(panelVoltage, buf);
	snprintf(buf, sizeof(buf), "%.2f W", victronSmartSolar.getPower(0) / 100.0);
	lv_label_set_text(panelPower, buf);
	snprintf(buf, sizeof(buf), "%.2f V", victronSmartSolar.getVoltage(1) / 100.0);
	lv_label_set_text(chargeVoltage, buf);
	snprintf(buf, sizeof(buf), "%.2f A", victronSmartSolar.getCurrent(1) / 100.0);
	lv_label_set_text(chargeCurrent, buf);
	snprintf(buf, sizeof(buf), "%s", victronSmartSolar.getStateText());
	lv_label_set_text(status, buf);
	snprintf(buf, sizeof(buf), "%.1f\u00b0C", victronSmartSolar.getTemperature() / 100.0);
	lv_label_set_text(temperature, buf);
}

/////////////////////////////


B2BInfoWindow::B2BInfoWindow(Window *parent_): Window(parent_) {
	scr = lv_win_create(lv_scr_act(), 40);
	lv_win_add_title(scr, "B2B Charger");

	lv_obj_t *btn = lv_win_add_btn(scr, LV_SYMBOL_CLOSE, 40);
	lv_obj_add_event_cb(btn, [](lv_event_t *e) {
		sound.addNote(1000, 100);
		sound.addNote(500, 100);
		Window::registry("powerscreen")->removeChild("win");
		//lv_obj_del_async(powerScreen.win);
	}, LV_EVENT_CLICKED, NULL);

	lv_obj_t *cont = lv_win_get_content(scr);

	static const lv_coord_t column_dsc[] = {100, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
	static const lv_coord_t row_dsc[] = {16, 16, 16, 16, 16, 16, 16, 16, 16, LV_GRID_TEMPLATE_LAST};

	lv_obj_set_grid_dsc_array(cont, column_dsc, row_dsc);
	lv_obj_set_layout(cont, LV_LAYOUT_GRID);

	int row = 0;

	lv_obj_t *label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "State");
	connState = lv_label_create(cont);
	lv_obj_set_grid_cell(connState, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Last update");
	lastUpdate = lv_label_create(cont);
	lv_obj_set_grid_cell(lastUpdate, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "In Voltage");
	inputVoltage = lv_label_create(cont);
	lv_obj_set_grid_cell(inputVoltage, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Charge Voltage");
	chargeVoltage = lv_label_create(cont);
	lv_obj_set_grid_cell(chargeVoltage, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Temp");
	temperature = lv_label_create(cont);
	lv_obj_set_grid_cell(temperature, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Status");
	status = lv_label_create(cont);
	lv_obj_set_grid_cell(status, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	Update();
}

void B2BInfoWindow::Update() {
	char buf[100];

	snprintf(buf, sizeof(buf), "%s %s %s", victronB2B.isEnabled() ? "Enabled" : "Disabled",
	victronB2B.isConnected() ? "Connected" : "Disconnected",
	victronB2B.isDemoMode() ? "Demo Mode" : "");
	lv_label_set_text(connState, buf);

	lv_label_set_text(lastUpdate, victronB2B.getLastUpdateTimeText());

	snprintf(buf, sizeof(buf), "%.2f V", victronB2B.getVoltage(0) / 100.0);
	lv_label_set_text(inputVoltage, buf);
	snprintf(buf, sizeof(buf), "%.2f V", victronB2B.getVoltage(1) / 100.0);
	lv_label_set_text(chargeVoltage, buf);
	snprintf(buf, sizeof(buf), "%s", victronB2B.getStateText());
	lv_label_set_text(status, buf);
	snprintf(buf, sizeof(buf), "%.1f\u00b0C", victronB2B.getTemperature() / 100.0);
	lv_label_set_text(temperature, buf);
}
