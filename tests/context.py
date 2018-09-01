import struct

import numpy as np
import moderngl as mgl
import pytest


@pytest.fixture(scope='module')
def ctx():
    context = mgl.create_context(standalone=True, debug=True)
    procs = np.frombuffer(mgl.glprocs(context), dtype='u%d' % struct.calcsize('P'))
    procs[534] = 0 # disable glBufferStorage
    procs[491] = 0 # disable glClearBufferData
    return context
