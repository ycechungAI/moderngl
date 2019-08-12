import decimal

import moderngl


def fmt(x):
    x = moderngl.pack([x], 'f2')
    t = ('0' * 16 + bin(int.from_bytes(x, 'little'))[2:])[-16:]
    return t[0 : 1] + ' ' + t[1 : 6] + ' ' + t[6 : 16]


def test_values():
    assert fmt(1.0) == '0 01111 0000000000'
    assert fmt(1.5) == '0 01111 1000000000'
    assert fmt(-1.0) == '1 01111 0000000000'
    assert fmt(-1.5) == '1 01111 1000000000'


def test_zero():
    assert fmt(0.0) == '0 00000 0000000000'
    assert fmt(+0.0) == '0 00000 0000000000'
    assert fmt(-0.0) == '1 00000 0000000000'


def test_nan():
    assert fmt(float('nan')) == '0 11111 0000000001'
    assert fmt(float('+nan')) == '0 11111 0000000001'
    assert fmt(float('-nan')) == '1 11111 0000000001'


def test_inf():
    assert fmt(float('inf')) == '0 11111 0000000000'
    assert fmt(float('+inf')) == '0 11111 0000000000'
    assert fmt(float('-inf')) == '1 11111 0000000000'


def test_half_pi():
    assert fmt(3.1415926535897932384626433832795028841971693993751) == '0 10000 1001001000'


def test_half_e():
    assert fmt(2.7182818284590452353602874713526624977572470936999) == '0 10000 0101110000'


def test_rounding():
    assert fmt(0.99999) == fmt(1.0)
    assert fmt(1e-16) == fmt(0.0)
