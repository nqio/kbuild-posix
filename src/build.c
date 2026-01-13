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
  int seek = 0;

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

    assert(size_index > 0); // means the file has been made
    char command[100];
    sprintf(command, "nasm -f bin %s -o %s.bin", argv[i], argv[i]);

    if (system(command) != 0) {
      return EXIT_FAILURE;
    }

    struct stat st;
    char bin_filename[strlen(argv[i]) + 4 + 1];
    sprintf(bin_filename, "%s.bin", argv[i]);
    stat(bin_filename, &st);
    long long size = st.st_size;

    sprintf(command, "dd if=%s.bin of=%s bs=512 seek=%d conv=notrunc", argv[i],
            argv[filename_index], seek);
    seek += size / 512;
    if (size % 512 != 0) {
      seek++;
    }

    remove(bin_filename);
  }
}
