import moderngl
import numpy as np

ctx = moderngl.context(standalone=True)

# reserving a buffer with 500 bytes
buf1 = ctx.buffer(reserve=500)
print(buf1.size)

# reserving 1KB using a human readable format
buf2 = ctx.buffer(reserve='1KB')
print(buf2.size)

# reserving 2MB using a human readable format
buf3 = ctx.buffer(reserve='2MB')
print(buf3.size)

# creating a buffer using a numpy array
# make sure the array contains floats and not doubles
buf4 = ctx.buffer(np.array([
    1.0, 1.0, 1.0, 1.0
], 'f4'))

print(buf4.size)
print(np.frombuffer(buf4.read(), 'f4'))

# creating a buffer using moderngl.pack
buf5 = ctx.buffer(moderngl.pack([
    1.0, 1.0, 1.0, 1.0
]))

print(buf5.size)
print(np.frombuffer(buf5.read(), 'f4'))

# creating a buffer with mixed type using moderngl.pack
buf6 = ctx.buffer(moderngl.pack([
    1.0, 1.0, 1.0, 0, True,
], layout='3f i u1'))

print(buf6.size)
print(np.frombuffer(buf6.read(), dtype='f4, f4, f4, i4, bool'))
