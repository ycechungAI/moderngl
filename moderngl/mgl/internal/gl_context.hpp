#pragma once

struct GLContext {
	void * display;
	void * window;
	void * context;
	void * old_context;
	void * old_window;
	void * old_display;
	bool standalone;

	bool load(bool standalone);
	void enter();
	void exit();
	void destroy();
};
