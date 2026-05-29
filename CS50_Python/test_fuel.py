from CS50_Mini_Projects.CS50_Python.fuel import convert
from CS50_Mini_Projects.CS50_Python.fuel import gauge

def test_valid_input():
    assert convert("2/5") == 40
    assert convert("0/1") == 0
    assert convert("5/5") == 100

def test_zero_division():
    try:
        convert("1/0")
        assert False, "Expected ZeroDivisionError"
    except ZeroDivisionError:
        pass

def test_noninteger_input():
    try:
        convert("a/b")
        assert False, "Expected ValueError"
    except ValueError:
        pass

def test_negative_input():
    try:
        convert("-1/5")
        assert False, "Expected ValueError"
    except ValueError:
        pass

    try:
        convert("1/-5")
        assert False, "Expected ValueError"
    except ValueError:
        pass

def test_x_greater_than_y():
    try:
        convert("5/2")
        assert False, "Expected ValueError"
    except ValueError:
        pass

def test_gauge():
    assert gauge(75) == "75%"
    assert gauge(99) == "F"
    assert gauge(1) == "E"
    assert gauge(0) == "E"
    assert gauge(100) == "F"