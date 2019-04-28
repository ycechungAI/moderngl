from moderngl import new as mgl

ctx = mgl.create_context(standalone=True)
print(mgl.hwinfo(ctx))
print(mgl.extensions(ctx))

