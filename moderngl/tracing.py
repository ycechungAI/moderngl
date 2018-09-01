class Tracer:
    callback = None


def tracer(callback):
    Tracer.callback = callback
