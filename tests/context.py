import os
import struct

import moderngl as mgl
import pytest


@pytest.fixture(scope='module')
def ctx():
    glhook = None
    if os.getenv('TRAVIS') == 'True':
        def glhook(glprocs, dtype):
            from gltraces import lookup, null
            glprocs[lookup['glBufferStorage']] = null
            glprocs[lookup['glClearBufferData']] = null

    context = mgl.create_context(standalone=True, glhook=glhook)

    return context
