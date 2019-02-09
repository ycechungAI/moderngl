set -e -x
cd /io

for PYBIN in /opt/python/cp3*/bin; do
    "${PYBIN}/python" setup.py bdist_wheel
done

for whl in dist/*.whl; do
    auditwheel repair "$whl" -w /io/dist
done
