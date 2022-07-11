#include <gtkmm.h>
#include <gtkmm/application.h>

#include "Window.h"

int main(int argc, char** argv) {
    auto app = Gtk::Application::create(argc, argv, "wiktoria.debowska");
    MyWindow window;
	return app->run(window);

}
