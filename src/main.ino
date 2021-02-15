#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <SD.h>
#include "gameboy_prelude.h"
#include "options.h"
#include <malloc.h>
static uint pixel_size = 2;

static uint width = GAMEBOY_WIDTH * pixel_size;
static uint height = GAMEBOY_HEIGHT * pixel_size;
static std::unique_ptr<Gameboy> gameboy;

#define SD_DETECT_PIN SD_DETECT_NONE

TFT_eSPI tft = TFT_eSPI();

const int chipSelect = PB9;

File gba_rom;
File gba_game;


static uint32_t get_real_color(Color color) {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    switch (color) {
        case Color::White: r = g = b = 255; break;
        case Color::LightGray: r = g = b = 170; break;
        case Color::DarkGray: r = g = b = 85; break;
        case Color::Black: r = g = b = 0; break;
    }

    return (r << 16) | (g << 8) | (b << 0);
}


static void set_pixel(uint32_t* pixels, uint x, uint y, uint32_t pixel_argb) {
    pixels[width * y + x] = pixel_argb;
}

static void set_large_pixel(uint32_t* pixels, uint x, uint y, uint32_t pixel_argb) {
    for (uint w = 0; w < pixel_size; w++) {
        for (uint h = 0; h < pixel_size; h++) {
            set_pixel(pixels, x * pixel_size + w, y * pixel_size + h, pixel_argb);
        }
    }
}

static void set_pixels(uint32_t* pixels, const FrameBuffer& buffer) {
    for (uint y = 0; y < GAMEBOY_HEIGHT; y++) {
        for (uint x = 0; x < GAMEBOY_WIDTH; x++) {
            Color color = buffer.get_pixel(x, y);
            uint32_t pixel_argb = get_real_color(color);
            set_large_pixel(pixels, x, y, pixel_argb);
        }
    }
}

static bool is_closed() {
    return false;
}


struct CliOptions {
    Options options;
    std::string filename;
};


static std::vector<u8> state_data() {
	return {};
}

static void draw(const FrameBuffer& buffer) {
	Serial.println("DRAW CALLED!");
	for(int i = 0; i < 160; i++) {
		for(int j=0; j < 240; j++) {
			Color color = buffer.get_pixel(i, j);
			uint32_t pixel_argb = get_real_color(color);
			tft.drawPixel(i, j, pixel_argb);
		}
	}
}



void setup() {
	delay(5000);
	CliOptions opts;
	opts.filename="mario.rom";
	Serial.begin(115200);
	Serial.println("Booting...");
	pinMode(PC13, OUTPUT);
	if(!SD.begin(PB9)) {
		while(1) {
			digitalWrite(PC13, LOW);
		}
	} else {
		Serial.println("SD Read!");
	}
	tft.init();
	tft.setRotation(1);
	tft.fillScreen(TFT_BLACK);
	Serial.println("Opening bios..");
	gba_rom = SD.open("mario.gba", FILE_READ);
	std::vector<u8> rom;
	gba_rom.read(&rom, sizeof(rom));
	Serial.println("Starting emulator....");

	auto save_data = state_data();
	gameboy = std::make_unique<Gameboy>(rom, opts.options, save_data);
	Serial.println("GBA Created");
    gameboy->run(&is_closed, &draw);
	Serial.println("Started");

}

void loop() {
	for(int i = 0; i < 100; i++) {
	//	tft.drawPixel(150, i, tft.color565(100, 0, 1));
	}
}