#pragma once

#include <string>
#include <vector>
#include <lvgl.h>

class Window;


struct ChildWindow {
	std::string name;
	Window *child;
};

class Window {
public:
	Window();
	Window(Window *parent_);
	virtual ~Window();

	void addChild(const char *name, Window *child);
	void removeChild(const char *name);

	virtual void update();
	virtual void fastUpdate();
	virtual void kbDone(lv_event_t *e);
	virtual lv_keyboard_mode_t kbMode(lv_event_t *e);
	virtual bool kbValidate(lv_event_t *e);
	virtual void kbPostValidate(lv_event_t *e);


	lv_obj_t *getLvObj();

	static void registerWindow(const char *name, Window *window);
	static Window *registry(const char *name);

protected:
	lv_obj_t *scr;
	Window *parent = nullptr;

private:
	std::vector<ChildWindow> children; 

	static std::vector<ChildWindow> registryStore;
};
