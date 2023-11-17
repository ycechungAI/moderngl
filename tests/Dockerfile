FROM python:3.12.0
RUN apt-get update && apt-get install -y libgl1-mesa-dev libx11-dev gcovr
RUN pip install -U pip wheel setuptools && pip install glcontext pytest numpy scipy
WORKDIR /app
COPY . .
ENV MODERNGL_COVERAGE=yes
RUN python setup.py build_ext --inplace && cp build/temp.*/src/moderngl.gcno .
CMD python -X dev -m pytest -s -vvv tests && gcovr
