#include "Draw.cpp"

enum Pixel_ID { null, empty, sand, stone, water };

class Pixel {
    public:
        Pixel_ID id;

        Pixel() : id(Pixel_ID::empty) {}
        Pixel(Pixel_ID id) : id(id) {}

        void GetRenderProperties() {
            switch (id)
            {
            case Pixel_ID::empty:
                Draw::SetColor(Color());
                break;
            case Pixel_ID::sand:
                Draw::SetColor(yellow);
                break;
            case Pixel_ID::stone:
                Draw::SetColor(grey);
                break;
            case Pixel_ID::water:
                Draw::SetColor(blue);
                break;
            default:
                break;
            }
        }
};

class Board {
    public:
        int pixelSize;
        IntVector2 size = IntVector2(10, 20);
        Pixel* pixels;

        Board(int pixelSize) : pixelSize(pixelSize), size(IntVector2(WIDTH, HEIGHT) / pixelSize) {}

        void Initialize() {
            pixels = new Pixel[size.x * size.y];
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    pixels[x + y * size.x] = Pixel();
                }
            }
        }

        void Draw() {
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    pixels[x + y * size.x].GetRenderProperties();
                    for (int i = 0; i < pixelSize; i++) {
                        for (int j = 0; j < pixelSize; j++) {
                            SDL_RenderDrawPoint(Draw::rendGlobal, x * pixelSize + i, HEIGHT - (y * pixelSize + j));
                        }
                    }
                }
            }
        }

        bool TestForPixel(Pixel pixel, Pixel_ID* testFor) {
            int len = sizeof(testFor) / sizeof(Pixel_ID); 
            for (int i = 0; i < len; i++) {
                if (pixel.id == testFor[i]) {
                    return true;
                }
            }
            return false;
        }

        Pixel GetPixel(int x, int y) {
            if (x < 0 || x > size.x) { return Pixel(Pixel_ID::null); }
            if (y < 0 || y > size.y) { return Pixel(Pixel_ID::null); }
            return pixels[x + y * size.x];
        }

        void SetPixel(IntVector2 set, Pixel_ID id) {
            set.y = size.y - set.y;
            if (set.x < 0 || set.x > size.x) { return; }
            if (set.y < 0 || set.y > size.y) { return; }
            pixels[set.x + set.y * size.x].id = id;
        }

        void Act() {
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    switch (GetPixel(x, y).id)
                    {
                    case Pixel_ID::sand: {
                        Pixel_ID test[2] = {empty, water};
                        if (TestForPixel(GetPixel(x, y - 1), test)) {
                            Swap(IntVector2(x, y), IntVector2(x, y - 1));
                        } else if (TestForPixel(GetPixel(x + 1, y - 1), test)) {
                            Swap(IntVector2(x, y), IntVector2(x + 1, y - 1));
                        } else if (TestForPixel(GetPixel(x - 1, y - 1), test)) {
                            Swap(IntVector2(x, y), IntVector2(x - 1, y - 1));
                        }
                        break;
                    }
                    case Pixel_ID::stone: {
                        Pixel_ID test[2] = {empty, water};
                        if (TestForPixel(GetPixel(x, y - 1), test)) {
                            Swap(IntVector2(x, y), IntVector2(x, y-1));
                        }
                        break;
                    }
                    case Pixel_ID::water:
                        if (GetPixel(x, y - 1).id == Pixel_ID::empty) {
                            Swap(IntVector2(x, y), IntVector2(x, y - 1));
                        } else if (GetPixel(x + 1, y - 1).id == Pixel_ID::empty) {
                            Swap(IntVector2(x, y), IntVector2(x + 1, y - 1));
                        } else if (GetPixel(x - 1, y - 1).id == Pixel_ID::empty) {
                            Swap(IntVector2(x, y), IntVector2(x - 1, y - 1));
                        } else if (GetPixel(x + 1, y).id == Pixel_ID::empty) {
                            Swap(IntVector2(x, y), IntVector2(x + 1, y));
                        } else if (GetPixel(x - 1, y).id == Pixel_ID::empty) {
                            Swap(IntVector2(x, y), IntVector2(x - 1, y));
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        void Swap(IntVector2 pixelA, IntVector2 pixelB) {
            Pixel buff = pixels[pixelA.x + pixelA.y * size.x];
            pixels[pixelA.x + pixelA.y * size.x] = pixels[pixelB.x + pixelB.y * size.x];
            pixels[pixelB.x + pixelB.y * size.x] = buff;
        }
};

std::ostream& operator << (std::ostream& os, Board& board) {
    return os << board.size << "w/" << board.pixelSize;
}