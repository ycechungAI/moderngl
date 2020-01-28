from window import Example, run_example


class EmptyWindow(Example):
    gl_version = (3, 3)
    title = "Empty Window"
    window_size = (1280, 720)
    aspect_ratio = 16 / 9
    resizable = True

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def render(self, time: float, frame_time: float):
        self.ctx.clear(0.2, 0.4, 0.7)

    def key_event(self, key, action):
        """
        Handle key events in a generic way
        supporting all window types.
        """
        if action == self.wnd.keys.ACTION_PRESS:
            if key == self.wnd.keys.SPACE:
                print("Space was pressed")

        if action == self.wnd.keys.ACTION_RELEASE:
            if key == self.wnd.keys.SPACE:
                print("Space was released")

    def mouse_position_event(self, x, y):
        """
        Mouse position reported in pixel position
        with the upper left corner as the origin
        """
        print("Mouse pos", x, y)

    def mouse_press_event(self, x, y, button):
        """Reports left and right mouse button presses + position"""
        if button == 1:
            print("Left mouse button pressed @", x, y)
        if button == 2:
            print("Right mouse button pressed @", x, y)

    def mouse_release_event(self, x, y, button):
        """Reports left and right mouse button releases + position"""
        if button == 1:
            print("Left mouse button released @", x, y)
        if button == 2:
            print("Right mouse button released @", x, y)


if __name__ == '__main__':
    run_example(EmptyWindow)
