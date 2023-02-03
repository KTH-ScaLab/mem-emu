// SPDX-License-Identifier: LGPL-2.1-only

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

struct sample {
    float localPercent;
    float localGB;
    float remoteGB;
    float totalGB;
    float time;
};

bool parse_sample(const char *buf, struct sample *sample, float time_off)
{
    int m = sscanf(buf,
            "emu: local%% %f localGB %f remoteGB %f totalGB %f time %f",
            &sample->localPercent, &sample->localGB, &sample->remoteGB,
            &sample->totalGB, &sample->time);
    if (m == 5) {
	    sample->time -= time_off;
	    return true;
    } else {
	    return false;
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s log1 log2 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = argc - 1;
    char buf[2048];
    FILE **files = malloc(n * sizeof(FILE *));
    float *time_off = malloc(n * sizeof(float));
    struct sample *samples = malloc(n * sizeof(struct sample));
    bool *eof = malloc(n * sizeof(bool));

    for (int i = 0; i < n; i++) {
        samples[i].time = -INFINITY;
        eof[i] = false;
    }

    for (int i = 0; i < n; i++) {
        files[i] = fopen(argv[i+1], "r");
        if (!files[i]) {
            snprintf(buf, sizeof(buf), "error: opening file '%s'",
                    argv[i+1]);
            perror(buf);
            exit(EXIT_FAILURE);
        }
        if (!fgets(buf, sizeof(buf), files[i])) {
            snprintf(buf, sizeof(buf), "error: reading file '%s'",
                    argv[i+1]);
            perror(buf);
            exit(EXIT_FAILURE);
        }
        // If processes run on different nodes the clocks need to be synced
        if (sscanf(buf, "emu: sync %f", time_off+i) != 1) {
            snprintf(buf, sizeof(buf), "error: reading sync line of '%s'",
                    argv[i+1]);
            perror(buf);
            exit(EXIT_FAILURE);
        }
    }

    printf("agg: %d files\n", n);

    float center = 0;
    int collected = 0;
    int neof = 0;

    // Samples may be missing from some processes main due to mlock at the
    // start or due to finishing earlier.
    // Find 1-second intervals where every process has been sampled:
    while (neof < n) {
        for (int i = 0; i < n; i++) {
            if (eof[i])
                continue;

            for (;;) {
                float delta = center - samples[i].time;
                if (delta > 0.5) {
                    // We are behind, get next sample
                    if (!fgets(buf, sizeof(buf), files[i])) {
                        eof[i] = true;
                        neof++;
                        break;
                    }

                    if (!parse_sample(buf, samples+i, time_off[i]))
                        printf("%s", buf);

                    continue;
                } else if (delta < -0.5) {
                    // We are ahead, move interval forward
                    center = samples[i].time;
                    collected = 0;
                }

                collected++;
                break;
            }

            if (collected == n) {
                collected = 0;
                center += 1;

                struct sample agg = {};
                for (int j = 0; j < n; j++) {
                    agg.localPercent += samples[j].localPercent;
                    agg.localGB += samples[j].localGB;
                    agg.remoteGB += samples[j].remoteGB;
                    agg.totalGB += samples[j].totalGB;
                    agg.time += samples[j].time;
                }
                agg.localPercent /= n;
                agg.time /= n;
                float var = 0;
                for (int j = 0; j < n; j++) {
                    float d = agg.localPercent-samples[j].localPercent;
                    var += d*d;
                }
                float stddev = sqrtf(var / (n-1));
                printf("emu: local%% %3.2f %.2f localGB %.2f remoteGB %.2f totalGB %.2f time %.2f\n",
                        agg.localPercent, stddev, agg.localGB, agg.remoteGB, agg.totalGB, agg.time);
            }
        }
    }
}
