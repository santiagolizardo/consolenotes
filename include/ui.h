#pragma once

typedef struct {
	unsigned int w, h;
} Dimension;

typedef struct {
	unsigned int x, y;
} Point;

void resize_handler(int sig);

