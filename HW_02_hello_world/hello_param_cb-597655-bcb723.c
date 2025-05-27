#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>

static int my_param = 0; // Переменная для хранения значения параметра

//---------------------------------------
// Callback-функции для обработки параметра
//---------------------------------------

// Функция вызывается при чтении параметра (например, через /sys/module/module_name/parameters/my_param)
static int param_get(char *buffer, const struct kernel_param *kp) {
    return param_get_int(buffer, kp); // Стандартный вывод для типа int
}

// Функция вызывается при записи параметра (например, через insmod или sysfs)
static int param_set(const char *val, const struct kernel_param *kp) {
    int res;
    int new_value;

    // Преобразуем строку в число
    res = kstrtoint(val, 10, &new_value);
    if (res) {
        pr_err("Ошибка: неверное значение параметра!\n");
        return res;
    }

    // Пример проверки значения
    if (new_value < 0 || new_value > 100) {
        pr_err("Ошибка: значение должно быть от 0 до 100!\n");
        return -EINVAL;
    }

    // Обновляем значение
    my_param = new_value;

    // Дополнительная логика (например, вывод в лог)
    pr_info("Параметр изменен на: %d\n", my_param);
    return 0;
}

// Структура с callback-функциями
static const struct kernel_param_ops param_ops = {
    .set = param_set,    // Функция при записи
    .get = param_get,    // Функция при чтении
};

// Объявляем параметр с callback
module_param_cb(my_param, &param_ops, &my_param, 0644);
MODULE_PARM_DESC(my_param, "Пример параметра с callback (диапазон: 0-100)");

//---------------------------------------
// Инициализация и выход
//---------------------------------------
static int __init mod_init(void) {
    pr_info("Модуль загружен. Текущее значение параметра: %d\n", my_param);
    return 0;
}

static void __exit mod_exit(void) {
    pr_info("Модуль выгружен\n");
}

module_init(mod_init);
module_exit(mod_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("OldALoneFag");
MODULE_DESCRIPTION("A simple param-callback module for the Linux kernel");