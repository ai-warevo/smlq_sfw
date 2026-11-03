from pathlib import Path

def prepend_lora_caption(target_dir='.', extension='txt', prefix='smlq_sfw'):
    """
    Добавляет строку `prefix, ` в самое начало каждого файла с расширением extension
    в указанной директории target_dir, если этот префикс еще не добавлен.
    """
    directory = Path(target_dir)
    
    if not directory.exists() or not directory.is_dir():
        print(f"-----\r\nОШИБКА: Директория '{target_dir}' не найдена!\r\n-----\r\n\r\n")
        return

    # Шаблон префикса с запятой и пробелом
    full_prefix = f"{prefix}, "
    glob_pattern = f"*.{extension.lstrip('.')}"

    for file_path in directory.glob(glob_pattern):
        try:
            old_content = file_path.read_text(encoding='utf-8')
        except Exception as e:
            print(f"-----\r\nОШИБКА чтения файла {file_path.name}: {e}\r\n-----\r\n\r\n")
            continue
            
        # Защита от дублирования: если файл уже начинается с этого префикса, пропускаем
        if old_content.startswith(full_prefix):
            continue

        # Формируем новый контент: префикс + старый текст
        new_content = f"{full_prefix}{old_content}"
        
        try:
            file_path.write_text(new_content, encoding='utf-8')
        except Exception as e:
            print(f"-----\r\nОШИБКА записи файла {file_path.name}: {e}\r\n-----\r\n\r\n")
            continue
        
        # Вывод лога "до" и "после"
        log_message = (
            f"-----\r\n"
            f"файл {file_path.name}\r\n"
            f"---\r\n"
            f"{old_content.rstrip()}\r\n"
            f"---\r\n"
            f"{new_content.rstrip()}\r\n"
            f"-----\r\n\r\n"
        )
        print(log_message, end='')

if __name__ == '__main__':
    # Запуск в текущей папке с вашим префиксом по умолчанию
    prepend_lora_caption(target_dir='.', extension='txt', prefix='smlq_sfw')
