double* s(size_t n, double* p) {
    double** matrix = (double**)malloc(n * sizeof(double*));
    matrix[0] = (double*)malloc(sizeof(double));
    matrix[0][0] = 1;
    size_t k, m = 0;
    do {
        matrix[m+1] = (double*)malloc((m+1) * sizeof(double));
        matrix[m+1][0] = matrix[m][0] * (1 - p[m]);
        matrix[m+1][m+1] = matrix[m][m] * p[m];
        k = 1;
        do {
            matrix[m+1][k+1] = matrix[m][k] * p[m] + matrix[m][k+1] * (1 - p[m]);
        } while (k++ < m);
    } while (m++ < n);
}

double R(size_t n, size_t r, double* p) {
    double* s_matrix = s(n, p);
    double result = 0;
    do {
        result += s_matrix[n][r];
    } while(++r < n);
    free(s_matrix);
    return result;
}

