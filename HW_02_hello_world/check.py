#!/usr/bin/env python3

import subprocess
import os
import sys

def run_command(cmd, check_output=False, ignore_errors=False):
    """Выполнить команду shell"""
    try:
        if check_output:
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True, check=True)
            return result.stdout.strip()
        else:
            result = subprocess.run(cmd, shell=True, check=True)
            return None
    except subprocess.CalledProcessError as e:
        if ignore_errors:
            return None
        print(f"Command failed: {cmd}")
        print(f"Error: {e}")
        sys.exit(1)

def read_param(param_name):
    """Прочитать параметр модуля"""
    return run_command(f"cat /sys/module/hello_mod/parameters/{param_name}", check_output=True)

def write_param(param_name, value):
    """Записать параметр модуля"""
    run_command(f"sudo sh -c 'echo {value} > /sys/module/hello_mod/parameters/{param_name}'")

def assert_equals(expected, actual, message):
    """Простой ассерт"""
    if expected == actual:
        print(f"✓ PASS: {message}")
    else:
        print(f"✗ FAIL: {message}")
        print(f"  Expected: '{expected}'")
        print(f"  Actual:   '{actual}'")
        sys.exit(1)

def assert_file_exists(filepath, message):
    """Проверить существование файла"""
    if os.path.exists(filepath):
        print(f"✓ PASS: {message}")
    else:
        print(f"✗ FAIL: {message} - File {filepath} does not exist")
        sys.exit(1)

def cleanup():
    """Очистка модуля"""
    run_command("sudo rmmod hello_mod", ignore_errors=True)

def main():
    print("=== Starting module test ===")
    
    # Очистка и подготовка
    print("\n1. Cleanup and build...")
    cleanup()
    run_command("make clean")
    run_command("bear -- make")
    
    # Загрузка модуля
    print("\n2. Loading module...")
    run_command("sudo insmod hello_mod.ko")
    
    # Проверка существования параметров
    print("\n3. Checking parameter files exist...")
    assert_file_exists("/sys/module/hello_mod/parameters/my_str", "my_str parameter exists")
    assert_file_exists("/sys/module/hello_mod/parameters/idx", "idx parameter exists") 
    assert_file_exists("/sys/module/hello_mod/parameters/ch_val", "ch_val parameter exists")
    
    # Проверка значений по умолчанию
    print("\n4. Checking default values...")
    default_str = read_param("my_str")
    default_idx = read_param("idx")
    default_ch = read_param("ch_val")
    
    print(f"Initial my_str: '{default_str}'")
    print(f"Initial idx: '{default_idx}'")
    print(f"Initial ch_val: '{default_ch}'")
    
    assert_equals("0", default_idx, "Default idx value is 0")
    # Предполагаем что default ch_val это пробел или пустая строка
    
    # Первый тест: установка значений
    print("\n5. Setting new values (test 1)...")
    write_param("idx", "5")
    write_param("ch_val", ".")
    
    # Проверка результатов
    new_str = read_param("my_str")
    new_idx = read_param("idx") 
    new_ch = read_param("ch_val")
    
    print(f"After test 1 - my_str: '{new_str}'")
    print(f"After test 1 - idx: '{new_idx}'")
    print(f"After test 1 - ch_val: '{new_ch}'")
    
    assert_equals("5", new_idx, "idx set to 5")
    assert_equals(".", new_ch, "ch_val set to '.'")
    # my_str должен остаться неизменным (только чтение)
    assert_equals("Hello. World!", new_str, "string has been changed")
    
    # Второй тест: изменение значений
    print("\n6. Setting new values (test 2)...")
    write_param("idx", "0")
    write_param("ch_val", "h")
    
    # Проверка результатов
    final_str = read_param("my_str")
    final_idx = read_param("idx")
    final_ch = read_param("ch_val")
    
    print(f"After test 2 - my_str: '{final_str}'")
    print(f"After test 2 - idx: '{final_idx}'")
    print(f"After test 2 - ch_val: '{final_ch}'")
    
    assert_equals("0", final_idx, "idx reset to 0")
    assert_equals("h", final_ch, "ch_val set to 'h'")
    assert_equals("hello, World!", final_str, "String has been changed again")
    
    # Проверка dmesg на наличие сообщений колбэков (опционально)
    print("\n7. Checking dmesg for callback messages...")
    try:
        dmesg_output = run_command("dmesg | tail -10", check_output=True)
        if "idx set to:" in dmesg_output:
            print("✓ PASS: Found idx callback messages in dmesg")
        else:
            print("⚠ WARNING: No idx callback messages found in dmesg")
    except:
        print("⚠ WARNING: Could not check dmesg")
    
    # Очистка
    print("\n8. Cleanup...")
    cleanup()
    
    print("\n=== All tests passed! ===")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nTest interrupted by user")
        cleanup()
        sys.exit(1)
    except Exception as e:
        print(f"\nUnexpected error: {e}")
        cleanup()
        sys.exit(1)