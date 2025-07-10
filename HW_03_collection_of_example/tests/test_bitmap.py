#!/usr/bin/env python3

import os

MODULE_NAME = "ex_bitmap"
SYSFS_PATH = f"/sys/module/{MODULE_NAME}/parameters"
SET_PATH = os.path.join(SYSFS_PATH, "set_slot")
CLEAR_PATH = os.path.join(SYSFS_PATH, "clear_slot")
COUNT_PATH = os.path.join(SYSFS_PATH, "count_slots")
SHOW_PATH = os.path.join(SYSFS_PATH, "show_slots")

def write_param(path, value):
    with open(path, "w") as f:
        f.write(str(value))

def read_param(path):
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
    
    # Начальное состояние
    write_param(CLEAR_PATH, 4)
    write_param(CLEAR_PATH, 7)
    write_param(CLEAR_PATH, 63)

    assert_eq(read_param(COUNT_PATH), "0", "Начальное количество слотов")
    assert_eq(read_param(SHOW_PATH), "0" * 64, "Начальное состояние битовой карты")

    # Установка битов
    write_param(SET_PATH, 4)
    write_param(SET_PATH, 7)
    write_param(SET_PATH, 63)

    assert_eq(read_param(COUNT_PATH), "3", "Установлено 3 слота")

    show = read_param(SHOW_PATH)
    expected = list("0" * 64)
    expected[63 - 4] = "1"
    expected[63 - 7] = "1"
    expected[0] = "1"  # бит 63 — самый левый (обратный порядок)

    assert_eq(show, "".join(expected), "Состояние битовой карты после установки")

    # Очистка одного слота
    write_param(CLEAR_PATH, 7)
    assert_eq(read_param(COUNT_PATH), "2", "Очистка одного слота")

    show = read_param(SHOW_PATH)
    expected[63 - 7] = "0"
    assert_eq(show, "".join(expected), "Состояние битовой карты после очистки")

if __name__ == "__main__":
    main()
