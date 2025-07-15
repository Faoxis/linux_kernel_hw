# Makefile для коллекции примеров структур данных ядра Linux

# Директории
SRC_DIR := src
BUILD_DIR := build
KERNEL_DIR := /lib/modules/$(shell uname -r)/build

# Модули для сборки
MODULES := ex_list ex_queue ex_bitmap ex_rb_tree ex_bin_search

# Цели по умолчанию
.PHONY: all clean format 

# Основная цель - сборка всех модулей
all: $(BUILD_DIR) $(addprefix $(BUILD_DIR)/, $(addsuffix .ko, $(MODULES)))

# Создание директории build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Сборка модулей
$(BUILD_DIR)/%.ko: $(SRC_DIR)/%.c $(BUILD_DIR)
	@echo "Сборка модуля $*..."
	@cp $(SRC_DIR)/$*.c $(BUILD_DIR)/
	@echo "obj-m += $*.o" > $(BUILD_DIR)/Makefile
	@$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR)/$(BUILD_DIR) modules
	@echo "Модуль $*.ko успешно собран"

# Очистка
clean:
	@echo "Очистка директории $(BUILD_DIR)..."
	@rm -rf $(BUILD_DIR)
	@echo "Очистка завершена"

# Тестирование
check:
	@sudo python3 tests/test_list.py
	@sudo python3 tests/test_queue.py
	@sudo python3 tests/test_bitmap.py
	@sudo python3 tests/test_rb_tree.py
	@sudo python3 tests/test_bin_search.py

# Загрузка модулей
load: all
	@echo "Загрузка модулей..."
	@for module in $(MODULES); do \
		if [ -f "$(BUILD_DIR)/$$module.ko" ]; then \
			echo "Загрузка $$module.ko..."; \
			sudo insmod $(BUILD_DIR)/$$module.ko; \
		else \
			echo "Модуль $$module.ko не найден"; \
		fi; \
	done
	@echo "Загрузка завершена"

# Выгрузка модулей
unload:
	@echo "Выгрузка модулей..."
	@for module in $(MODULES); do \
		if lsmod | grep -q "^$$module "; then \
			echo "Выгрузка $$module..."; \
			sudo rmmod $$module; \
		else \
			echo "Модуль $$module не загружен"; \
		fi; \
	done
	@echo "Выгрузка завершена"

# Автоформатирование исходников
format:
	@echo "Форматирование исходников в $(SRC_DIR)..."
	@find $(SRC_DIR) -name "*.c" -exec clang-format -i {} \;
	@echo "Форматирование завершено"

