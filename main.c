#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int M = 0;
int N = 0;
int K = 0;
int Q = 0;
int** S;
int number_pts = 0;
int number_games = 0;
int* starting_pts;
int* visited;
int turn_count = 0;
int play_game = 0;

int get_max_depth(int depth, int s_pt) {
    int max_depth = depth;
    visited[s_pt-1] = 1;
    int j;
    for (j=0; j < M; j++) {
        if (s_pt == S[j][0]) {
            int m_depth = get_max_depth(depth+1, S[j][1]);
            if (m_depth > max_depth) max_depth = m_depth;
        }
    }
    return max_depth;
}

int calculate_total_moves(int* starting_pts, int s_pts_size) {
    int total_moves = 0;
    int i;
    for(i=0; i < s_pts_size; i++) {
        int s_pt = starting_pts[i];
        visited = (int*) malloc(sizeof(int) * N);
        int ii;
        for (ii=0; ii<N; ii++)
            visited[ii] = 0;
        total_moves += get_max_depth(0, s_pt);
    }
    return total_moves;
}

int main() {

    #define CHUNK 1024 /* read 1024 bytes at a time */
    char buf[CHUNK];
    FILE *file;
    size_t nread;

    file = fopen("input.txt", "r");
    if (file) {
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
            char* pch = NULL;
            char *save_ptr1, *save_ptr2;
            pch = strtok_r(buf, "\r\n", &save_ptr1);
            int line = 1;
            while (pch != NULL) {

                int first = NULL;
                int second = NULL;
                int first_assigned = 0;

                char *cpy;
                cpy = malloc(sizeof(char) * strlen(pch));
                strcpy(cpy, pch);
                char *p = strtok_r(cpy, " ", &save_ptr2);

                while (p != NULL) {

                    if (line == 1) {
                        if (M == 0) {
                            M = atoi(p);
                        } else if (N == 0) {
                            N = atoi(p);
                            S = (int**)malloc(M * sizeof(int*));
                        }
                    } else if (line > 1 && line <= M+1) {
                        if (first_assigned == 0) {
                            first = atoi(p);
                            first_assigned = 1;
                        } else if (first_assigned == 1) {
                            second = atoi(p);
                            S[line-2] = (int*)malloc(sizeof(int) * 2);
                            S[line-2][0] = first;
                            S[line-2][1] = second;
                        }
                    } else if (line == M+2) {
                        number_games = atoi(p);
                    } else if (line > M+2) {
                        if (!play_game) {
                            number_pts = atoi(p);
                            starting_pts = (int*)malloc(sizeof(int)*number_pts);
                            turn_count = 0;
                            play_game = 1;
                        } else {
                            starting_pts[turn_count] = atoi(p);
                            turn_count++;
                        }
                    }
                    p = strtok_r(NULL, " ", &save_ptr2);
                }

                if (line > M+2 && play_game == 1 && turn_count) {

                    char * winner = "Iroh";
                    if (calculate_total_moves(starting_pts, number_pts) % 2 == 1) {
                        winner = "Bumi";
                    }

                    printf("%s\n", winner);

                    play_game = 0;
                }

                int n_spaces = 0;
                pch = strtok_r(NULL, "\r\n", &save_ptr1);
                line++;
            }
        }

        fclose(file);
    }

    //printf("M is %d\n", M);
    //printf("N is %d\n", N);
    //int v = 0;
    //for (v=0; v<M; v++)
    //    printf("S is %d %d\n", S[v][0], S[v][1]);

    return 0;
}
