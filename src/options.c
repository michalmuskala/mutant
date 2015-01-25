#include "options.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef unix
/* We're on unix - use standard getopt */
#include <unistd.h>

#else
/* It's windows - roll out our own */

char *optarg = NULL;
int optind = 0;
int optopt = 0;

int
getopt(int argc, char **argv, char *optstring)
{
    static char *next = NULL;
    char *cp = NULL;

    if (optind == 0) {
        next = NULL;
    }

    optarg = NULL;

    if (next == NULL || *next == '\0') {
        if (optind == 0) {
            optind++;
        }

        if ((optind >= argc) ||
            (argv[optind][0] != '-') ||
            (argv[optind][1] == '\0')) {
            optarg = NULL;

            if (optind < argc) {
                optarg = argv[optind];
            }

            return -1;
        }

        if (strncmp(argv[optind], "--", 2) == 0) {
            optind++;
            optarg = NULL;

            if (optind < argc) {
                optarg = argv[optind];
            }

            return -1;
        }

        next = argv[optind];
        next++; /* Skip past - */
        optind++;
    }

    optopt = *next++;
    cp = strchr(optstring, optopt);

    if (cp == NULL || optopt == ':') {
        return '?';
    }

    cp++;

    if (*cp == ':') {
        if (*next != '\0') {
            optarg = next;
            next = NULL;
        }
        else if (optind < argc) {
            optarg = argv[optind];
            optind++;
        }
        else {
            return '?';
        }
    }

    return optopt;
}

#endif /* __GNUC__ */

/* Arguments defaults */
#define MAX_TRIANGLES 50
#define ADDITION_CHANCE 10
#define MUTATION_CHANCE 20
#define DELETION_CHANCE 30
#define SWAP_CHANCE 40
#define DELAY 1000

/* Easy printing */
#define TOS(val) _TOS(val)
#define _TOS(val) #val

static void
print_option(const char *c, const char *def, const char *desc)
{
    fprintf(stderr, "%-8s %-12s %s\n", c, def, desc);
}

static void
usage(const char *name)
{
    fprintf(stderr, "Usage: %s -f <image> [options]\n", name);
    fprintf(stderr, "\n");
    print_option("Option", "Default", "Description");
    print_option("-f", "(required)", "image to process");
    print_option("-m", TOS(MAX_TRIANGLES), "max number of triangles to use");
    print_option("-a", TOS(ADDITION_CHANCE), "triangle addition chance");
    print_option("-m", TOS(MUTATION_CHANCE), "triangle mutation chance");
    print_option("-d", TOS(DELETION_CHANCE), "triangle deletion chance");
    print_option("-s", TOS(SWAP_CHANCE), "triangles swap chance");
    print_option("-t", TOS(DELAY), "delay before quitting in ms");
    fprintf(stderr, "All chances are expressed as 1 in value provided\n");
}

static void
version()
{
    fprintf(stderr, "Mutant 1.0\n");
    fprintf(stderr, "Copyright (C) 2015 Michał Muskała\n");
    fprintf(stderr, "License MIT: <url>\n");
    fprintf(stderr, "Please report bugs to: <url>\n");
}

Options *options = NULL;

Options *
parse_options(int argc, char **argv)
{
    int c = 0;
    char *opts = "hvf:n:a:m:d:s:t:";
    char *argopts = "fnamdst";

    if (options != NULL) {
        fprintf(stderr, "Overriding options\n");
        free_options();
    }

    options = malloc(sizeof(*options));

    if (options == NULL) {
        perror("Parsing options");
        return NULL;
    }

    /* default values */
    options->image = NULL;
    options->max_triangles = 50;
    options->addition_chance = 10;
    options->mutation_chance = 20;
    options->deletion_chance = 30;
    options->swap_chance = 40;
    options->delay = 1000;

    while((c = getopt(argc, argv, opts)) != -1) {
        switch (c) {
        case 'f':
            options->image = optarg;
            break;
        case 'n':
            options->max_triangles = atoi(optarg);
            break;
        case 'a':
            options->addition_chance = atoi(optarg);
            break;
        case 'm':
            options->mutation_chance = atoi(optarg);
            break;
        case 'd':
            options->deletion_chance = atoi(optarg);
            break;
        case 's':
            options->swap_chance = atoi(optarg);
            break;
        case 't':
            options->delay = atoi(optarg);
            break;
        case 'h':
            usage(argv[0]);
            free_options();
            return NULL;
        case 'v':
            version();
            free_options();
            return NULL;
        case '?':
            if (strchr(argopts, optopt) != NULL) {
                free_options();
                fprintf(stderr, "Option -%c requires an argument\n", optopt);
                usage(argv[0]);
                return NULL;
            }
            else {
                free_options();
                fprintf(stderr, "Unkown option -%c\n", optopt);
                usage(argv[0]);
                return NULL;
            }
        default:
            abort();
        }
    }

    if (options->image == NULL) {
        free_options();
        usage(argv[0]);
        return NULL;
    }

    return options;
}

void
free_options()
{
    if (options == NULL) {
        return;
    }

    free(options);
}