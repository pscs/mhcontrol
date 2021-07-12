#include "options.h"

#include "DS3231.h"

#include "timezone.h"
#include "logger.h"
#include "screens.h"
#include "sound.h"
#include "Controller.h"
#include <WiFi.h>
#include "inifile.h"
#include "config.h"
#include "level_window.h"

OptionsScreen options;

void OptionsScreen::Create(lv_obj_t *scr_)
{
	static const lv_coord_t column_dsc[] = {50, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
	static const lv_coord_t row_dsc[] = {16, 16, 16, 16, 40, 16, 16, 16, LV_GRID_TEMPLATE_LAST};

	scr = scr_;

	Window::registerWindow("options", this);

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

	static const char *btnm_map[] = {"Cal scrn", "Network", "Level", ""};
	lv_obj_t *btnm = lv_btnmatrix_create(scr);
	lv_btnmatrix_set_map(btnm, btnm_map);
	lv_obj_set_size(btnm, screenWidth - 20, 38);
	lv_obj_set_style_height(btnm, 34, LV_PART_ITEMS);
	lv_obj_set_style_pad_all(btnm, 3, LV_PART_MAIN);
	lv_obj_set_style_bg_color(btnm, lv_palette_darken(LV_PALETTE_GREY, 3), LV_PART_MAIN);
	lv_obj_set_style_bg_color(btnm, lv_palette_main(LV_PALETTE_GREY), LV_PART_ITEMS);
	lv_obj_set_grid_cell(btnm, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, row, 1);

	lv_obj_add_event_cb(btnm, [](lv_event_t *e) {
		lv_obj_t *t = lv_event_get_target(e);
		uint32_t id = lv_btnmatrix_get_selected_btn(t);
		sound.addNote(1000, 50);
		if (id == 0) {
			ScreenClass::doCalibration();
		} else if (id == 1) {
			options.addChild("win", new NetworkWindow(&options));
		} else if (id == 2) {
			options.addChild("win", new LevelWindow(&options));
		}
	}, LV_EVENT_VALUE_CHANGED, NULL);

/*	lv_obj_t *btn = lv_btn_create(scr);
	lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_SPACE_EVENLY, 0, 2, LV_GRID_ALIGN_CENTER, row, 1);

	label = lv_label_create(btn);
	lv_obj_center(label);
	lv_label_set_text(label, "Cal scrn");
	
	lv_obj_add_event_cb(btn, ScreenClass::doCalibration, LV_EVENT_CLICKED, NULL);

	btn = lv_btn_create(scr);
	lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_SPACE_EVENLY, 0, 2, LV_GRID_ALIGN_CENTER, row, 1);

	label = lv_label_create(btn);
	lv_obj_center(label);
	lv_label_set_text(label, "Network");

	btn = lv_btn_create(scr);
	lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_SPACE_EVENLY, 0, 2, LV_GRID_ALIGN_CENTER, row, 1);

	label = lv_label_create(btn);
	lv_obj_center(label);
	lv_label_set_text(label, "Time");
*/
}

void OptionsScreen::update()
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

	Window::update();
}

