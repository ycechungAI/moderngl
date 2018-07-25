import json

import moderngl

ctx = moderngl.create_context(standalone=True, debug=True)

print('Limits:', json.dumps(ctx.limits.json(), indent=2))
print('Extensions:', json.dumps(moderngl.extensions(ctx), indent=2))
print('Hardware Info:', json.dumps(moderngl.hwinfo(ctx), indent=2))
print('Version Code:', ctx.version_code)
