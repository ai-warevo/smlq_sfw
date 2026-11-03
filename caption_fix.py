from pathlib import Path

def process_text_files(target_dir='.', extension='txt', min_lines=2):
    """
    Находит все файлы с указанным extension в target_dir, 
    проверяет, что в них не менее min_lines строк, 
    объединяет ВСЕ строки через запятую и логирует результат.
    """
    directory = Path(target_dir)
    
    if not directory.exists() or not directory.is_dir():
        print(f"-----\r\nОШИБКА: Директория '{target_dir}' не найдена!\r\n-----\r\n\r\n")
        return

    glob_pattern = f"*.{extension.lstrip('.')}"

    for file_path in directory.glob(glob_pattern):
        try:
            old_content = file_path.read_text(encoding='utf-8')
        except Exception as e:
            print(f"-----\r\nОШИБКА чтения файла {file_path.name}: {e}\r\n-----\r\n\r\n")
            continue
            
        lines = [line.strip() for line in old_content.splitlines()]
        
        # Проверка кастомного лимита строк
        if len(lines) < min_lines:
            error_message = (
                f"-----\r\n"
                f"ОШИБКА в файле {file_path.name}\r\n"
                f"В файле {len(lines)} стр. вместо требуемых {min_lines}!\r\n"
                f"---\r\n"
                f"{old_content.rstrip()}\r\n"
                f"-----\r\n\r\n"
            )
            print(error_message, end='')
            continue

        # Логика склейки ВСЕХ строк
        processed_lines = []
        for i, line in enumerate(lines):
            # Пропускаем пустые строки, если они вдруг есть между текстом
            if not line:
                continue
                
            # Для всех строк, кроме первой: убираем точку в начале, если она есть
            if i > 0 and line.startswith('.'):
                line = line[1:].strip()
                
            processed_lines.append(line)
        
        # Соединяем все обработанные строки через запятую с пробелом
        new_content = ", ".join(processed_lines)
        
        try:
            file_path.write_text(new_content, encoding='utf-8')
        except Exception as e:
            print(f"-----\r\nОШИБКА записи файла {file_path.name}: {e}\r\n-----\r\n\r\n")
            continue
        
        # Вывод лога строго по вашему формату
        log_message = (
            f"-----\r\n"
            f"файл {file_path.name}\r\n"
            f"---\r\n"
            f"{old_content.rstrip()}\r\n"
            f"---\r\n"
            f"{new_content}\r\n"
            f"-----\r\n\r\n"
        )
        print(log_message, end='')

if __name__ == '__main__':
    # Склеит ВСЕ строки во всех .txt файлах текущей папки, если в файле минимум 2 строки
    process_text_files(target_dir='.', extension='txt', min_lines=2)
