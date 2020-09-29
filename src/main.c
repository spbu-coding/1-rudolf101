#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_STRING_LENGTH 32
#define error(...) (fprintf(stderr, __VA_ARGS__))

struct interval_t {
    double border_left, border_right;
};

int read_interval(struct interval_t *interval) {
    if (printf("Enter the left border:") < 0) {
        error("Cannot write to stdout\n");
        exit(0);
    }
    if (scanf("%lf", &(interval->border_left)) != 1) {
        error("Cannot read the left border\n");
        exit(0);
    }
    if (interval->border_left < 0) {
        error("The left border cannot be less than zero\n");
        exit(0);
    }
    if (printf("Enter the right border:") < 0) {
        error("Cannot write to stdout\n");
        exit(0);
    }
    if (scanf("%lf", &(interval->border_right)) != 1) {
        error("Cannot read the right border\n");
        exit(0);
    }
    if (interval->border_right > M_PI) {
        error("The right border cannot be more than PI\n");
        exit(0);
    }
    if (interval->border_left > interval->border_right) {
        error("The left border cannot be more than right border\n");
        exit(0);
    }
    return 0;
}

void free_result(char **arr, const unsigned int n) {
    for (int i = 0; i < n; ++i) {
        free(arr[i]);
    }
    free(arr);
}

double rectangle_integral(struct interval_t *interval, const unsigned int *partitions) {
    double step = (interval->border_right - interval->border_left) / *partitions;
    double left = interval->border_left, right = left + step;
    double result = 0;
    for (int i = 0; i < *partitions; ++i) {
        result += sin((left + right) / 2) * step;
        left = right;
        right += step;
    }
    return result;
}

double simpson_integral(struct interval_t *interval, const unsigned int *partitions) {
    double step = (interval->border_right - interval->border_left) / *partitions;
    double left = interval->border_left, right = left + step;
    double result = 0;
    for (int i = 0; i < *partitions; ++i) {
        result += (right - left) / 6 * (sin(left) + 4 * sin((left + right) / 2) + sin(right));
        left = right;
        right += step;
    }
    return result;
}

char **get_integrals(struct interval_t *interval, const unsigned int *number_of_actions, unsigned int *partitions) {
    char **answer = (char **) malloc(sizeof(char *) * *number_of_actions);
    if (!answer) {
        free_result(answer, *number_of_actions);
        error("Cannot allow memory for all answer array\n");
        exit(0);
    }
    for (int i = 0; i < *number_of_actions; ++i) {
        answer[i] = (char *) malloc(sizeof(char) * MAX_STRING_LENGTH);
        if (!answer[i]) {
            free_result(&answer[i], i);
            error("Cannot allow memory for answer[%i] array\n", i);
            exit(0);
        }
        if (!sprintf(answer[i], "%d %.5f %.5f", partitions[i], rectangle_integral(interval, &partitions[i]),
                     simpson_integral(interval, &partitions[i]))) {
            error("Cannot write the answer[%i]\n", i);
            exit(0);
        }
    }
    return answer;
}

int main() {
    struct interval_t interval;
    read_interval(&interval);
    unsigned int partitions[] = {6, 10, 20, 100, 500, 1000};
    unsigned int number_of_actions = sizeof(partitions) / sizeof(partitions[0]);
    char **answer = get_integrals(&interval, &number_of_actions, partitions);
    if (!answer) {
        error("Cannot write the answer\n");
        exit(0);
    }
    for (int i = 0; i < number_of_actions; ++i) {
        if (printf("%s\n", answer[i]) < 0) {
            error("Cannot write the answer to stdout in line: %d\n", i);
            exit(0);
        }
    }
    free_result(answer, number_of_actions);
    return 0;
}