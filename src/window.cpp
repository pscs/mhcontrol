#include "window.h"

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

void Window::update() {
	for (const auto w: children) {
		w.child->update();
	}
}

void Window::fastUpdate() {
	for (const auto w: children) {
		w.child->fastUpdate();
	}
}

void Window::kbDone(lv_event_t *) {
}

lv_keyboard_mode_t Window::kbMode(lv_event_t *e) {
	return LV_KEYBOARD_MODE_TEXT_LOWER;
}

bool Window::kbValidate(lv_event_t *e) {
	return true;
}

void Window::kbPostValidate(lv_event_t *e) {
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