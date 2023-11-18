FROM ubuntu:22.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    python3 python3-pip libgl1-mesa-dev libegl1-mesa-dev libx11-dev gcovr &&\
    python3 -m pip install build glcontext numpy pyopengl pytest scipy
WORKDIR /app
COPY . .
RUN python3 -m build --no-isolation
ENV MODERNGL_COVERAGE=yes LIBGL_ALWAYS_SOFTWARE=1
RUN python3 setup.py build_ext --inplace && cp build/temp.*/src/moderngl.gcno .
CMD python3 -X dev -m pytest -s -vvv tests && gcovr
