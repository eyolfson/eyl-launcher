// Copyright 2014 Jon Eyolfson
//
// This file is distributed under the GPLv3 license

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <pango/pangocairo.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <fcntl.h>
#include <unistd.h>

namespace lnchr {

namespace pango {

class context {
private:
    PangoContext * ptr;
public:
    context();
    ~context();
    PangoContext * data() const { return ptr; }
};

class layout {
private:
    PangoLayout * ptr;
    int w;
    int h;
    double wn;
public:
    layout(const lnchr::pango::context & c, const std::string & font, double whiteness);
    ~layout();
    PangoLayout * data() const { return ptr; }
    void text(const std::string & text)
    {
        pango_layout_set_text(ptr, text.c_str(), -1);
        pango_layout_get_pixel_size(ptr, &w, &h);
    }
    int width() const { return w; }
    int height() const { return h; }
    double whiteness() const { return wn; }
};

}

namespace cairo {

const cairo_format_t FORMAT = CAIRO_FORMAT_ARGB32;

class png {
private:
    cairo_surface_t * ptr;
public:
    png(const std::string & filename);
    ~png();
    bool error() const { return ptr == nullptr; }
    int width() const { return cairo_image_surface_get_height(ptr); }
    int height() const { return cairo_image_surface_get_height(ptr); }
    const unsigned char * data() const
    {
        return cairo_image_surface_get_data(ptr);
    }   
};

class text {
private:
    cairo_surface_t * ptr;
    int w;
    int h;
public:
    text(const pango::layout & l);
    ~text();
    int width() const { return w; }
    int height() const { return h; }
    const unsigned char * data() const
    {
        return cairo_image_surface_get_data(ptr);
    }   
};

}

namespace sdl {

class library {
private:
    int code;
public:
    library();
    ~library();
    bool error() const { return code != 0; }
};

class window {
private:
    SDL_Window * ptr;
    const int ROWS = 7;
    const int WIDTH = 142;
    const int HEIGHT = 5 * (ROWS + 1) + 32 * ROWS;
public:
    window();
    ~window();
    bool error() const { return ptr == nullptr; }
    SDL_Window * data() { return ptr; }
};

class texture {
public:
  const int BYTES_PER_PIXEL = 4;
  const Uint32 FORMAT = SDL_PIXELFORMAT_ARGB8888;
  const int ACCESS = SDL_TEXTUREACCESS_STATIC;
  virtual SDL_Texture * data() const = 0;
  virtual int width() const = 0;
  virtual int height() const = 0;
};

class renderer {
private:
    SDL_Renderer * ptr;
public:
    renderer(lnchr::sdl::window & w);
    ~renderer();
    bool error() const { return ptr == nullptr; }
    SDL_Renderer * data() { return ptr; }
    void clear() { SDL_RenderClear(ptr); }
    void present() { SDL_RenderPresent(ptr); }
    void copy(const texture & t, int x, int y)
    {
        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        dst.w = t.width();
        dst.h = t.height();
        SDL_RenderCopy(ptr, t.data(), nullptr, &dst);
    }
};

class png_texture : public texture {
private:
    SDL_Texture * ptr;
    int w;
    int h;
public:
    png_texture(lnchr::sdl::renderer & r, const std::string & filename);
    ~png_texture();
    bool error() const { return ptr == nullptr; }
    SDL_Texture * data() const { return ptr; }
    int width() const { return w; }
    int height() const { return h; }
};

class text_texture : public texture {
private:
    SDL_Texture * ptr;
    int w;
    int h;
public:
    text_texture(lnchr::sdl::renderer & r, const lnchr::pango::layout & l);
    ~text_texture();
    SDL_Texture * data() const { return ptr; }
    int width() const { return w; }
    int height() const { return h; }
};

}

class entry {
public:
    std::string binding;
    std::string base_name;
};

class display {
public:
    display() {}
    std::unique_ptr<lnchr::sdl::texture> icon;
    std::unique_ptr<lnchr::sdl::text_texture> name;
    std::unique_ptr<lnchr::sdl::text_texture> binding;
};

}

lnchr::pango::context::context()
{
    PangoFontMap * font_map = pango_cairo_font_map_get_default();
    ptr = pango_font_map_create_context(font_map);
}

lnchr::pango::context::~context()
{
    g_object_unref(ptr);
}

lnchr::pango::layout::layout(const lnchr::pango::context & c,
                             const std::string & font,
                             double whiteness)
{
    ptr = pango_layout_new(c.data());
    PangoFontDescription * d = pango_font_description_from_string(font.c_str());
    pango_layout_set_font_description(ptr, d);
    pango_font_description_free(d);
    wn = whiteness;
}

lnchr::pango::layout::~layout()
{
    g_object_unref(ptr);
}

lnchr::cairo::png::png(const std::string & filename)
{
    ptr = cairo_image_surface_create_from_png(filename.c_str());
    if (cairo_image_surface_get_format(ptr) != FORMAT) {
        ptr = nullptr;
    }
}

lnchr::cairo::png::~png()
{
    if (ptr) cairo_surface_destroy(ptr);
}

