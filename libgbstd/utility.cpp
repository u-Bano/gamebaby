#include"utility.hpp"
#include"string.hpp"
#include"text.hpp"
#include<cstdio>


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif




namespace gbstd{


int  reporting_counter;


gbstd::string
make_string_from_file(const char*  filepath) noexcept
{
  gbstd::string  s;

  auto  f = fopen(filepath,"rb");

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          s.append(c);
        }


      fclose(f);
    }


  return std::move(s);
}


void
printf_with_indent(int  indent, const char*  fmt, ...) noexcept
{
  char  buf[256];

  va_list  ap;
  va_start(ap,fmt);

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

  const char*  p = buf;

    while(*p)
    {
      fputc(*p,stdout);

        if(*p++ == '\n')
        {
            for(int  n = 0;  n < indent;  ++n)
            {
              fputc(' ',stdout);
            }
        }
    }
}




void
set_caption(const char*  s) noexcept
{
#ifdef EMSCRIPTEN
  string_form  sf;

  emscripten_run_script(sf(
    "  var  cap = document.getElementById(\"caption\");"
    "  cap.innerHTML = \"%s\";"
  ,s));
#endif
}


void
set_description(const char*  s) noexcept
{
#ifdef EMSCRIPTEN
  string_form  sf;

  emscripten_run_script(sf(
    "  var  desc = document.getElementById(\"description\");"
    "  desc.innerHTML = \"%s\";"
  ,s));
#endif
}


void
generate_saved_image_link(int  x, int  y, int  w, int  h) noexcept
{
#ifdef EMSCRIPTEN
  string_form  sf;

  emscripten_run_script(sf(
    "  var  src = document.getElementById(\"canvas\");"
    "  var  clp = document.createElement(\"canvas\");"
    "  var  ctx = clp.getContext(\"2d\");"
    "  ctx.drawImage(src,%d,%d,%d,%d,0,0,%d,%d);"
    "  var  ln = document.getElementById(\"link_of_saved_image\");"
    "  ln.href = clp.toDataURL();"
    "  ln.download = \"new_image.png\";"
    "  ln.text = \"ここから画像を保存してください\";"
  ,x,y,w,h,w,h));
#endif
}




std::random_device
random_device;


std::default_random_engine
random_number_engine;


}




