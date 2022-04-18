#include "Chip8.h"
#include "Platform.h"


int main(int argc, char** argv)
{
	if (argc != 4)
	{
		std::cout << std::format("Usage: {} <Scale> <Delay> <ROM>\n", argv[0]);
		std::exit(EXIT_FAILURE);
	}

	int videoScale = std::stoi(argv[1]);
	int cycleDelay = std::stoi(argv[2]);
	char const* romFilename = argv[3];

	Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

	Chip8 chip8;
	chip8.LoadROM(romFilename);

	int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	//std::thread worker(&Chip8::registerPresentState, &chip8);

	while (!quit)
	{
		quit = platform.ProcessInput(chip8.keypad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::milli>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			chip8.Cycle();
			platform.Update(chip8.video, videoPitch);
		}
	}
	/*chip8.stillPlaying = false;
	worker.join();*/
	return 0;
}

