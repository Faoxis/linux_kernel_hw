#!/usr/bin/env python3

import os

MODULE_NAME = "ex_queue"
SYSFS_PATH = f"/sys/module/{MODULE_NAME}/parameters"
INPUT_PATH = os.path.join(SYSFS_PATH, "input")
OUTPUT_PATH = os.path.join(SYSFS_PATH, "output")
SIZE_PATH = os.path.join(SYSFS_PATH, "size")

def write_input(value):
    with open(INPUT_PATH, "w") as f:
        f.write(str(value))

def read_output():
    with open(OUTPUT_PATH, "r") as f:
        return f.read().strip()

def read_size():
    with open(SIZE_PATH, "r") as f:
        return int(f.read().strip())

def assert_eq(actual, expected, desc):
    if actual == expected:
        print(f"[PASS] {desc}")
    else:
        print(f"[FAIL] {desc}")
        print(f"       Expected: {expected}")
        print(f"       Actual:   {actual}")

def main():
    print(f"------------------------------------ Тест модуля {MODULE_NAME} ------------------------------------")
    assert_eq(read_size(), 0, "Очередь: начальный размер")

    print("[+] Подача чисел 10, 20, 30")
    write_input(10)
    write_input(20)
    write_input(30)

    assert_eq(read_size(), 3, "Размер очереди после ввода")

    print("[>] Чтение значений:")
    assert_eq(read_output(), str(10), "Чтение первого значения");
    assert_eq(read_output(), str(20), "Чтение второго значения");
    assert_eq(read_output(), str(30), "Чтение третьего значения");
    assert_eq(read_output(), "(empty)", "Выход за границу");

    assert_eq(read_size(), 0, "Финальный размер")


if __name__ == "__main__":
    main()
