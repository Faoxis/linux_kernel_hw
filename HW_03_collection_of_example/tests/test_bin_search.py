#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os

MODULE_NAME = "ex_bin_search"
SYSFS_BASE = f"/sys/module/{MODULE_NAME}/parameters"

WRITE_PATH = os.path.join(SYSFS_BASE, "write_param")
READ_PATH  = os.path.join(SYSFS_BASE, "read_param")


def _write(path: str, value: str) -> None:
    """Записать строку в sysfs-параметр"""
    with open(path, "w") as f:
        f.write(f"{value}\n")


def _read(path: str) -> str:
    """Считать sysfs-параметр, убирая перенос строки"""
    with open(path, "r") as f:
        return f.read().strip()


def _assert_eq(actual: str, expected: str, desc: str) -> None:
    if actual == expected:
        print(f"[PASS] {desc}")
    else:
        print(f"[FAIL] {desc}")
        print(f"       Expected: {expected}")
        print(f"       Actual:   {actual}")


def main() -> None:
    print(f"--------------------------- Тест модуля {MODULE_NAME} ---------------------------")

    # ────────────────────────────────────────────────────────────────────────────────
    # 1. Простая сортировка положительных чисел
    # ────────────────────────────────────────────────────────────────────────────────
    input_data = "5,2,9,1,3"
    expected_sorted = "1,2,3,5,9"
    _write(WRITE_PATH, input_data)
    _assert_eq(_read(READ_PATH), expected_sorted, "Сортировка положительных чисел")

    # ────────────────────────────────────────────────────────────────────────────────
    # 2. Сортировка с отрицательными числами
    # ────────────────────────────────────────────────────────────────────────────────
    input_data = "-4,3,0,-1,2"
    expected_sorted = "-4,-1,0,2,3"
    _write(WRITE_PATH, input_data)
    _assert_eq(_read(READ_PATH), expected_sorted, "Сортировка с отрицательными числами")

    # ────────────────────────────────────────────────────────────────────────────────
    # 3. Повторяющиеся значения
    # ────────────────────────────────────────────────────────────────────────────────
    input_data = "3,1,2,3,2"
    expected_sorted = "1,2,2,3,3"
    _write(WRITE_PATH, input_data)
    _assert_eq(_read(READ_PATH), expected_sorted, "Сортировка с дубликатами")

    # ────────────────────────────────────────────────────────────────────────────────
    # 4. Один элемент
    # ────────────────────────────────────────────────────────────────────────────────
    input_data = "42"
    expected_sorted = "42"
    _write(WRITE_PATH, input_data)
    _assert_eq(_read(READ_PATH), expected_sorted, "Один элемент")

    # ────────────────────────────────────────────────────────────────────────────────
    # 5. Пустой ввод (некорректный случай)
    # ────────────────────────────────────────────────────────────────────────────────
    try:
        _write(WRITE_PATH, "")
        result = _read(READ_PATH)
        _assert_eq(result, "[]", "Пустой ввод возвращает []")
    except Exception as e:
        print(f"[PASS] Пустой ввод вызвал ожидаемую ошибку: {e}")


if __name__ == "__main__":
    main()
