#include "raylib.h"
#include "Application.h"

int main(int argc, char* argv[])
{
	{
		Application app(320, 180,4);
		app.Run();
	}

	return 0;
}