#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
#include"libgbstd/widget.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#include<SDL.h>
#endif


using namespace gbstd;


namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 400;


widgets::canvas*
cv;


gbstd::image
image(screen_w,screen_h);


widgets::root
root;


widgets::widget*
radio_menu;


widgets::color_maker*
color_maker;


bool
need_to_hide_cursors;


constexpr int  cv_w = 16;
constexpr int  cv_h = 16;


gbstd::image
cv_image(cv_w,cv_h);


void
undo(widgets::button&  btn) noexcept
{
    if(btn.get_count())
    {
      btn.reset_count();

      cv->undo();
    }
}


void
save(widgets::button&  btn) noexcept
{
    if(btn.get_count())
    {
      btn.reset_count();

#ifdef EMSCRIPTEN
      need_to_hide_cursors = true;

      ptrs::farm->redraw(image);

      need_to_hide_cursors = false;

      sdl::update_screen(image);


      char  buf[256];

      auto&  pt = ptrs::farm->get_absolute_point();
      auto    w = ptrs::farm->get_width();
      auto    h = ptrs::farm->get_height();

      snprintf(buf,sizeof(buf),
        "  var  src = document.getElementById(\"canvas\");"
        "  var  clp = document.createElement(\"canvas\");"
        "  var  ctx = clp.getContext(\"2d\");"
        "  ctx.drawImage(src,%d,%d,%d,%d,0,0,%d,%d);"
        "  var  img = document.getElementById(\"img\");"
        "  img.src = clp.toDataURL();",pt.x,pt.y,w,h,w,h);


      emscripten_run_script(buf);
#else
        for(int  y = 0;  y < cv_h;  ++y)
        {
            for(int  x = 0;  x < cv_w;  ++x)
            {
              cv_image.get_pixel(x,y).color.print();

              printf(",");
            }


          printf("\n");
        }


      printf("\n");
#endif
    }
}


void
main_loop()
{
  sdl::update_controller();

  root.react();

    if(root.redraw_only_needed_widgets(image) || ctrl.is_needed_to_redraw())
    {
      sdl::update_screen(image);
    }
}


void
change_mode(widgets::radio_button&  btn, uint32_t  old_state, uint32_t  new_state)
{
    switch(new_state)
    {
  case(0x01): cv->change_mode_to_draw_dot();break;
  case(0x02): cv->change_mode_to_draw_line();break;
  case(0x04): cv->change_mode_to_draw_rectangle();break;
  case(0x08): cv->change_mode_to_fill_rectangle();break;
  case(0x10): cv->change_mode_to_fill_area();break;
    }
}




}


int
main(int  argc, char**  argv)
{
  sdl::init(screen_w,screen_h);


  cv = new widgets::canvas(cv_image,nullptr);

  cv->set_grid();
  cv->set_pixel_size(12);

  color_sample = new widgets::color_maker([](widgets::color_maker&  cm,images::color  color){cv->set_drawing_color(color);});


  auto  undo_btn = new widgets::button(new widgets::label(u"UNDO"),undo);
  auto  save_btn = new widgets::button(new widgets::label(u"SAVE"),save);

  auto  mcol = new widgets::table_column({color_sample,undo_btn,save_btn});

  radio_menu = widgets::create_radio_menu({new widgets::label(u"draw dot"),
                                           new widgets::label(u"draw line"),
                                           new widgets::label(u"draw rectangle"),
                                           new widgets::label(u"fill rectangle"),
                                           new widgets::label(u"fill area"),
                                           },change_mode,1);

  auto  row = new widgets::table_row({cv,mcol,radio_menu});

  root->append_child(row,0,0);

  root->show_all();

  update_color();


#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


//  sdl::quit();

  return 0;
}




