#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int ipow (int begin, int power) {
  int result = begin; // 10
  for (int i = 1; i < power; ++i) {
    result *= begin;
  }
}

int stoi(char *str) {
  int result = 0;
  int i = 0;
  while (str[i] != '\0') {
    result += (str[i] - '0') * ipow(10, i);
    i++;
  }

  return result;
}

int main(int argc, char **argv) {
  int filename_index = 0;
  int size_index = 0;
  system("cat src/kernel.asm src/*[^kernel].asm > kernel.asm.cat");

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-o") == 0) {
      assert(argc > i);
      i++;
      filename_index = i;
      continue;
    }

    if (strcmp(argv[i], "-s") == 0) {
      assert(argc > i);
      i++;
      size_index = i;
      assert(filename_index > 0);

      char command[100];
      sprintf(command, "dd if=/dev/zero of=%s bs=512 count=%d",
              argv[filename_index], stoi(argv[size_index]));

      if (system(command) != 0) {
        return EXIT_FAILURE;
      }

      continue;
    }
  }

  assert(size_index > 0);
  
  if (system("nasm -f bin boot.asm -o boot.bin") != 0) {
    return EXIT_FAILURE;
  }
  
  if (system("nasm -f bin kernel.asm.cat -o kernel.bin.cat") != 0) {
    return EXIT_FAILURE;
  }
  char command[200];
  sprintf(command, "dd if=boot.bin of=%s bs=512 seek=0 conv=notrunc", arg[filename_index]);
  if (system(command) != 0) {
    return EXIT_FAILURE;
  }
  
  sprintf(command, "dd if=kernel.bin.cat of=%s bs=512 seek=1 conv=notrunc", arg[filename_index]);
  if (system(command) != 0) {
    return EXIT_FAILURE;
  }
  
  remove("src/kernel.asm.cat");
  remove("src/kernel.bin.cat");
  remove("boot.bin");
}
