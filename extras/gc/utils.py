"""
This modules is really unisable for ref counting. Just an experiment!
"""
import gc
import sys
import ctypes

def print_referrers(obj):
    refs = gc.get_referrers(obj)
    for ref in refs:
        if is_module(ref):
            print("-->", is_module(ref))
        else:
            print("--> ", type(ref), ref)


def is_module(obj):
    if isinstance(obj, dict) and obj.get("__name__"):
        return obj.get("__name__")
    return None

def ref_c_1(obj):
    """+ 1"""
    return ctypes.c_long.from_address(id(obj)).value

def ref_c_2(obj):
    """+ 2"""
    return sys.getrefcount(obj)
