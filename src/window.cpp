#include "window.h"

#include <lvgl.h>
#include "logger.h"
#include <algorithm>

std::vector<ChildWindow> Window::registryStore;

Window::Window() {
}

Window::Window(Window *parent_): parent(parent_) {
}

Window::~Window() {
	for (const auto w: children) {
		delete(w.child);
	}
	if (scr) {
		lv_obj_del_async(scr);
	}
}

lv_obj_t *Window::getLvObj() {
	return scr;
}

void Window::Update() {
	for (const auto w: children) {
		w.child->Update();
	}
}

void Window::addChild(const char *name, Window *window) {
	const auto it = std::find_if(children.begin(), children.end(), [&name](const ChildWindow &r) {
		return r.name == name;
	});

	if (it == children.end()) {
		children.emplace_back(ChildWindow{name, window});
	}
}


void Window::removeChild(const char *name) {
	const auto it = std::find_if(children.begin(), children.end(), [&name](const ChildWindow &r) {
		return r.name == name;
	});

	if (it != children.end()) {
		delete it->child;
		children.erase(it);
	}
}

void Window::registerWindow(const char *name, Window *window) {
	const auto it = std::find_if(registryStore.begin(), registryStore.end(), [&name](const ChildWindow &r) {
		return r.name == name;
	});
	if (it == registryStore.end()) {
		registryStore.emplace_back(ChildWindow{name, window});
	}
}

Window *Window::registry(const char *name) {
	for (const auto w: registryStore) {
		if (w.name == name) {
			return w.child;
		}
	}
	logger.printf(LOG_GENERAL, LOG_ERROR, "Unable to find %s in Window registry\n", name);
	return nullptr;
}