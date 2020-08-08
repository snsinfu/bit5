#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>


// getopt_long(3) is a GNU extension but is fairly common.


struct options {
    int width;
    int height;
    double scale;
    double fps;
    const char *title;
    const char *filename;
};

static void show_usage(FILE *out);
static void parse_options(int argc, char **argv, struct options *options);
static void parse_size(const char *str, int *width, int *height);
static void parse_scale(const char *str, double *val);
static void parse_fps(const char *str, double *val);
static void errexit_usage(const char *msg);


int
main(int argc, char **argv)
{
    struct options options = {
        .width = 500,
        .height = 500,
        .scale = 1,
        .fps = 60,
    };

    parse_options(argc, argv, &options);

    printf("width   \t%d\n", options.width);
    printf("height  \t%d\n", options.height);
    printf("scale   \t%g\n", options.scale);
    printf("fps     \t%g\n", options.fps);
    printf("title   \t%s\n", options.title);
    printf("filename\t%s\n", options.filename);
}

static void
show_usage(FILE *out)
{
    static const char *usage =
        "usage: frag [-sxfth] <source>\n"
        "\n"
        "  <source>  filename of fragment shader\n"
        "\n"
        "options:\n"
        "  -s, --size <width>,<height>  surface size\n"
        "  -x, --scale <scale>          display scale\n"
        "  -f, --fps <fps>              frames per second\n"
        "  -t, --title <title>          window title\n"
        "  -h, --help                   show this help message and exit\n"
        "\n";

    fputs(usage, out);
}

static void
parse_options(int argc, char **argv, struct options *options)
{
    static const struct option longopts[] = {
        { "size", required_argument, NULL, 's' },
        { "scale", required_argument, NULL, 'x' },
        { "fps", required_argument, NULL, 'f' },
        { "title", required_argument, NULL, 't' },
        { "help", no_argument, NULL, 'h' },
    };
    static const char *shortopts = "s:x:f:t:h";

    for (int opt; (opt = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1; ) {
        switch (opt) {
        case 's':
            parse_size(optarg, &options->width, &options->height);
            break;

        case 'x':
            parse_scale(optarg, &options->scale);
            break;

        case 'f':
            parse_fps(optarg, &options->fps);
            break;

        case 't':
            options->title = optarg;
            break;

        case 'h':
            show_usage(stdout);
            exit(EXIT_SUCCESS);

        default:
            errexit_usage(NULL);
        }
    }

    argc -= optind;
    argv += optind;

    if (argc != 1) {
        errexit_usage("missing <source> argument");
    }

    options->filename = argv[0];
}

static void
parse_size(const char *str, int *width, int *height)
{
    if (sscanf(str, "%d,%d", width, height) != 2) {
        errexit_usage("bad size argument");
    }
}

static void
parse_scale(const char *str, double *val)
{
    char *end;
    *val = strtod(str, &end);

    if (*end != '\0') {
        errexit_usage("bad size argument");
    }
}

static void
parse_fps(const char *str, double *val)
{
    char *end;
    *val = strtod(str, &end);

    if (*end != '\0') {
        errexit_usage("bad fps argument");
    }
}

static void
errexit_usage(const char *msg)
{
    if (msg) {
        fprintf(stderr, "error: %s\n", msg);
    }
    fprintf(stderr, "use --help to see usage\n");
    exit(EXIT_FAILURE);
}
