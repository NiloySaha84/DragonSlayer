// code from https://github.com/SFML/SFML/wiki/Source:-Animated-GIF
#include <SFML/Graphics.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF

// MAY NEED TO CHANGE THE FOLLOWING TO WHERE stb WAS CLONED TO  ***
#include "/Users/niloysaha/Documents/stb/stb_image.h"  
// #include<stb_image.h> /* use this for visual studio on windows */

class AnimatedGIF
{
public:
    AnimatedGIF(const char* filename);

    const sf::Vector2i& getSize(void);
    void update(sf::Sprite& sprite);

private:
    sf::Vector2i size;
    sf::Clock clock;
    sf::Time startTime;
    sf::Time totalDelay;
    std::vector<std::tuple<int, sf::Texture>> frames;
    std::vector<std::tuple<int, sf::Texture>>::iterator frameIter;
};

AnimatedGIF::AnimatedGIF(const char* filename)
{
    FILE* f = stbi__fopen(filename, "rb");
    stbi__context s;
    stbi__start_file(&s, f);

    int* delays = 0;
    int z = 0, comp = 0;

    void* pixels = stbi__load_gif_main(&s, &delays, &size.x, &size.y, &z, &comp, STBI_rgb_alpha);

    sf::Image image;
    int step = size.x * size.y * 4;

    for (int i = 0; i < z; i++)
    {
        image.create(size.x, size.y, (const sf::Uint8*)pixels + step * i);

        sf::Texture texture;
        texture.loadFromImage(image);

        frames.push_back(std::tuple<int, sf::Texture>(delays[i], texture));
    }

    frameIter = frames.begin();

    stbi_image_free(pixels);
    if (delays)
        stbi_image_free(delays);
    fclose(f);

    totalDelay = sf::Time::Zero;
    startTime = clock.getElapsedTime();
}

const sf::Vector2i&
AnimatedGIF::getSize()
{
    return size;
}

void
AnimatedGIF::update(sf::Sprite& sprite)
{
    sf::Time delay = sf::milliseconds(std::get<0>(*frameIter));

    while (startTime + totalDelay + delay < clock.getElapsedTime())
    {
        frameIter++;
        if (frameIter == frames.end()) frameIter = frames.begin();
        totalDelay += delay;
        delay = sf::milliseconds(std::get<0>(*frameIter));
    }

    sf::Texture& texture = std::get<1>(*frameIter);
    sprite.setTexture(texture);
}
