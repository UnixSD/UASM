#include <stdio.h>
#include <stdlib.h>
#include "uasm/c/preproc.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <input_file> [output_file]\n", argv[0]);
		return 1;
	}
	
	// Создаем контекст препроцессора
	PreprocContext* ctx = preproc_create_context();
	if (!ctx) {
		fprintf(stderr, "Failed to create preprocessor context\n");
		return 1;
	}
	
	// Добавляем стандартные пути для поиска include файлов
	preproc_add_include_path(ctx, ".");
	preproc_add_include_path(ctx, "include");
	
	// Открываем выходной файл
	FILE* output = stdout;
	if (argc >= 3) {
		output = fopen(argv[2], "w");
		if (!output) {
			fprintf(stderr, "Failed to open output file: %s\n", argv[2]);
			preproc_destroy_context(ctx);
			return 1;
		}
	}
	
	// Обрабатываем входной файл
	int result = preproc_process_file(ctx, argv[1], output);
	
	if (result != 0) {
		fprintf(stderr, "Preprocessing failed\n");
	}
	
	// Закрываем файлы и освобождаем ресурсы
	if (output != stdout) {
		fclose(output);
	}
	preproc_destroy_context(ctx);
	
	return result;
}
