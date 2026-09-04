#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int check_key(char *input) {
    if (strlen(input) != 8) {
        return 0;
    }
    
    int sum = 0;
    for (int i = 0; i < 8; i++) {
        sum += (int)input[i];
    }
    
    if (sum == 0x2A0 && input[0] == 'C' && input[7] == 'K') {
        return 1;
    }
    
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <license_key>\n", argv[0]);
        return 1;
    }
    
    if (check_key(argv[1])) {
        printf("Access Granted! License Key Validated.\n");
    } else {
        printf("Access Denied! Invalid Key.\n");
    }
    
    return 0;
}
