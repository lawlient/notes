#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* Flag set by ‘--verbose’. */
static int verbose_flag;
opterr = 1;
int main(int argc, char **argv) {
    int c;
    static struct option long_options[] = {
        /* These options set a flag. */
        {"verbose1", no_argument, &verbose_flag, 1},
        {"verbose2", no_argument, &verbose_flag, 2},
        {"verbose3", no_argument, &verbose_flag, 3},
        {"brief", no_argument, &verbose_flag, 0},
        /* These options don’t set a flag.
         *              We distinguish them by their indices. */
        {"add", no_argument, 0, 'a'},
        {"append", no_argument, 0, 'b'},
        {"delete", required_argument, 0, 'd'},
        {"create", required_argument, 0, 'c'},
        {"file", required_argument, 0, 'f'},
        {0, 0, 0, 0}
    };

    /* getopt_long stores the option index here. */
    char shortOpt[512] = {0};
    int cnt = 0;
    int optsize = sizeof(long_options) / sizeof(struct option);
    for (int i = 0; i < optsize; i++) {
        if (long_options[i].val >= 'a' && long_options[i].val <= 'z') {
            shortOpt[cnt++] = long_options[i].val;
            if (long_options[i].has_arg == required_argument) {
                shortOpt[cnt++] = ':';
            } else if (long_options[i].has_arg == optional_argument) {
                shortOpt[cnt++] = ':';
                shortOpt[cnt++] = ':';
            }
        }
    }
    printf("line: [%d] shortOpt[%s]\n\n", __LINE__, shortOpt);

    int option_index = 0;
    while (1) {

        c = getopt_long(argc, argv, shortOpt, long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1) break;

        /// printf("option:%s    optarg=%s\n",long_options[option_index].name,  optarg);
        /// if (optind < argc) {
        ///     printf("next optarg=%s\n", argv[optind]);
        /// }

        switch (c) {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;

            case 'a':
                puts("option -a\n");
                break;

            case 'b':
                puts("option -b\n");
                break;

            case 'c':
                printf("option -c with value `%s'\n", optarg);
                break;

            case 'd':
                printf("option -d with value `%s'\n", optarg);
                break;

            case 'f':
                printf("option -f with value `%s'\n", optarg);
                break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                abort();
        }
    }

    /* Instead of reporting ‘--verbose’
     * and ‘--brief’ as they are encountered,
     * we report the final status resulting from them. */
    if (verbose_flag) printf("verbose flag is set:%d\n", verbose_flag);

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        putchar('\n');
    }

    exit(0);
}
