#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os

MODULE_NAME = "ex_rb_tree"
SYSFS_BASE = f"/sys/module/{MODULE_NAME}/parameters"

ADD_PATH            = os.path.join(SYSFS_BASE, "add_node")
SHOW_PATH           = os.path.join(SYSFS_BASE, "show_tree")
SEARCH_PATH         = os.path.join(SYSFS_BASE, "search_data")
SEARCH_RESULT_PATH  = os.path.join(SYSFS_BASE, "search_data_result")


def _write(path: str, value: int | str) -> None:
    """Записать value в sysfs-параметр"""
    with open(path, "w") as f:
        f.write(f"{value}\n")


def _read(path: str) -> str:
    """Считать sysfs-параметр без завершающих переводов строки"""
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
    # 1. Дерево при загрузке должно быть пустым
    # ────────────────────────────────────────────────────────────────────────────────
    _assert_eq(_read(SHOW_PATH), "", "Пустое дерево после загрузки модуля")

    # ────────────────────────────────────────────────────────────────────────────────
    # 2. Добавление набора значений
    # ────────────────────────────────────────────────────────────────────────────────
    values = [50, 20, 70, 10, 30]
    print("[+] Добавление узлов:", ", ".join(map(str, values)))
    for v in values:
        _write(ADD_PATH, v)

    expected_tree = ",".join(map(str, sorted(values)))
    _assert_eq(_read(SHOW_PATH), expected_tree, "Корректный in-order-вывод после вставки")

    # ────────────────────────────────────────────────────────────────────────────────
    # 3. Повторная вставка существующего значения не должна изменять дерево
    # ────────────────────────────────────────────────────────────────────────────────
    _write(ADD_PATH, values[1])               # дублируем «20»
    _assert_eq(_read(SHOW_PATH), expected_tree, "Дублирующая вставка не меняет дерево")

    # ────────────────────────────────────────────────────────────────────────────────
    # 4. Поиск присутствующего и отсутствующего значения
    # ────────────────────────────────────────────────────────────────────────────────
    _write(SEARCH_PATH, 30)
    _assert_eq(_read(SEARCH_RESULT_PATH), "found", "Поиск существующего узла (30)")

    _write(SEARCH_PATH, 42)
    _assert_eq(_read(SEARCH_RESULT_PATH), "not found", "Поиск отсутствующего узла (42)")

    # ────────────────────────────────────────────────────────────────────────────────
    # 5. Вставка отрицательных чисел (проверка диапазона значений)
    # ────────────────────────────────────────────────────────────────────────────────
    negatives = [-5, -15]
    for v in negatives:
        _write(ADD_PATH, v)

    full_sorted = sorted(values + negatives)
    _assert_eq(_read(SHOW_PATH), ",".join(map(str, full_sorted)),
               "In-order-вывод с отрицательными значениями")


if __name__ == "__main__":
    main()