NetworkWindow::NetworkWindow(Window *parent_): Window(parent_) {
	scr = lv_win_create(lv_scr_act(), 40);
	lv_win_add_title(scr, "Network Info");

	lv_obj_t *btn = lv_win_add_btn(scr, LV_SYMBOL_CLOSE, 40);
	lv_obj_add_event_cb(btn, [](lv_event_t *e) {
		sound.addNote(1000, 50);
		sound.addNote(500, 50);
		Window::registry("options")->removeChild("win");
	}, LV_EVENT_CLICKED, NULL);

	lv_obj_t *cont = lv_win_get_content(scr);

	static const lv_coord_t column_dsc[] = {100, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
	static const lv_coord_t row_dsc[] = {16, 16, 16, 16, 16, 16, 16, 16, 16, LV_GRID_TEMPLATE_LAST};

	lv_obj_set_grid_dsc_array(cont, column_dsc, row_dsc);
	lv_obj_set_layout(cont, LV_LAYOUT_GRID);

	int row = 0;

	lv_obj_t *label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "SSID");
	ssid = lv_label_create(cont);
	lv_obj_set_grid_cell(ssid, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "SSID");
	rssi = lv_label_create(cont);
	lv_obj_set_grid_cell(rssi, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	btn = lv_btn_create(cont);
	lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, row++, 1);
	label = lv_label_create(btn);
	lv_label_set_text(label, "Config");
	lv_obj_add_event_cb(btn, [](lv_event_t *e) {
		Window *p = reinterpret_cast<Window *>(lv_event_get_user_data(e));
		options.addChild("config", new NetworkSettingsWindow(p));
	}, LV_EVENT_CLICKED, this);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "IP Addr");
	ipaddress = lv_label_create(cont);
	lv_obj_set_grid_cell(ipaddress, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Netmask");
	mask = lv_label_create(cont);
	lv_obj_set_grid_cell(mask, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Gateway");
	gateway = lv_label_create(cont);
	lv_obj_set_grid_cell(gateway, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "DNS Svr");
	dns = lv_label_create(cont);
	lv_obj_set_grid_cell(dns, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	
	update();
}

void NetworkWindow::update() {
	char buf[50];
	lv_label_set_text(ssid, WiFi.SSID().c_str());
	snprintf(buf, sizeof(buf), "%d dB", WiFi.RSSI());
	lv_label_set_text(rssi, buf);
	if (DHCPMode) {
		snprintf(buf, sizeof(buf), "%s (DHCP)", WiFi.localIP().toString().c_str());
	} else {
		snprintf(buf, sizeof(buf), "%s (Static)", WiFi.localIP().toString().c_str());
	}
	lv_label_set_text(ipaddress, buf);
	lv_label_set_text(mask, WiFi.subnetMask().toString().c_str());
	lv_label_set_text(gateway, WiFi.gatewayIP().toString().c_str());
	IPAddress DNS0 = WiFi.dnsIP();
	IPAddress DNS1 = WiFi.dnsIP(1);
	if (static_cast<uint32_t>(DNS1) == 0) {
		snprintf(buf, sizeof(buf), "%s", DNS0.toString().c_str());
	} else {
		snprintf(buf, sizeof(buf), "%s, %s", DNS0.toString().c_str(), DNS1.toString().c_str());
	}
	lv_label_set_text(dns, buf);
}

////////////////////////

NetworkSettingsWindow::NetworkSettingsWindow(Window *parent_): Window(parent_) {
	scr = lv_win_create(lv_scr_act(), 40);
	lv_win_add_title(scr, "Network Settings");

	lv_obj_t *btn = lv_win_add_btn(scr, LV_SYMBOL_CLOSE, 40);
	lv_obj_add_event_cb(btn, [](lv_event_t *e) {
		sound.addNote(500, 50);
		Screen.closeKeyboard();
		Window::registry("options")->removeChild("config");
	}, LV_EVENT_CLICKED, NULL);

	lv_obj_t *cont = lv_win_get_content(scr);

	static const lv_coord_t column_dsc[] = {100, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
	static const lv_coord_t row_dsc[] = {28, 28, 28, 28, 28, 28, 28, 28, 28, LV_GRID_TEMPLATE_LAST};

	lv_obj_set_grid_dsc_array(cont, column_dsc, row_dsc);
	lv_obj_set_layout(cont, LV_LAYOUT_GRID);

	int row = 0;

	lv_obj_t *label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "SSID");
	ssid = lv_textarea_create(cont);
	lv_textarea_set_one_line(ssid, true);
	lv_obj_set_width(ssid, screenWidth - 110);
	lv_textarea_set_text(ssid, WifiSSID.c_str());
	lv_obj_set_grid_cell(ssid, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_obj_add_event_cb(ssid, ScreenClass::textFieldEvent, LV_EVENT_ALL, this);
	lv_obj_add_flag(ssid, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Password");
	password = lv_textarea_create(cont);
	lv_textarea_set_one_line(password, true);
	lv_obj_set_width(password, screenWidth - 110);
	lv_textarea_set_text(password, WifiPassword.c_str());
	lv_obj_set_grid_cell(password, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_obj_add_event_cb(password, ScreenClass::textFieldEvent, LV_EVENT_ALL, this);
	lv_obj_add_flag(password, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Use DHCP");
	DHCPSwitch = lv_switch_create(cont);
	lv_obj_set_grid_cell(DHCPSwitch, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	if (DHCPMode) {
		lv_obj_add_state(DHCPSwitch, LV_STATE_CHECKED);
	}
	lv_obj_add_event_cb(DHCPSwitch, [](lv_event_t *e) {
		NetworkSettingsWindow *p = reinterpret_cast<NetworkSettingsWindow *>(lv_event_get_user_data(e));
		p->updateStates();
	}, LV_EVENT_CLICKED, this);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "IP Addr");
	ipaddress = lv_textarea_create(cont);
	lv_obj_set_grid_cell(ipaddress, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_textarea_set_one_line(ipaddress, true);
	lv_obj_set_width(ipaddress, screenWidth - 110);
	lv_obj_add_event_cb(ipaddress, ScreenClass::textFieldEvent, LV_EVENT_ALL, this);
	lv_textarea_set_accepted_chars(ipaddress, "1234567890.");
	lv_textarea_set_max_length(ipaddress, 15);
	lv_textarea_set_text(ipaddress, DHCPMode && WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString().c_str() : StaticIP.toString().c_str());
	lv_obj_add_flag(ipaddress, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Netmask");
	mask = lv_textarea_create(cont);
	lv_obj_set_grid_cell(mask, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_textarea_set_one_line(mask, true);
	lv_obj_set_width(mask, screenWidth - 110);
	lv_obj_add_event_cb(mask, ScreenClass::textFieldEvent, LV_EVENT_ALL, this);
	lv_textarea_set_accepted_chars(mask, "1234567890.");
	lv_textarea_set_max_length(mask, 15);
	lv_textarea_set_text(mask, DHCPMode && WiFi.status() == WL_CONNECTED ? WiFi.subnetMask().toString().c_str() : StaticMask.toString().c_str());
	lv_obj_add_flag(mask, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "Gateway");
	gateway = lv_textarea_create(cont);
	lv_obj_set_grid_cell(gateway, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_textarea_set_one_line(gateway, true);
	lv_obj_set_width(gateway, screenWidth - 110);
	lv_obj_add_event_cb(gateway, ScreenClass::textFieldEvent, LV_EVENT_ALL, this);
	lv_textarea_set_accepted_chars(gateway, "1234567890.");
	lv_textarea_set_max_length(gateway, 15);
	lv_textarea_set_text(gateway, DHCPMode && WiFi.status() == WL_CONNECTED ? WiFi.gatewayIP().toString().c_str() : StaticGateway.toString().c_str());
	lv_obj_add_flag(gateway, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "DNS Svr 1");
	dns1 = lv_textarea_create(cont);
	lv_obj_set_grid_cell(dns1, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_textarea_set_one_line(dns1, true);
	lv_obj_set_width(dns1, screenWidth - 110);
	lv_obj_add_event_cb(dns1, ScreenClass::textFieldEvent, LV_EVENT_ALL, this);
	lv_textarea_set_accepted_chars(dns1, "1234567890.");
	lv_textarea_set_max_length(dns1, 15);
	lv_textarea_set_text(dns1, DHCPMode && WiFi.status() == WL_CONNECTED ? WiFi.dnsIP(0).toString().c_str() : StaticDNS[0].toString().c_str());
	lv_obj_add_flag(dns1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	label = lv_label_create(cont);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
	lv_label_set_text(label, "DNS Svr 2");
	dns2 = lv_textarea_create(cont);
	lv_obj_set_grid_cell(dns2, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	lv_textarea_set_one_line(dns2, true);
	lv_obj_set_width(dns2, screenWidth - 110);
	lv_obj_add_event_cb(dns2, ScreenClass::textFieldEvent, LV_EVENT_ALL, this);
	lv_textarea_set_accepted_chars(dns2, "1234567890.");
	lv_textarea_set_max_length(dns2, 15);
	lv_textarea_set_text(dns2, DHCPMode && WiFi.status() == WL_CONNECTED ? WiFi.dnsIP(1).toString().c_str() : StaticDNS[1].toString().c_str());
	lv_obj_add_flag(dns2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	updateStates();

	saveBtn = lv_btn_create(cont);
	lv_obj_set_grid_cell(saveBtn, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row++, 1);
	label = lv_label_create(saveBtn);
	lv_label_set_text(label, "Save");

	lv_obj_add_event_cb(saveBtn, [](lv_event_t *e) {
		NetworkSettingsWindow *p = reinterpret_cast<NetworkSettingsWindow *>(lv_event_get_user_data(e));
		p->saveSettings();
		sound.addNote(500, 50);
		sound.addNote(1000, 50);
		Screen.closeKeyboard();
		Window::registry("options")->removeChild("config");
	}, LV_EVENT_CLICKED, this);
}

void NetworkSettingsWindow::saveSettings() {
	WifiSSID = lv_textarea_get_text(ssid);
	WifiPassword = lv_textarea_get_text(password);
	DHCPMode = lv_obj_has_state(DHCPSwitch, LV_STATE_CHECKED);
	StaticIP.fromString(lv_textarea_get_text(ipaddress));
	StaticMask.fromString(lv_textarea_get_text(mask));
	StaticGateway.fromString(lv_textarea_get_text(gateway));
	StaticDNS[0].fromString(lv_textarea_get_text(dns1));
	StaticDNS[1].fromString(lv_textarea_get_text(dns2));
	WiFi.disconnect();
	IniFile::save(SETTINGS_FILENAME);
}

void NetworkSettingsWindow::updateStates() {
	if (lv_obj_has_state(DHCPSwitch, LV_STATE_CHECKED)) {
		lv_obj_add_state(ipaddress, LV_STATE_DISABLED);
		lv_obj_add_state(mask, LV_STATE_DISABLED);
		lv_obj_add_state(gateway, LV_STATE_DISABLED);
		lv_obj_add_state(dns1, LV_STATE_DISABLED);
		lv_obj_add_state(dns2, LV_STATE_DISABLED);
	} else {
		lv_obj_clear_state(ipaddress, LV_STATE_DISABLED);
		lv_obj_clear_state(mask, LV_STATE_DISABLED);
		lv_obj_clear_state(gateway, LV_STATE_DISABLED);
		lv_obj_clear_state(dns1, LV_STATE_DISABLED);
		lv_obj_clear_state(dns2, LV_STATE_DISABLED);
	}
}

void NetworkSettingsWindow::kbDone(lv_event_t *e) {
//    WifiSSID = lv_textarea_get_text(ssid);
//    WifiPassword = lv_textarea_get_text(password);
//	WiFi.disconnect();
//	IniFile::save(SETTINGS_FILENAME);

}

lv_keyboard_mode_t NetworkSettingsWindow::kbMode(lv_event_t *e) {
	lv_obj_t *t = lv_event_get_target(e);
	if ((t == ssid) || (t == password)) {
		return LV_KEYBOARD_MODE_TEXT_LOWER;
	} else {
		return LV_KEYBOARD_MODE_NUMBER;
	}
}

bool NetworkSettingsWindow::kbValidate(lv_event_t *e) {
	lv_obj_t *t = lv_event_get_target(e);
	if ((t != ssid) && (t != password)) {
		const char *v = lv_textarea_get_text(t);
		int part = -1;
		int dotCount = 0;
		for (int i = 0; v[i]; i++) {
			if (isdigit(v[i])) {
				if (part == -1) {
					part = 0;
				}
				part = part * 10 + v[i] - '0';
			} else if (v[i] == '.') {
				if ((part < 0) || (part > 255)) {
					return false;
				}
				if (++dotCount == 4) {
					return false;
				}
				part = -1;
			} else {
				return false;
			}
		}
		if ((part < 0) || (part > 255) || (dotCount !=3)) {
			return false;
		}
		return true;
	}
	return true;
}

void NetworkSettingsWindow::kbPostValidate(lv_event_t *) {
	if (saveBtn) {
		if ((lv_obj_has_state(ipaddress, LV_STATE_USER_1)) || 
			(lv_obj_has_state(mask, LV_STATE_USER_1)) || 
			(lv_obj_has_state(gateway, LV_STATE_USER_1)) || 
			(lv_obj_has_state(dns1, LV_STATE_USER_1)) || 
			(lv_obj_has_state(dns2, LV_STATE_USER_1))) {
			lv_obj_add_state(saveBtn, LV_STATE_DISABLED);
		} else {
			lv_obj_clear_state(saveBtn, LV_STATE_DISABLED);
		}
	}
}