lnchr::cairo::text::text(const pango::layout & l)
{
    w = l.width();
    h = l.height();
    ptr = cairo_image_surface_create(FORMAT, w, h);
    cairo_t * c = cairo_create(ptr);
    double wn = l.whiteness();
    cairo_set_source_rgba(c, wn, wn, wn, 1.0);
    pango_cairo_show_layout(c, l.data());
    cairo_destroy(c);
}

lnchr::cairo::text::~text()
{
    if (ptr) cairo_surface_destroy(ptr);
}

lnchr::sdl::library::library()
{
    code = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
}

lnchr::sdl::library::~library()
{
    SDL_Quit();
}

lnchr::sdl::window::window()
{
    ptr = SDL_CreateWindow("Launcher", SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED, WIDTH,
                           HEIGHT, SDL_WINDOW_BORDERLESS);
}

lnchr::sdl::window::~window()
{
    if (ptr) SDL_DestroyWindow(ptr);
}

lnchr::sdl::renderer::renderer(lnchr::sdl::window & w)
{
    ptr = SDL_CreateRenderer(w.data(), -1, SDL_RENDERER_ACCELERATED);
}

lnchr::sdl::renderer::~renderer()
{
    if (ptr) SDL_DestroyRenderer(ptr);
}

lnchr::sdl::png_texture::png_texture(lnchr::sdl::renderer & r,
                                     const std::string & filename)
{
    lnchr::cairo::png png(filename);
    if (png.error()) {
        ptr = nullptr;
        return;
    }
    w = png.width();
    h = png.height();
    ptr = SDL_CreateTexture(r.data(), FORMAT, ACCESS, w, h);
    SDL_UpdateTexture(ptr, nullptr, png.data(), w * BYTES_PER_PIXEL);
}

lnchr::sdl::png_texture::~png_texture()
{
    if (ptr) SDL_DestroyTexture(ptr);
}

lnchr::sdl::text_texture::text_texture(lnchr::sdl::renderer & r,
                                       const lnchr::pango::layout & l)
{
    lnchr::cairo::text text(l);
    w = text.width();
    h = text.height();
    ptr = SDL_CreateTexture(r.data(), FORMAT, ACCESS, w, h);
    SDL_UpdateTexture(ptr, nullptr, text.data(), w * BYTES_PER_PIXEL);
}

lnchr::sdl::text_texture::~text_texture()
{
    if (ptr) SDL_DestroyTexture(ptr);
}

using namespace lnchr;

int main(int argc, const char * * argv)
{
    std::vector<entry> config;
    config.push_back({"c", "chromium"});
    config.push_back({"d", "evince"});
    config.push_back({"f", "firefox"});
    config.push_back({"e", "emacs"});
    config.push_back({"m", "mumble"});
    config.push_back({"s", "steam"});
    config.push_back({"t", "termite"});

    sdl::library library;
    if (library.error()) return 1;
    sdl::window window;
    if (window.error()) return 1;
    sdl::renderer renderer(window);
    if (renderer.error()) return 1;

    pango::context context;
    pango::layout name_layout(context, "Sans Bold 12", 1.0);
    pango::layout binding_layout(context, "Sans 8", 0.5);

    std::vector<display> displays;
    for (auto i : config) {
        std::stringstream ss;
        ss << "/usr/share/icons/hicolor/32x32/apps/" << i.base_name << ".png";
        std::unique_ptr<sdl::texture> icon(new sdl::png_texture(renderer, ss.str()));
        name_layout.text(i.base_name);
        std::unique_ptr<sdl::text_texture> name(
            new sdl::text_texture(renderer, name_layout));
        binding_layout.text(i.binding);
        std::unique_ptr<sdl::text_texture> binding(
            new sdl::text_texture(renderer, binding_layout));
        display d;
        d.icon = std::move(icon);
        d.name = std::move(name);
        d.binding = std::move(binding);
        displays.push_back(std::move(d));
    }

    const int X_OFFSET = 5;
    while (true) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
            else if (e.type == SDL_KEYDOWN) {
                SDL_Keycode code = e.key.keysym.sym;
                if (code == SDLK_ESCAPE) {
                    break;
                }
                for (auto i : config) {
                    if (code == SDL_GetKeyFromName(i.binding.c_str())) {
                        pid_t pid = fork();
                        if (pid == 0) {
                            int fd = open("/dev/null", O_RDWR);
                            if (fd < 0) {
                                return 0;
                            }
                            if (dup2(fd, 1) < 0) {
                                return 0;
                            }
                            if (dup2(fd, 2) < 0) {
                                return 0;
                            }
                            execlp(i.base_name.c_str(), i.base_name.c_str(), nullptr);
                        }
                        else {
                            return 0;
                        }
                    }
                }
            }
        }

        renderer.clear();
        int x = X_OFFSET;
        for (auto const & i : displays) {
            renderer.copy(*i.icon, X_OFFSET, x);
            renderer.copy(*i.name, X_OFFSET * 2 + 32, x);
            renderer.copy(*i.binding, X_OFFSET * 2 + 32,
                          x + 32 - i.binding->height());
            x += X_OFFSET + 32;
        }
        renderer.present();
    }
    return 0;
}
