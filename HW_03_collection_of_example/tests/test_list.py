#!/usr/bin/env python3

import random

MODULE_NAME = "ex_list"
SYSFS_BASE = f"/sys/module/{MODULE_NAME}/parameters"

def write_param(name, value):
    path = f"{SYSFS_BASE}/{name}"
    with open(path, "w") as f:
        f.write(f"{value}\n")

def read_param(name):
    path = f"{SYSFS_BASE}/{name}"
    with open(path, "r") as f:
        return f.read().strip()

def assert_eq(actual, expected, desc):
    if actual == expected:
        print(f"[PASS] {desc}")
    else:
        print(f"[FAIL] {desc}")
        print(f"       Expected: {expected}")
        print(f"       Actual:   {actual}")

def main():
    print(f"------------------------------------ Тест модуля {MODULE_NAME} ------------------------------------")
    values = random.sample(range(1, 10_000), 100)
    for v in values:
        write_param("new_element", str(v))

    all_elements = read_param("all_elements")
    expected_all = ",".join(str(v) for v in values) + "\n"
    assert_eq(all_elements, expected_all.strip(), "All elements in list")

    first = read_param("get_first")
    assert_eq(first, str(values[0]), "First element")

    last = read_param("get_last")
    assert_eq(last, str(values[len(values) - 1]), "Last element")
if __name__ == "__main__":
    main()
