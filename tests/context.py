import os
import struct

import moderngl as mgl
import numpy as np
import pytest


@pytest.fixture(scope='module')
def ctx():
    context = mgl.create_context(standalone=True, debug=True)

    if os.getenv('TRAVIS') == 'True':
        import gltraces
        mgl_procs = np.frombuffer(mgl.glprocs(context), dtype='u%d' % struct.calcsize('P'))
        mgl_procs[gltraces.lookup['glBufferStorage']] = 0
        mgl_procs[gltraces.lookup['glClearBufferData']] = 0

    return context
