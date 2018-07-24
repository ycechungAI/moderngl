import moderngl

ctx = moderngl.create_context(standalone=True, debug=True)
print(ctx)
print(ctx.limits.json())
print(moderngl.extensions(ctx))
print(moderngl.hwinfo(ctx))
print(ctx.version_code)
