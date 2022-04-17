/**
 * Author RogerYu, Written in 2022/04/16
 */

#include "DisplayEventTracker.h"

using namespace android;

static int mVsyncRate = 1;

void usage(const char *me)
{
    printf("\nusage: \t%s [options]\n"
            "\t------------------------------------ options ---------------------------------------------\n"
            "\t[-i] vsync rate(default:1)\n"
            "\t[-h] help\n"
            "\tinput 'r' to call requestNextVsync or input 'q' to exit when running\n"
            "\t------------------------------------------------------------------------------------------\n",
            me);
   exit(1);
}

void parseOptions(int argc, char **argv)
{
    const char *me = argv[0];
    int res;
    while((res = getopt(argc, argv, "hi:")) >= 0) {
        switch(res) {
            case 'i':
                mVsyncRate = atoi(optarg);
                break;
            case 'h':
            default:
                usage(me);
        }
    }
}


int main(int argc, char ** argv)
{
    parseOptions(argc, argv);
    sp<DisplayEventTracker> displayEventTracker = new DisplayEventTracker(mVsyncRate);
    displayEventTracker->run("DisplayEventTracker", PRIORITY_DISPLAY);

    do {
        char input = getchar();
		if(input == 'q')
			break;
        if(input == 'r')
            displayEventTracker->requestNextVsync();
            
    } while(1);

    return 0;
}
