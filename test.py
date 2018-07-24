import moderngl

ctx = moderngl.create_context(standalone=True, debug=True)
print(ctx)
print(ctx.limits.json())
print(moderngl.extensions(ctx))
print(ctx.version_code)
