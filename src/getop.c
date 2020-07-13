#include <getopt.h>
#include <unistd.h>

void getop(int argc, char* argv[]) {
    const char* optstring = "vn:h";
    int arg;
    struct option opts[] = {
        {"version", 0, NULL, 'v'},
        {"name", 1, NULL, 'n'},
        {"help", 0, NULL, 'h'}};
    while ((arg = getopt_long(argc, argv, optstring, opts, NULL)) != -1) {
        switch (arg) {
            case 'n':
                printf("username is %s\n", optarg);
                break;
            case 'v':
                printf("version is 0.0.1\n");
                break;
            case 'h':
                printf("this is help\n");
                break;
            case '?':
                printf("unknown option\n");
                break;
            case 0:
                printf("the return val is 0\n");
                break;
            default:
                printf("------\n");
        }
    }
}
