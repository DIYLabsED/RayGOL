#include <stdio.h>
#include <string.h>

#include <raylib.h>


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

#define GOL_WIDTH_END 800

#define NUM_CELLS_X 50
#define NUM_CELLS_Y 50


const Color LIVE_CELL_COLOR = {229, 202, 149, 255};
const Color DEAD_CELL_COLOR = {8, 8, 22, 255};
const Color GRID_COLOR      = {50, 50, 50, 255};

int cells[NUM_CELLS_X][NUM_CELLS_Y];
int nextCells[NUM_CELLS_X][NUM_CELLS_Y];

float cellWidth = (float)GOL_WIDTH_END / NUM_CELLS_X;
float cellHeight = (float)WINDOW_HEIGHT / NUM_CELLS_Y;

int paused = 1;
float updateTimer = 0;
float updateInterval = 0.05f;

int drawGrid = 1;


int countAliveNeighbors(int x, int y){

	int count = 0;

	for(int dX = -1; dX <= 1; dX++){
		for(int dY = -1; dY <= 1; dY++){

			if(dX == 0 && dY == 0) continue;

			int wX = (x + dX + NUM_CELLS_X) % NUM_CELLS_X;
			int wY = (y + dY + NUM_CELLS_Y) % NUM_CELLS_Y;
			count += cells[wX][wY];

		}


	}

	return count;

}

void randomizeCells(){

	for(int x = 0; x < NUM_CELLS_X; x++){
		for(int y = 0; y < NUM_CELLS_Y; y++){
			cells[x][y] = GetRandomValue(0, 4) == 0 ? 1 : 0;
		}
	}

}

void clearCells(){

	for(int x = 0; x < NUM_CELLS_X; x++){
		for(int y = 0; y < NUM_CELLS_Y; y++){
			cells[x][y] = 0;
		}
	}

}

void handleInput(){

	// would be convenient if I could do ternary this to avoid duplicate
	if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){

		Vector2 mousePos = GetMousePosition();
		int cellX = (int)(mousePos.x / cellWidth);
		int cellY = (int)(mousePos.y / cellHeight);
		cells[cellX][cellY] = 1;

	}

	if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){

		Vector2 mousePos = GetMousePosition();
		int cellX = (int)(mousePos.x / cellWidth);
		int cellY = (int)(mousePos.y / cellHeight);
		cells[cellX][cellY] = 0;

	}

	float wheelMove = GetMouseWheelMove();
	if(wheelMove){
		updateInterval -= wheelMove * 0.0005f;
		if(updateInterval < 0.001f) updateInterval = 0.001f;
		if(updateInterval > 0.5f) updateInterval = 0.5f;		
	}

	if(IsKeyPressed(KEY_SPACE)){

		if(paused) paused = 0;
		else paused = 1;

	}

	if(IsKeyPressed(KEY_C)){
		clearCells();
	}

	if(IsKeyPressed(KEY_R)){
		randomizeCells();
	}

	if(IsKeyPressed(KEY_G)){

		if(drawGrid) drawGrid = 0;
		else drawGrid = 1;

	}

}

void drawCells(){

	for(int x = 0; x < NUM_CELLS_X; x++){
		for(int y = 0; y < NUM_CELLS_Y; y++){
			if(cells[x][y]){

				DrawRectangle((int)(x * cellWidth),	(int)(y * cellHeight), (int)cellWidth, (int)cellHeight,	LIVE_CELL_COLOR);

			}
		}
	}

	
	if(drawGrid){

		for(int x = 0; x < NUM_CELLS_X; x++){
			DrawLine(x * cellWidth, 0, x * cellWidth, GOL_WIDTH_END, GRID_COLOR);			
		}

		for(int y = 0; y < NUM_CELLS_Y; y++){
			DrawLine(0, y * cellHeight, GOL_WIDTH_END, y * cellHeight, GRID_COLOR);			
		}

	}

}


int main(){

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayGOL");
	SetTargetFPS(60);

	randomizeCells();

	while(!WindowShouldClose()){

		handleInput();

		if(!paused){

			updateTimer += GetFrameTime();
			if(updateTimer >= updateInterval){

				updateTimer = 0;

				for(int x = 0; x < NUM_CELLS_X; x++){
					for(int y = 0; y < NUM_CELLS_Y; y++){

						int numNeighbors = countAliveNeighbors(x, y);
						int alive = cells[x][y];

						if(alive && (numNeighbors < 2 || numNeighbors > 3)){
							nextCells[x][y] = 0;

						}
						else if(!alive && numNeighbors == 3){
							nextCells[x][y] = 1;
						}
						else{
							nextCells[x][y] = alive;
						}


					}

				}

				memcpy(cells, nextCells, sizeof(cells));

			}

		}

		BeginDrawing();
		ClearBackground(DEAD_CELL_COLOR);
		drawCells();
		EndDrawing();

	}

	CloseWindow();

	return 0;

}