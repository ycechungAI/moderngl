__all__ = ['ConditionalRender']


class ConditionalRender:
    def __init__(self):
        self.mglo = None
        raise TypeError()

    def __repr__(self):
        return '<ConditionalRender>'

    def __enter__(self):
        self.mglo.begin_render()
        return self

    def __exit__(self, *args):
        self.mglo.end_render()
