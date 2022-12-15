

def test_1(ctx):
    tex = ctx.texture((5, 10), 4)
    ext = ctx.external_texture(
        glo=tex.glo,
        size=(5, 10),
        components=4,
        samples=0,
        dtype="f1",
    )
    ext.size == (5, 10)
    ext.width == 5
    ext.height == 10
    ext.components == 4
    ext.glo == tex.glo
    ext.dtype == "f1"
    ext.samples == 0
    ext.repeat_x is True
    ext.repeat_y is True
    ext.filter == (ctx.LINEAR, ctx.LINEAR)
    ext.swizzle == "RGBA"
    ext.depth is False
    ext.anisotropy == 1.0
    ext.mglo != tex.mglo
    ext.ctx = ctx
    ext.extra == {}
