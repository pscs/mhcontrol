#pragma once

#include <string>
#include <vector>

class Window;
struct _lv_obj_t;
typedef _lv_obj_t lv_obj_t;

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

	virtual void Update();

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
